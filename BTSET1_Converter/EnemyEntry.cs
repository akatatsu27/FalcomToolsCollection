using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class EnemyEntry
    {
        [JsonProperty(Required = Required.Always)] public readonly UInt16 Index;
        [JsonIgnore] internal UInt16 DataTable;

        //Metadata
        private UInt16 Offset;
        [JsonProperty("DataTable", Required = Required.Always)] public string DataTableName;

        internal EnemyEntry(ref UInt16 offset, BTSET1 file)
        {
            Offset = offset;

            Index = file.ReadUInt16(ref offset);
            DataTable = file.ReadUInt16(ref offset);

            DataTableName = $"ED6_DT{DataTable.ToString("X")}";
        }

        //For Newtonsoft only
        public EnemyEntry() { }

        internal byte[] ToByteArray()
        {
            return BitConverter.GetBytes(Index).Concat(BitConverter.GetBytes(DataTable)).ToArray();
        }
    }
}
