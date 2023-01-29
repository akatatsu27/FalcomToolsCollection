using Newtonsoft.Json;

namespace BTSET_Converter;

internal class FC_BTLSETJson : BTLSETJson
{
	public override Dictionary<string, Bonuses> BonusesTable { get; set; } = new();
	public override Dictionary<string, ModelEntry> ModelTable { get; set; } = new();
	public override Dictionary<string, Placement[]> PlacementTable { get; set; } = new();
	public Dictionary<string, FC_BattleEntry> Battles = new();
	public override Dictionary<string, AutoBattleEntry> AutoBattles { get; set; } = new();
	[JsonIgnore] private UInt16 BattleEntryOffset_Offset; //The offset of the offset table
	[JsonIgnore] private UInt16 AutoBattleEntryOffset_Offset; //The offset of the offset table
	private FC_BTLSETJson() { }
	public FC_BTLSETJson(FC_BTLSET file)
	{
		string name;
		string str = "Bonuses";
		uint count = 0;
		foreach (var bonuses in file.BonusesTable.Values)
		{
			name = $"{str}{count++}";
			BonusesTable[name] = bonuses;
			OffsetToNameMap[bonuses.Offset] = name;
		}
		foreach (var model in file.ModelTable)
		{
			ModelTable[$"{model.Value.Filename}"] = model.Value;
		}
		str = "Placements";
		count = 0;
		foreach (var placements in file.PlacementTable.Values)
		{
			name = $"{str}{count++}";
			PlacementTable[name] = placements;
			OffsetToNameMap[placements[0].Offset] = name;
		}
		str = "Battle";
		count = 0;
		foreach (var battle in file.Battles.Values)
		{
			battle.BonusesName = OffsetToNameMap[(UInt16)battle.BonusesOffset];
			battle.PlacementsNames = OffsetToNameMap[battle.PlacementTableOffset];
			battle.SurprisePlacementsNames = OffsetToNameMap[battle.SurpriseTableOffset];

			name = $"{str}{count++}";
			Battles[name] = battle;
		}
		str = "AutoBattle";
		count = 0;
		foreach (var battle in file.AutoBattles)
		{
			name = $"{str}{count++}";
			AutoBattles[name] = battle.Value;
		}
	}

