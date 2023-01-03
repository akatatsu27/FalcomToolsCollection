namespace Shared
{
    public class dat_file
    {
        Header header;
        public Body body;

        public class Header
        {
            public static readonly byte[] magic = { 0x4c, 0x42, 0x20, 0x44, 0x41, 0x54, 0x1a, 0x00 }; // ["LB DAT", 0x1a, 0x00]
            public ulong entries_num { get; private set; }
            public uint[] entry_boundaries { get; private set; }

            public string Parse(BinaryReader br)
            {
                byte[] signature = br.ReadBytes(magic.Length);
                if (!magic.SequenceEqual(signature))
                {
                    return "bad signature";
                }
                entries_num = br.ReadUInt64();
                entry_boundaries = new uint[entries_num + 1];
                for (ulong i = 0; i < entries_num + 1; i++)
                {
                    entry_boundaries[i] = br.ReadUInt32();
                }
                if (br.BaseStream.Position != entry_boundaries[0])
                {
                    return "invalid begin of data";
                }
                return "valid header";
            }
        }
        
        public class Body
        {
            public Dictionary<uint, byte[]> entries; // <offset, data>

            public string Parse(BinaryReader br, Header header)
            {
                entries = new();

                for(ulong i = 0; i < header.entries_num; i++)
                {
                    uint offset = header.entry_boundaries[i];
                    long length = header.entry_boundaries[i + 1] - offset;
                    byte[] data = br.ReadBytes((int)length);
                    entries[offset] = data;
                    if (header.entry_boundaries[i] + length == br.BaseStream.Length) // did we reach the eos?
                    {
                        break;
                    }
                }

                if(br.BaseStream.Position == br.BaseStream.Length) // did we reach the end of the file?
                {
                    return "valid body";
                }
                return "failed to read whole file";
            }
        }

        public async Task<string> Parse(string filepath)
        {
            byte[] bytes = await File.ReadAllBytesAsync(filepath);
            await using var ms = new MemoryStream(bytes);
            using var br = new BinaryReader(ms);

            header = new Header();
            string header_status = header.Parse(br);
            if (header_status != "valid header")
            {
                return header_status;
            }
            body = new Body();
            string body_status = body.Parse(br, header);
            if (body_status != "valid body")
            {
                return body_status;
            }
            return "reached eos";
        }
    }
}
