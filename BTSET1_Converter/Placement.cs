using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class Placement
    {
        [JsonProperty(Required = Required.Always)] public readonly byte X;
        [JsonProperty(Required = Required.Always)] public readonly byte Y;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Rot;

        //Metadata
        internal UInt16 Offset;
        List<Variation> UsedByList = new();

        internal Placement(ref UInt16 offset, BTSET1 file)
        {
            Offset = offset;

            X = file.ReadUInt8(ref offset);
            Y = file.ReadUInt8(ref offset);
            Rot = file.ReadUInt16(ref offset);
        }
        //For Newtonsoft only
        public Placement() { }
        internal Placement AddReference(Variation variation)
        {
            UsedByList.Add(variation);
            return this;
        }

        internal byte[] ToByteArray()
        {
            return (new byte[] { X, Y }).Concat(BitConverter.GetBytes(Rot)).ToArray();
        }
    }
}