	internal static async Task<FC_BTLSETJson> Deserialize(string dirPath)
	{
		string bonuses = Program.Bonuses;
		string models = Program.Models;
		string placements = Program.Placements;
		string battles = Program.Battles;
		string autobattles = Program.AutoBattles;

		FC_BTLSETJson json = new();
		json.BonusesTable = JsonConvert.DeserializeObject<Dictionary<string, FC_SC_Bonuses>>(await File.ReadAllTextAsync(Path.Combine(dirPath, bonuses)))?.ToDictionary(w => w.Key, w => w.Value as Bonuses) ?? throw new FileNotFoundException(bonuses);
		json.ModelTable = JsonConvert.DeserializeObject<Dictionary<string, ModelEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, models))) ?? throw new FileNotFoundException(models);
		json.PlacementTable = JsonConvert.DeserializeObject<Dictionary<string, Placement[]>>(await File.ReadAllTextAsync(Path.Combine(dirPath, placements))) ?? throw new FileNotFoundException(placements);
		json.Battles = JsonConvert.DeserializeObject<Dictionary<string, FC_BattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, battles))) ?? throw new FileNotFoundException(battles);
		json.AutoBattles = JsonConvert.DeserializeObject<Dictionary<string, AutoBattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, autobattles))) ?? throw new FileNotFoundException(autobattles);

		foreach (var placementTable in json.PlacementTable)
		{
			if (placementTable.Value.Length != 8) throw new InvalidDataException($"Error in {placementTable.Key}: Each placement table must have exactly 8 entries!");
		}
		foreach (var model in json.ModelTable)
		{
			model.Value.Filename = model.Key;
		}
		foreach (var battle in json.Battles)
		{
				if (battle.Value.Enemies.Length != 8)
				throw new InvalidDataException($"Error in {battle.Key}: Each battle variation must have exactly 8 enemy entries, even if empty!");
				foreach (EnemyEntry enemy in battle.Value.Enemies)
				{
					enemy.DataTable = UInt16.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
				}
		}
		foreach (var battle in json.AutoBattles)
		{
			if (battle.Value.Side1.Length != 8 || battle.Value.Side2.Length != 8)
				throw new InvalidDataException($"Error in {battle.Key}: Each autobattle side must have exactly 8 enemy entries, even if empty!");
			foreach (EnemyEntry enemy in battle.Value.Side1.Concat(battle.Value.Side2))
			{
				enemy.DataTable = UInt16.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
			}
		}

		return json;
	}

	private void CalculateOffsets()
	{
		//Skipping first 2 UInt16 which are the BattleEntryTableOffset and the AutoBattleEntryTableOffset
		UInt16 curOffset = 4;
#if !DEBUG
        //Debug header (#bonuses + #models + #placements + #battles)
        curOffset += 2 + 2 + 2 + 2;
#endif
		BattleEntryOffset_Offset = curOffset;
		CalculateBattleOffsetsPart1(ref curOffset);

		AutoBattleEntryOffset_Offset = curOffset;
		CalculateAutoBattleOffsetsPart1(ref curOffset);

		CalculatePlacementTableOffsets(ref curOffset);
		CalculateModelTableOffsets(ref curOffset);
		CalculateBonusesOffsets(ref curOffset);

		CalculateBattleOffsetsPart2(ref curOffset);
		CalculateAutoBattleOffsetsPart2(ref curOffset);

		if (curOffset > UInt16.MaxValue) throw new InvalidDataException("Compiled file too big!");
	}

	internal byte[] Compile()
	{
		CalculateOffsets();
		using var ms = new MemoryStream(0xF8DA);
		using var bw = new BinaryWriter(ms);

		bw.Write(BattleEntryOffset_Offset);
		bw.Write(AutoBattleEntryOffset_Offset);
#if !DEBUG
		//Debug Header
		bw.Write((UInt16)BonusesTable.Count);
		bw.Write((UInt16)ModelTable.Count);
		bw.Write((UInt16)PlacementTable.Count);
		bw.Write((UInt16)(Battles.Count + AutoBattles.Count));
#endif
		foreach (var battle in Battles.Values)
		{
			bw.Write(battle.Offset);
		}
		foreach (var battle in AutoBattles.Values)
		{
			bw.Write(battle.Offset);
		}
		foreach (var placements in PlacementTable.Values)
		{
			foreach (Placement plac in placements)
			{
				bw.Write(plac.ToByteArray());
			}
		}
		foreach (var model in ModelTable.Values)
		{
			bw.Write(model.ToByteArray());
		}
		foreach (var bonuses in BonusesTable.Values)
		{
			bw.Write(bonuses.ToByteArray());
		}
		foreach (var battle in Battles.Values)
		{
			bw.Write(battle.ToByteArray());
		}
		foreach (var battle in AutoBattles.Values)
		{
			bw.Write(battle.ToByteArray());
		}

		return ms.ToArray();
	}

	private void CalculateBattleOffsetsPart1(ref UInt16 curOffset)
	{
		foreach (var battle in Battles)
		{
			curOffset += 2;
		}
	}
	private void CalculateBattleOffsetsPart2(ref UInt16 curOffset)
	{
		foreach (var battle in Battles)
		{
			string name = battle.Key;
			FC_BattleEntry entry = battle.Value;
			entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];
			battle.Value.BonusesOffset = NameToOffsetMap[battle.Value.BonusesName];
			battle.Value.PlacementTableOffset = NameToOffsetMap[battle.Value.PlacementsNames];
			battle.Value.SurpriseTableOffset = NameToOffsetMap[battle.Value.SurprisePlacementsNames];
			entry.Offset = curOffset;

			curOffset += 28 + 8 * 4; // sizeof(FC_BattleEntry)
		}
	}
}
