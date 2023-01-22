using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class Bonuses
    {
        [JsonProperty(Required = Required.Always)] public readonly byte Unk00;
        [JsonProperty(Required = Required.Always)] public readonly byte Hp10;
        [JsonProperty(Required = Required.Always)] public readonly byte Hp50;
        [JsonProperty(Required = Required.Always)] public readonly byte Ep10;
        [JsonProperty(Required = Required.Always)] public readonly byte Ep50;
        [JsonProperty(Required = Required.Always)] public readonly byte Cp10;
        [JsonProperty(Required = Required.Always)] public readonly byte Cp50;
        [JsonProperty(Required = Required.Always)] public readonly byte Atk10;
        [JsonProperty(Required = Required.Always)] public readonly byte Atk50;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk09;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk0A;
        [JsonProperty(Required = Required.Always)] public readonly byte Sepith;
        [JsonProperty(Required = Required.Always)] public readonly byte Crit;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk0D;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk0E;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk0F;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk10;
        [JsonProperty(Required = Required.Always)] public readonly byte Unk11;

        //Metadata
        List<Variation> UsedByList = new();
        internal UInt16 Offset;

        internal Bonuses(ref UInt16 offset, BTSET1 file)
        {
            Offset = offset;

            Unk00 = file.ReadUInt8(ref offset);
            Hp10 = file.ReadUInt8(ref offset);
            Hp50 = file.ReadUInt8(ref offset);
            Ep10 = file.ReadUInt8(ref offset);
            Ep50 = file.ReadUInt8(ref offset);
            Cp10 = file.ReadUInt8(ref offset);
            Cp50 = file.ReadUInt8(ref offset);
            Atk10 = file.ReadUInt8(ref offset);
            Atk50 = file.ReadUInt8(ref offset);
            Unk09 = file.ReadUInt8(ref offset);
            Unk0A = file.ReadUInt8(ref offset);
            Sepith = file.ReadUInt8(ref offset);
            Crit = file.ReadUInt8(ref offset);
            Unk0D = file.ReadUInt8(ref offset);
            Unk0E = file.ReadUInt8(ref offset);
            Unk0F = file.ReadUInt8(ref offset);
            Unk10 = file.ReadUInt8(ref offset);
            Unk11 = file.ReadUInt8(ref offset);
        }
        //For Newtonsoft only
        public Bonuses() { }
        internal Bonuses AddReference(Variation variation)
        {
            UsedByList.Add(variation);
            return this;
        }

        internal byte[] ToByteArray()
        {
            return new byte[] { Unk00, Hp10, Hp50, Ep10, Ep50, Cp10, Cp50, Atk10, Atk50, Unk09, Unk0A, Sepith, Crit, Unk0D, Unk0E, Unk0F, Unk10, Unk11 };
        }
    }
}
