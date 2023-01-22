using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class AutoBattleEntry : Battle
    {
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Id;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk02;
        [JsonIgnore] internal UInt16 FieldOffset;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk06;
        [JsonProperty(Required = Required.Always)] public readonly EnemyEntry[] Side1 = new EnemyEntry[8];
        [JsonProperty(Required = Required.Always)] public readonly EnemyEntry[] Side2 = new EnemyEntry[8];
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Bgm;
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk4A;

        //Metadata
        [JsonIgnore] internal UInt16 Offset;
        [JsonIgnore] public readonly ModelEntry Battlefield;
        [JsonProperty("Battlefield", Required = Required.Always)] public readonly string BattlefieldName;

        internal AutoBattleEntry(ref UInt16 offset, BTSET1 file)
        {
            Offset = offset;

            Id = file.ReadUInt16(ref offset);
            Unk02 = file.ReadUInt16(ref offset);
            FieldOffset = file.ReadUInt16(ref offset);
            Unk06 = file.ReadUInt16(ref offset);
            for (byte i = 0; i < 8; i++)
            {
                Side1[i] = new EnemyEntry(ref offset, file);
            }
            for (byte i = 0; i < 8; i++)
            {
                Side2[i] = new EnemyEntry(ref offset, file);
            }
            Bgm = file.ReadUInt16(ref offset);
            Unk4A = file.ReadUInt16(ref offset);


            Battlefield = file.ReadBattlefield(FieldOffset, this);
            BattlefieldName = Battlefield.Filename;
        }

        //For Newtonsoft only
        public AutoBattleEntry() { }

        internal byte[] ToByteArray()
        {
            IEnumerable<byte> data = BitConverter.GetBytes(Id).Concat(BitConverter.GetBytes(Unk02))
                .Concat(BitConverter.GetBytes(FieldOffset)).Concat(BitConverter.GetBytes(Unk06));

            foreach(EnemyEntry entry in Side1.Concat(Side2))
            {
                data = data.Concat(entry.ToByteArray());
            }
            data = data.Concat(BitConverter.GetBytes(Bgm)).Concat(BitConverter.GetBytes(Unk4A));

            return data.ToArray();
        }
    }
}
