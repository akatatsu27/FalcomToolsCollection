using System.Text;

namespace archive_extractor
{
    internal class dir_file
    {
        static readonly byte[] magic = { 0x4c, 0x42, 0x20, 0x44, 0x49, 0x52, 0x1a, 0x00 }; // ["LB DIR", 0x1a, 0x00]
        ulong entries_num;
        public Entry[] entries { get; private set; }

        public class Entry
        {
            public string file_name { get; private set; } // fixed size 12 bytes (not null terminated), encoding: shift-jis
            uint unk1;
            public uint length { get; private set; }
            uint unk3;
            uint length_copy;
            uint timestamp;
            public uint dat_offset { get; private set; }

            public bool Parse(BinaryReader br)
            {
                var encoding = Encoding.GetEncoding("shift-jis");
                file_name = encoding.GetString(br.ReadBytes(12));
                unk1 = br.ReadUInt32();
                length = br.ReadUInt32();
                unk3 = br.ReadUInt32();
                length_copy = br.ReadUInt32();
                timestamp = br.ReadUInt32();
                dat_offset = br.ReadUInt32();
                if(length != length_copy)
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
