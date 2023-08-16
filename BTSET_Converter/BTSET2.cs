using System.Diagnostics;
using System.Security.Cryptography;

namespace BTSET_Converter;

internal class BTSET2 : BTSET
{
    // This tool is meant for analysis, so it's also keeping track of who is accessing fields.
    internal UInt16 BattleEntryTableOffset;
    internal UInt16 AutoBattleEntryTableOffset;
    internal override Dictionary<UInt16, Bonuses> BonusesTable { get; set; } = new();
    internal override Dictionary<UInt16, ModelEntry> ModelTable { get; set; } = new();
    internal override Dictionary<UInt16, Placement[]> PlacementTable { get; set; } = new();
    private List<UInt16> BattleEntryOffsets = new();
    private List<UInt16> AutoBattleEntryOffsets = new();
    internal override Dictionary<UInt16, BattleEntry> Type1Battles { get; set; } = new();
	internal override Dictionary<UInt16, AutoBattleEntry> AutoBattles { get; set; } = new();

	//Unfortunately the below information isn't encoded anywhere
	private const ushort SC_BT_OFFSETS_NUM = 29;
	private const ushort SC_AUTOBT_OFFSETS_NUM = 6;

	private const ushort THIRD_BT_OFFSETS_NUM = 95;
	private const ushort THIRD_AUTOBT_OFFSETS_NUM = 2;
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

		Bytes = await File.ReadAllBytesAsync(filePath);
        if (Bytes.Length > UInt16.MaxValue) throw new InvalidDataException("file too big!");
        Coverage = new bool[Bytes.Length];

		if (Convert.ToHexString(hash) == "B7423D80515F66413D10E0F4BF3BF7F9") //hack to distinguish between SC and THIRD
		{
			ParseTHIRD();
		}
		else if (Convert.ToHexString(hash) == "0B4BA88E977BBE9D696BEEC4D3AD7FEB")
		{
			ParseSC();
		}
		else
		{
			Console.WriteLine("Please provide the original T_BTSET2._DT file");
		}
	}

    private void ParseTHIRD()
    {
		UInt16 curOffset = 0;
		BattleEntryTableOffset = ReadUInt16(ref curOffset);
		AutoBattleEntryTableOffset = ReadUInt16(ref curOffset);
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
		Debug.Assert(curOffset == AutoBattleEntryTableOffset);
		for (int i = 0; i < THIRD_AUTOBT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			AutoBattleEntryOffsets.Add(offset);
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

		Debug.Assert(IsFullCoverage);
	}

	private void ParseSC()
	{
		UInt16 curOffset = 0;
		BattleEntryTableOffset = ReadUInt16(ref curOffset);
		AutoBattleEntryTableOffset = ReadUInt16(ref curOffset);
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
		Debug.Assert(curOffset == AutoBattleEntryTableOffset);
		for (int i = 0; i < SC_AUTOBT_OFFSETS_NUM; i++)
		{
			UInt16 offset = ReadUInt16(ref curOffset);
			AutoBattleEntryOffsets.Add(offset);
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

		Debug.Assert(IsFullCoverage);
	}
}
