using System.Diagnostics;
using archive_extractor;
using static Shared.dir_file;

namespace BTSET1_Converter
{
	internal class BTSET1
	{
		// This tool is meant for analysis, so it's also keeping track of who is accessing fields.
		internal UInt16 BattleEntryTableOffset;
		internal readonly Dictionary<UInt16, Bonuses> BonusesTable = new();
		internal readonly Dictionary<UInt16, ModelEntry> ModelTable = new();
		internal readonly Dictionary<UInt16, Placement[]> PlacementTable = new(); //84 sets of 8 placements
		private List<UInt16> BattleEntryOffsets = new();
		internal readonly Dictionary<UInt16, BattleEntry> Battles1 = new();
		internal readonly Dictionary<UInt16, AutoBattleEntry> AutoBattles = new();
		internal readonly Dictionary<UInt16, BattleEntry> Battles2 = new();
		private byte[] Data;
		private bool[] Coverage; // set to true if the byte has been read at least once. If all true => full coverage
		private bool IsFullCoverage => Coverage.All(w => w == true);
		private IEnumerable<UInt16> UnusedBattles => Battles1.Keys.Concat(AutoBattles.Keys).Concat(Battles2.Keys).Where(w => !BattleEntryOffsets.Contains(w));
		private IEnumerable<BattleEntry> InvalidBattles => Battles1.Values.Concat(Battles2.Values).Where(w => w.Id == 0xFFFF);
		private IEnumerable<AutoBattleEntry> InvalidAutoBattles => AutoBattles.Values.Where(w => w.Id == 0xFFFF);

		public async Task Parse(string filePath)
		{
			Data = await File.ReadAllBytesAsync(filePath);
			if (Data.Length > UInt16.MaxValue) throw new InvalidDataException("file too big!");
			Coverage = new bool[Data.Length];
			UInt16 curOffset = 0;
			BattleEntryTableOffset = ReadUInt16(ref curOffset);
			for (int i = 0; i < 15; i++)
			{
				UInt16 off = curOffset;
				Bonuses entry = new(ref curOffset, this);
				BonusesTable.Add(off, entry);
			}
			for (int i = 0; i < 188; i++)
			{
				UInt16 off = curOffset;
				ModelEntry entry = new(ref curOffset, this);
				ModelTable.Add(off, entry);
			}
			for (int i = 0; i < 84; i++)
			{
				UInt16 off = curOffset;
				Placement[] placements = new Placement[8];
				for (int j = 0; j < 8; j++)
				{
					Placement entry = new(ref curOffset, this);
					placements[j] = entry;
				}
				PlacementTable.Add(off, placements);
			}
			Debug.Assert(curOffset == BattleEntryTableOffset);
			for (int i = 0; i < 376; i++)
			{
				UInt16 offset = ReadUInt16(ref curOffset);
				BattleEntryOffsets.Add(offset);
			}
			BattleEntry curBTEntry;
			do
			{
				UInt16 off = curOffset;
				BattleEntry entry = new(ref curOffset, this, true);
				Battles1.Add(off, entry);
				curBTEntry = entry;
			} while (curBTEntry.Id != 0xFFFF);
			AutoBattleEntry curAutoBTEntry;
			do
			{
				UInt16 off = curOffset;
				AutoBattleEntry entry = new(ref curOffset, this);
				AutoBattles.Add(off, entry);
				curAutoBTEntry = entry;
			} while (curAutoBTEntry.Id != 0xFFFF);
			for (int i = 0; i < 363; i++)
			{
				UInt16 off = curOffset;
				BattleEntry entry = new(ref curOffset, this, false);
				Battles2.Add(off, entry);
			}
			Debug.Assert(IsFullCoverage);
		}

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
			for(int i = 0; i <= length; i++)
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
