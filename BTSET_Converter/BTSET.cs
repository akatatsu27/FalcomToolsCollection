using Shared;

namespace BTSET1_Converter
{
    internal abstract class BTSET
    {
        internal virtual Dictionary<UInt16, Bonuses> BonusesTable { get; set; }
        internal virtual Dictionary<UInt16, ModelEntry> ModelTable { get; set; }
        internal virtual Dictionary<UInt16, Placement[]> PlacementTable { get; set; } //84 sets of 8 placements

        protected virtual byte[] Data { get; set; }
        protected virtual bool[] Coverage { get; set; } // set to true if the byte has been read at least once. If all true => full coverage

        internal ModelEntry ReadBattlefield(UInt16 offset, Battle battleEntry)
            => ModelTable[offset].AddReference(battleEntry);

        internal Bonuses ReadBonuses(UInt16 offset, Variation variation)
            => BonusesTable[offset].AddReference(variation);

        internal Placement[] ReadPlacement(ref UInt16 offset, Variation variation)
            => PlacementTable[offset];

        internal byte ReadUInt8(ref UInt16 position)
        {
            byte num = Data[position];
            Coverage[position++] |= true;

            return num;
        }

        internal UInt16 ReadUInt16(ref UInt16 position)
        {
            UInt16 num = Data.ReadU16(position);
            Coverage[position++] |= true;
            Coverage[position++] |= true;

            return num;
        }

        internal UInt32 ReadUInt32(ref UInt16 position)
        {
            UInt32 num = Data.ReadU32(position);
            Coverage[position++] |= true;
            Coverage[position++] |= true;
            Coverage[position++] |= true;
            Coverage[position++] |= true;

            return num;
        }

        internal string ReadString(ref UInt16 position)
        {
            UInt16 length = 0;
            while (Data[position + length] != 0) length++;
            string theString = System.Text.Encoding.ASCII.GetString(Data, position, length);
            for (int i = 0; i <= length; i++)
            {
                Coverage[position++] |= true;
            }
            return theString;
        }

        internal bool GetCoverageUInt8(UInt16 position)
        {
            bool coverage = false;
            coverage = Coverage[position];

            return coverage;
        }

        internal bool[] GetCoverageUInt16(UInt16 position)
        {
            bool[] coverage = { false, false };
            coverage[0] = Coverage[position];
            coverage[1] = Coverage[position + 1];

            return coverage;
        }

        internal bool[] GetCoverageUInt32(UInt16 position)
        {
            bool[] coverage = { false, false };
            coverage[0] = Coverage[position];
            coverage[1] = Coverage[position + 1];
            coverage[2] = Coverage[position + 2];
            coverage[3] = Coverage[position + 3];

            return coverage;
        }
    }
}
