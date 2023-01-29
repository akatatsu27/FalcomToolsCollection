using Shared;

namespace BTSET_Converter
{
	internal abstract class BTLSET
	{
		internal abstract Dictionary<UInt16, Bonuses> BonusesTable { get; set; }
		internal abstract Dictionary<UInt16, ModelEntry> ModelTable { get; set; }
		internal abstract Dictionary<UInt16, Placement[]> PlacementTable { get; set; }
		internal abstract Dictionary<UInt16, AutoBattleEntry> AutoBattles { get; set; }
		protected byte[]? Data;
		protected bool[]? Coverage; // set to true if the byte has been read at least once. If all true => full coverage
		protected bool IsFullCoverage => Coverage.All(w => w == true);

		internal ModelEntry ReadBattlefield(UInt16 offset, Battle battleEntry)
			=> ModelTable[offset].AddReference(battleEntry);
		internal ModelEntry ReadBattlefield(UInt16 offset, FC_BattleEntry battleEntry)
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
	}
}
