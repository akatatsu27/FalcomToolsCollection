using System.Diagnostics;
using System.Security.Cryptography;

namespace BTSET_Converter;

internal class BTSET1 : BTSET
{
	// This tool is also meant for analysis, so it's also keeping track of who is accessing fields.
	internal UInt16 BattleEntryTableOffset;
	internal override Dictionary<UInt16, Bonuses> BonusesTable { get; set; } = new();
	internal override Dictionary<UInt16, ModelEntry> ModelTable { get; set; } = new();
	internal override Dictionary<UInt16, Placement[]> PlacementTable { get; set; } = new();
	private List<UInt16> BattleEntryOffsets = new();
	internal override Dictionary<UInt16, BattleEntry> Type1Battles { get; set; } = new();
	internal override Dictionary<UInt16, AutoBattleEntry> AutoBattles { get; set; } = new();
	internal readonly Dictionary<UInt16, BattleEntry> Type2Battles = new();
	private IEnumerable<UInt16> UnusedBattles => Type1Battles.Keys.Concat(AutoBattles.Keys).Concat(Type2Battles.Keys).Where(w => !BattleEntryOffsets.Contains(w));
	private IEnumerable<BattleEntry> InvalidBattles => Type1Battles.Values.Concat(Type2Battles.Values).Where(w => w.Id == 0xFFFF);
	private IEnumerable<AutoBattleEntry> InvalidAutoBattles => AutoBattles.Values.Where(w => w.Id == 0xFFFF);

	//Unfortunately the below information isn't encoded anywhere
	private const ushort SC_BT_OFFSETS_NUM = 504; // this is remarkably correct. good job falcom!
	private const ushort SC_TYPE2_BT_ENTRIES_NUM = 1; 

	private const ushort THIRD_BT_OFFSETS_NUM = 376; // 3 battles are actually missing from the offset table
	private const ushort THIRD_TYPE2_BT_ENTRIES_NUM = 363;

	public async Task Parse(string filePath)
	{
		byte[] hash;
		using (var md5 = MD5.Create())
		{
			using (var stream = File.OpenRead(filePath))
			{
				hash = md5.ComputeHash(stream);
			}
		}
		Data = await File.ReadAllBytesAsync(filePath);
		if (Data.Length > UInt16.MaxValue) throw new InvalidDataException("file too big!");
		Coverage = new bool[Data.Length];

		if (Convert.ToHexString(hash) == "26D79AFD5B5F48ABF19772B62EF4F5E3") //hack to distinguish between SC and THIRD
		{
			ParseTHIRD();
		}
		else if (Convert.ToHexString(hash) == "FE30A025EE2B6AA464B09F603089C922")
		{
			ParseSC();
		}
		else
		{
			Console.WriteLine("Please provide the original T_BTSET1._DT file");
		}
	}

	private void ParseTHIRD()
	{
		UInt16 curOffset = 0;
		BattleEntryTableOffset = ReadUInt16(ref curOffset);
		for (int i = 0; i < THIRD_BONUSES_NUM; i++)
		{
			UInt16 off = curOffset;
			THIRD_Bonuses entry = new(ref curOffset, this);
			BonusesTable.Add(off, entry);
		}
		for (int i = 0; i < THIRD_MODELS_NUM; i++)
		{
			UInt16 off = curOffset;
			ModelEntry entry = new(ref curOffset, this);
			ModelTable.Add(off, entry);
		}
		for (int i = 0; i < THIRD_PLACEMENTS_NUM; i++)
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
		for (int i = 0; i < THIRD_BT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			BattleEntryOffsets.Add(offset);
		}
		BattleEntry curBTEntry;
		do
		{
			UInt16 off = curOffset;
			BattleEntry entry = new(ref curOffset, this, true);
			Type1Battles.Add(off, entry);
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
		for (int i = 0; i < THIRD_TYPE2_BT_ENTRIES_NUM; i++)
		{
			UInt16 off = curOffset;
			BattleEntry entry = new(ref curOffset, this, false);
			Type2Battles.Add(off, entry);
		}
		Debug.Assert(IsFullCoverage);
	}
	private void ParseSC()
	{
		UInt16 curOffset = 0;
		BattleEntryTableOffset = ReadUInt16(ref curOffset);
		for (int i = 0; i < SC_BONUSES_NUM; i++)
		{
			UInt16 off = curOffset;
			FC_SC_Bonuses entry = new(ref curOffset, this);
			BonusesTable.Add(off, entry);
		}
		for (int i = 0; i < SC_MODELS_NUM; i++)
		{
			UInt16 off = curOffset;
			ModelEntry entry = new(ref curOffset, this);
			ModelTable.Add(off, entry);
		}
		for (int i = 0; i < SC_PLACEMENTS_NUM; i++)
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
		for (int i = 0; i < SC_BT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			BattleEntryOffsets.Add(offset);
		}
		BattleEntry curBTEntry;
		do
		{
			UInt16 off = curOffset;
			BattleEntry entry = new(ref curOffset, this, true, true);
			Type1Battles.Add(off, entry);
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
		for (int i = 0; i < SC_TYPE2_BT_ENTRIES_NUM; i++)
		{
			UInt16 off = curOffset;
			BattleEntry entry = new(ref curOffset, this, false);
			Type2Battles.Add(off, entry);
		}
		Debug.Assert(IsFullCoverage);
	}
}
