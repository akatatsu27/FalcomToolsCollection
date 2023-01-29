using System.Diagnostics;

namespace BTSET_Converter;

internal class FC_BTLSET : BTLSET
{
	// This tool is meant for analysis, so it's also keeping track of who is accessing fields.
	internal UInt16 BattleEntryTableOffset;
	internal UInt16 AutoBattleEntryTableOffset;
	private List<UInt16> BattleEntryOffsets = new();
	private List<UInt16> AutoBattleEntryOffsets = new();
	internal override Dictionary<UInt16, Placement[]> PlacementTable { get; set; } = new();
	internal override Dictionary<UInt16, ModelEntry> ModelTable { get; set; } = new();
	internal override Dictionary<UInt16, Bonuses> BonusesTable { get; set; } = new();
	internal Dictionary<UInt16, FC_BattleEntry> Battles = new();
	internal override Dictionary<UInt16, AutoBattleEntry> AutoBattles { get; set; } = new();

	private Placement[] RoguePlacements1;
	private Placement[] RoguePlacements2;

	//Unfortunately this information isn't encoded anywhere
	private const ushort FC_BT_OFFSETS_NUM = 974;
	private const ushort FC_AUTO_BT_OFFSETS_NUM = 8;
	private const ushort FC_PLACEMENTS_IN_TABLE_NUM = 36; // + 2 rogue entries placed in-between battles -_-
	private const ushort FC_MODELS_NUM = 67;
	private const ushort FC_BONUSES_NUM = 11;

	private const ushort BT_ENTRIES_PART1 = 951;
	private const ushort BT_ENTRIES_PART2 = 17;
	private const ushort BT_ENTRIES_PART3 = 6; //repeat till Id == 0xFFFF

	public async Task Parse(string filePath)
	{
		Data = await File.ReadAllBytesAsync(filePath);
		if (Data.Length > UInt16.MaxValue) throw new InvalidDataException("file too big!");
		Coverage = new bool[Data.Length];

		UInt16 curOffset = 0;
		BattleEntryTableOffset = ReadUInt16(ref curOffset);
		AutoBattleEntryTableOffset = ReadUInt16(ref curOffset);
		Debug.Assert(curOffset == BattleEntryTableOffset);
		for (int i = 0; i < FC_BT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			BattleEntryOffsets.Add(offset);
		}
		Debug.Assert(curOffset == AutoBattleEntryTableOffset);
		for (int i = 0; i < FC_AUTO_BT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			AutoBattleEntryOffsets.Add(offset);
		}
		for (int i = 0; i < FC_PLACEMENTS_IN_TABLE_NUM; i++)
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
		for (int i = 0; i < FC_MODELS_NUM; i++)
		{
			UInt16 off = curOffset;
			ModelEntry entry = new(ref curOffset, this);
			ModelTable.Add(off, entry);
		}
		for (int i = 0; i < FC_BONUSES_NUM; i++)
		{
			UInt16 off = curOffset;
			FC_SC_Bonuses entry = new(ref curOffset, this);
			BonusesTable.Add(off, entry);
		}
		for (int i = 0; i < BT_ENTRIES_PART1; i++)
		{
			UInt16 off = curOffset;
			FC_BattleEntry entry = new(ref curOffset, this);
			Battles.Add(off, entry);
		}
		UInt16 rogueOff1 = curOffset;
		Placement[] roguePlacements1 = new Placement[8];
		for (int j = 0; j < 8; j++)
		{
			Placement entry = new(ref curOffset, this);
			roguePlacements1[j] = entry;
		}
		PlacementTable.Add(rogueOff1, roguePlacements1);
		RoguePlacements1 = roguePlacements1;
		for (int i = 0; i < BT_ENTRIES_PART2; i++)
		{
			UInt16 off = curOffset;
			FC_BattleEntry entry = new(ref curOffset, this);
			Battles.Add(off, entry);
		}
		UInt16 rogueOff2 = curOffset;
		Placement[] roguePlacements2 = new Placement[8];
		for (int j = 0; j < 8; j++)
		{
			Placement entry = new(ref curOffset, this);
			roguePlacements2[j] = entry;
		}
		PlacementTable.Add(rogueOff2, roguePlacements2);
		RoguePlacements2 = roguePlacements2;
		FC_BattleEntry curBTEntry;
		do
		{
			UInt16 off = curOffset;
			FC_BattleEntry entry = new(ref curOffset, this);
			Battles.Add(off, entry);
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
		Debug.Assert(IsFullCoverage);
	}	
}
