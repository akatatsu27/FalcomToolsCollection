namespace archive_extractor
{
    internal class decompressor
    {
        ushort chunk_length;
        byte current_byte => (byte)br.PeekChar();
        BinaryReader br;

        public async Task<byte[]> decompress(byte[] input_bytes)
        {
            await using var output = new MemoryStream();
            using var bw = new BinaryWriter(output);

            await using var input = new MemoryStream(input_bytes);
            br = new BinaryReader(input);

            do
            {
                chunk_length = br.ReadUInt16();
                
                if (current_byte == 0)
                {
                    decompress_type1_chunk(br, bw);
                }
                else
                {
                    decompress_type2_chunk(br, bw);
                }
            } while (current_byte != 0);
            return output.ToArray();
        }
#region type2 chunk
        /*
         * Format:
         *  ushort: chunk length
         *  subchunk:
         *      byte: bitflags + number
         *      byte[] (optional): compressed data 
         *  read subchunks till total bytes read == chunk length
         */
#endregion
        private void decompress_type2_chunk(BinaryReader br, BinaryWriter bw)
        {
            int bytes_read = 2;
            if(chunk_length == 2)
            {
                return;
            }

            while(chunk_length != bytes_read)
            {
                byte subChunkType = br.ReadByte();
                bytes_read++;

                int num;
                int offset_from_end = 0;
                if((subChunkType & 0B10000000) != 0) // case: 0B1NNX_XXXX
                {
                    num = (subChunkType & 0B01100000) >> 5; // get NN
                    offset_from_end = (subChunkType & 0B00011111) << 8 | br.ReadByte();
                    bytes_read++;
                    AppendFromOutputBacklog(bw, num + 4, offset_from_end);
                }/*
                else if((subChunkType & 0B01000000) != 0 && (subChunkType & 0B00100000) != 0) // case: 0B011N_NNNN
                {
                    num = subChunkType & 0B00011111; // low 5 bits
                    AppendFromOutputBacklog(bw, num, offset_from_end);
                }*/
                else if((subChunkType & 0B01000000) != 0) // case: 0B010X_NNNN
                {
                    num = subChunkType & 0B00001111; // low 4 bits
                    if ((subChunkType & 0B00010000) != 0) // if X == 1
                    {
                        num = (num << 8) | br.ReadByte();
                        bytes_read++;
                    }
                    num += 4;
                    byte next_byte = br.ReadByte();
                    bytes_read++;
                    for (int i = 0; i < num; i++) // write the next byte num times
                        bw.Write(next_byte);                                
                }
                else // case: 0B00XN_NNNN
                {
                    num = subChunkType & 0B00011111; // low 5 bits
                    if ((subChunkType & 0B00100000) != 0) // if X == 1
                    {
                        num = (num << 8) | br.ReadByte();
                        bytes_read++;
                    }
                    bw.Write(br.ReadBytes(num)); // directly copy #num bytes

                    bytes_read += num;
                }
            }
        }
        // smth smth flags idk
        private void decompress_type1_chunk(BinaryReader br, BinaryWriter bw)
        {
            int bitflags = br.ReadUInt16();
            int index = 8;
            bool ReadBitFlag()
            {
                if (index == 16)
                {
                    // read next available byte to get new flags
                    bitflags = br.ReadUInt16();
                    index = 0;
                }
                return (bitflags & (1 << index++)) != 0; // check flag at index, then increment the index
            }
            int SumBitFlags(int n)
            {
                int x = 0;
                while ((n--) != 0) x |= (ReadBitFlag() ? 1 : 0) << n;
                return x;
            }
            void repeatBackLog(int pos)
            {
                int count = ReadBitFlag() ? 2 : ReadBitFlag() ? 3 : ReadBitFlag() ? 4 : ReadBitFlag() ? 5 : ReadBitFlag() ? 6 + SumBitFlags(3) : 14 + br.ReadByte();
                AppendFromOutputBacklog(bw, count, pos);
            }

            while(true)
            {
                if (!ReadBitFlag())
                    bw.Write(br.ReadByte());
                else if (!ReadBitFlag())
                    repeatBackLog(br.ReadByte());
                else
                {
                    int x = (SumBitFlags(5) << 8) | br.ReadByte();
                    if (x == 1)
                    {
                        int count = 14 + (ReadBitFlag() ? (SumBitFlags(4) << 8) | br.ReadByte() : SumBitFlags(4));
                        byte nextByte = br.ReadByte();
                        while (count-- != 0) bw.Write(nextByte);
                    }
                    else if (x != 0)
                        repeatBackLog(x);
                    else
                        break;
                }
            }
        }

        private void AppendFromOutputBacklog(BinaryWriter bw, int num_bytes, int offset_from_end)
        {
            //Go back "offset_from_end" bytes in the output stream, and copy the first "num_bytes" bytes from that position to the end of the buffer
            for (int i = 0; i < num_bytes; i++)
            {
                byte byteToCopy = (bw.BaseStream as MemoryStream).GetBuffer()[bw.BaseStream.Position - offset_from_end];
                bw.Write(byteToCopy); // stream position is advanced automatically
            }
        }
    }
}
