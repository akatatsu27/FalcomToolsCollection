using System.Text;

namespace Shared
{
    public class dir_file
    {
        static readonly byte[] magic = { 0x4c, 0x42, 0x20, 0x44, 0x49, 0x52, 0x1a, 0x00 }; // ["LB DIR", 0x1a, 0x00]
        ulong entries_num;
        public Entry[] entries { get; private set; }

        public class Entry
        {
            public string file_name { get; private set; } // fixed size 12 bytes (not null terminated), encoding: shift-jis
            public uint timestamp1 { get; private set; } // almost always null
            public uint compressed_size { get; private set; }
            public uint decompressed_size { get; private set; } // 99% of the time. other times it's 2*n*0x2000, or same as compressed_size
            public uint compressed_size_copy; // literally no idea why
            public uint timestamp2 { get; private set; }
            public uint dat_offset { get; private set; }

            public bool Parse(BinaryReader br)
            {
                var encoding = Encoding.GetEncoding("shift-jis");
                file_name = encoding.GetString(br.ReadBytes(12));
                timestamp1 = br.ReadUInt32();
                compressed_size = br.ReadUInt32();
                decompressed_size = br.ReadUInt32();
                compressed_size_copy = br.ReadUInt32();
                timestamp2 = br.ReadUInt32();
                dat_offset = br.ReadUInt32();
                if(compressed_size != compressed_size_copy)
                {
                    return false;
                }
                return true;
            }
        }

        public async Task<string> Parse(string filepath)
        {
            byte[] bytes = await File.ReadAllBytesAsync(filepath);
            await using var ms = new MemoryStream(bytes);
            using var br = new BinaryReader(ms);

            byte[] signature = br.ReadBytes(magic.Length);
            if (!magic.SequenceEqual(signature))
            {
                return "bad signature";
            }
            entries_num = br.ReadUInt64();
            entries = new Entry[entries_num];
            for (ulong i = 0; i < entries_num; i++)
            {
                entries[i] = new();
                if(!entries[i].Parse(br))
                {
                    return $"failed to parse entry {i}";
                }
            }
            if (br.BaseStream.Position == br.BaseStream.Length) // did we reach the end of the file?
            {
                return "reached eos";
            }
            return "failed to read whole file";
        }
    }
}
