using System.Buffers.Binary;
using System.Text;

namespace Shared
{
    public static class Extensions
    {
        public static ushort ReadU16(this byte[] bs, int position) => BinaryPrimitives.ReadUInt16LittleEndian(bs.AsSpan()[position..]);
        public static uint ReadU32(this byte[] bs, long position) => BinaryPrimitives.ReadUInt32LittleEndian(bs.AsSpan()[(int)position..]);
        public static string ReadCString(this BinaryReader br)
        {
			List<byte> strBytes = new();
			while (true)
			{
                byte theByte = br.ReadByte();
                strBytes.Add(theByte);

                if (theByte == 0)
                    break;
			}
            
            string str = Encoding.ASCII.GetString(strBytes.ToArray());
            return str;
		}
    }
}
