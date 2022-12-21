using System.Diagnostics;

namespace archive_extractor
{
    /*
     * The compressed byte stream has the following structure:
     * 
     * -------------------------------
     * Chunk:
     *  uint16: chunk_length
     *  uint8: type
     *  uint8[chunk_length - 3]: data
     * 
     * uint8: # of remaining chunks
     * 
     * repeat Chunks till # of remaining chunks == 0
     * -------------------------------
     * 
     * Chunks are split into two types.
     * If the byte after chunk_length is 0x00, then it's type1, and it's type2 otherwise.
     */

    class Chunk
    {
        ushort chunkLength;
        byte type;
        byte[] input;
        int position;
        public byte[]? output { get; private set; }

        public Chunk(byte[] stream, ref int streamPos)
        {
            chunkLength = stream.ReadU16(streamPos);
            type = stream[streamPos + 2];
            input = stream[streamPos..(streamPos + chunkLength)];
            streamPos += chunkLength;
        }

        public async Task decompress()
        {
            if (type == 0)
            {
                await decompress1();
            }
            else
            {
                await decompress2();
            }
        }
        #region type2 subchunk
        /*
         * ---------------------
         *  ushort: bitflags
         *  byte[]: compressed data 
         *  ushort: moar bitflags
         *  read subchunks till position == chunk length
         * ---------------------
         * 
         * How to check the bitflags:
         *  Check against the least significant bit,
         *  then shift right to get the next bit.
         *  When the bitflags are exhausted, read the 
         *  next ushort (little endian) from the stream.
         *  The FIRST bitflags are an exception, as the 
         *  first byte is always 0x00, so we do half a 
         *  cycle, and start checking from the high byte
         *  of the bitflags.
         * 
         */
        #endregion
        private async Task decompress1()
        {
            position = 2; // skip chunkLength
            int bitflags = input[position++] | (input[position++] << 8); // read ushort little endian
            int bitIndex = 8; // the first bitflag is special, so we start counting the bit index from 8 instead of 0. 
            bitflags = bitflags >> 8;
            await using var ms = new MemoryStream();
            using var bw = new BinaryWriter(ms);
            // local helper functions ------------------------------------//
            bool ReadBitFlag()
            {
                if (bitIndex == 0)
                {
                    // read next available ushort to get new flags
                    bitflags = input[position++] | (input[position++] << 8);
                    bitIndex = 16;
                }
                --bitIndex;
                bool flag = (bitflags & 1) != 0;
                bitflags = bitflags >> 1;
                return flag;
            }
            int SumBitFlags(int n)
            {
                int x = 0;
                while ((n--) != 0) x |= (ReadBitFlag() ? 1 : 0) << n;
                return x;
            }
            void repeatBackLog(int pos)
            {
                int count = ReadBitFlag() ? 2 : ReadBitFlag() ? 3 : ReadBitFlag() ? 4 : ReadBitFlag() ? 5 : ReadBitFlag() ? 6 + SumBitFlags(3) : 14 + input[position++];
                try
                {
                    for (int i = 0; i < count; i++)
                    {
                        byte byteToCopy = ms.GetBuffer()[bw.BaseStream.Position - pos];
                        bw.Write(byteToCopy); // stream position is advanced automatically
                    }
                }
                catch
                {
                    Console.WriteLine("Type1 chunk: tried to copy inexistent bytes from previous output");
                }
            }
            //-----------------------------------------------------------//

            while (true)
            {
                if (position == input.Length) // algorithm may need correction
                {
#if DEBUG
                    Debugger.Break();
                    Console.WriteLine("Type1 chunk: Unexpected end of stream");
#endif
                    break;
                }
                if (!ReadBitFlag())
                    bw.Write(input[position++]);
                else if (!ReadBitFlag())
                    repeatBackLog(input[position++]);
                else
                {
                    int x = (SumBitFlags(5) << 8) | input[position++];
                    if (x == 1)
                    {
                        int count = 14 + (ReadBitFlag() ? (SumBitFlags(4) << 8) | input[position++] : SumBitFlags(4));
                        byte nextByte = input[position++];
                        while (count-- != 0) bw.Write(nextByte);
                    }
                    else if (x != 0)
                        repeatBackLog(x);
                    else
                        break;
                }
            }
            output = ms.ToArray();
        }
        #region type2 subchunk
        /*
         *  byte: bitflags + number
         *  byte[] (optional): compressed data 
         *  read subchunks till position == chunk length
         */
        #endregion
        private async Task decompress2()
        {
            position = 2; // skip chunkLength
            await using var ms = new MemoryStream();
            using var bw = new BinaryWriter(ms);
            while (chunkLength > position)
            {
                byte subChunkType = input[position++];
                int num;
                if ((subChunkType & 0B10000000) != 0) // case: 0B1NNX_XXXX
                {
                    int offset = (subChunkType & 0B00011111) << 8 + input[position++];
                    num = ((subChunkType & 0B01100000) >> 5) + 4; // NN + 4
                    if(chunkLength != position)
                    {
                        byte nextByte = input[position++];
                        while((nextByte & 0xe0) == 0x60)
                        {
                            num += nextByte & 0B00011111;
                            nextByte = input[position++];
                        }
                    }
                    if(offset < num)
                    {
                        if(offset < 4)
                        {
                            if(num > 0)
                            {
                                while(num-- != 0)
                                {
                                    byte byteToCopy = ms.GetBuffer()[ms.Position - offset];
                                    bw.Write(byteToCopy);
                                }
                            }
                        }
                        else
                        {
                            for (int i = (int)((num >> 0x1f & 0xfU) + num) >> 4; 0 < i; i--)
                            {
                                uint uintToCopy = ms.GetBuffer().ReadU32(ms.Position - offset);
                                bw.Write(uintToCopy);
                                uintToCopy = ms.GetBuffer().ReadU32(ms.Position - offset + 4);
                                bw.Write(uintToCopy);
                                uintToCopy = ms.GetBuffer().ReadU32(ms.Position - offset + 8);
                                bw.Write(uintToCopy);
                                uintToCopy = ms.GetBuffer().ReadU32(ms.Position - offset + 12);
                                bw.Write(uintToCopy);
                            }
                            num = (int)(num & 0x8000000f);
                            if ((int)num < 0)
                            {
                                num = (int)(num - 1 | 0xfffffff0) + 1;
                            }
                            for (; 0 < (int)num; num = num - 1)
                            {
                                byte byteToCopy = ms.GetBuffer()[ms.Position - offset];
                                bw.Write(byteToCopy);
                            }
                        }
                    }
                    else
                    {
                        //poor man's memcpy
                        for(int i = 0; i < num; i++)
                        {
                            byte byteToCopy = ms.GetBuffer()[ms.Position - offset];
                            bw.Write(byteToCopy);
                        }
                    }
                    if (chunkLength == position)
                        return;
                }
                else if((subChunkType & 0B01000000) != 0) // cases: 0B011X_NNNN, 0B010X_NNNN
                {
                    byte val = input[position++];
                    if((subChunkType & 0B00010000) != 0) // if X == 1
                    {
                        num = val + 4 + (subChunkType & 0B00001111) << 8;
                        byte val2 = input[position++];
                        for (int i = 0; i < num; i++)
                            bw.Write(val2);
                    }
                    else
                    {
                        num = (subChunkType & 0B00001111) + 4; // NNNN + 4
                        for (int i = 0; i < num; i++)
                            bw.Write(val);
                    }
                }/*
                else if ((subChunkType & 0B01000000) != 0) // case: 0B010X_NNNN
                {
                    num = subChunkType & 0B00001111; // low 4 bits
                    if ((subChunkType & 0B00010000) != 0) // if X == 1
                    {
                        num = (num << 8) | input[position++];
                    }
                    num += 4;
                    byte next_byte = input[position++];
                    for (int i = 0; i < num; i++) // write the next byte num times
                        bw.Write(next_byte);
                }*/
                else // case: 0B00XN_NNNN
                {
                    num = subChunkType & 0B00011111; // low 5 bits
                    if ((subChunkType & 0B00100000) != 0) // if X == 1
                    {
                        num = (num << 8) | input[position++];
                    }
                    int i = 0;
                    try
                    {
                        for (i = 0; i < num; i++)
                            bw.Write(input[position++]); // directly copy #num bytes
                    }
                    catch
                    {
                        Console.WriteLine("Type2 chunk: tried to read beyond the input");
                    }
                }
            }
            output = ms.ToArray();
        }
    }
    internal class decompressor
    {
        public async Task<byte[]> decompress(byte[] input_bytes)
        {
            int pos = 0;
            List<Chunk> chunks = new();
            do
            {
                chunks.Add(new(input_bytes, ref pos));
                pos++; // are there more chunks?
            } while (pos != input_bytes.Length && input_bytes[pos] != 0);
            var tasks = chunks.Select(c => c.decompress());
            await Task.WhenAll(tasks);
            await using var ms = new MemoryStream();
            using var bw = new BinaryWriter(ms);
            for (int i = 0; i < chunks.Count; i++)
            {
                bw.Write(chunks[i].output);
            }
            return ms.ToArray();
        }
    }
}
