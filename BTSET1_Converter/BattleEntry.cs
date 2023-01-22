using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal abstract class Battle
    {
    }

    internal class BattleEntry : Battle
    {
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Id;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Flags;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk04;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk06;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk08;
        internal UInt16 FieldOffset;
        [JsonIgnore] public byte[] VariationWeights = new byte[4];      
        [JsonProperty(Required = Required.Always)] public readonly Variation?[] Variations = { null, null, null, null }; // at most 4 different variations. First one is mandatory
        
        //Metadata
        internal UInt16 Offset;
        internal ModelEntry BattlefieldEntry;
        [JsonProperty(Required = Required.Always)] public readonly string BattlefieldName;
        [JsonProperty("VariationWeights", Required = Required.Always)] public int[] JsonBytes;

        internal BattleEntry(ref UInt16 offset, BTSET1 file, bool canBeEmpty)
        {
            Offset = offset;

            Id = file.ReadUInt16(ref offset);
            Flags = file.ReadUInt16(ref offset);
            Unk04 = file.ReadUInt16(ref offset);
            Unk06 = file.ReadUInt16(ref offset);
            Unk08 = file.ReadUInt16(ref offset);
            FieldOffset = file.ReadUInt16(ref offset);
            VariationWeights[0] = file.ReadUInt8(ref offset);
            VariationWeights[1] = file.ReadUInt8(ref offset);
            VariationWeights[2] = file.ReadUInt8(ref offset);
            VariationWeights[3] = file.ReadUInt8(ref offset);

            BattlefieldEntry = file.ReadBattlefield(FieldOffset, this);
            if (canBeEmpty)
            {
                if (VariationWeights[0] != 0) Variations[0] = new Variation(ref offset, file, this);
            }
            else
            {
                Variations[0] = new Variation(ref offset, file, this);
            }
            if (VariationWeights[1] != 0) Variations[1] = new Variation(ref offset, file, this);
            if (VariationWeights[2] != 0) Variations[2] = new Variation(ref offset, file, this);
            if (VariationWeights[3] != 0) Variations[3] = new Variation(ref offset, file, this);

            BattlefieldName = BattlefieldEntry.Filename;
            JsonBytes = VariationWeights?.Select(x => (int)x).ToArray() ?? new int[0];
        }

        //For Newtonsoft only
        public BattleEntry() { }

        internal byte[] ToByteArray()
        {
            IEnumerable<byte> data = BitConverter.GetBytes(Id).Concat(BitConverter.GetBytes(Flags))
                .Concat(BitConverter.GetBytes(Unk04)).Concat(BitConverter.GetBytes(Unk06))
                .Concat(BitConverter.GetBytes(Unk08)).Concat(BitConverter.GetBytes(FieldOffset))
                .Concat(VariationWeights);
            foreach(Variation? var in Variations)
            {
                if (var == null) continue;
                data = data.Concat(var.ToByteArray());
            }

            return data.ToArray();
        }
    }
}
