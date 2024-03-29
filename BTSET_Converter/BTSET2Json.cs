﻿using System.Text;
using Newtonsoft.Json;

namespace BTSET_Converter;

internal class BTSET2Json : BTSETJson
{
    public override Dictionary<string, Bonuses> BonusesTable { get; set; } = new();
    public override Dictionary<string, ModelEntry> ModelTable { get; set; } = new();
    public override Dictionary<string, Placement[]> PlacementTable { get; set; } = new();
    public override Dictionary<string, BattleEntry> Type1Battles { get; set; } = new();
    public override Dictionary<string, AutoBattleEntry> AutoBattles { get; set; } = new();

    [JsonIgnore] private UInt16 BattleEntryOffset_Offset; //The offset of the offset table
    [JsonIgnore] private UInt16 AutoBattleEntryOffset_Offset; //The offset of the offset table

    private BTSET2Json() { }
    public BTSET2Json(BTSET2 file)
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
        str = "Type1Battle";
        count = 0;
        foreach (var battle in file.Type1Battles.Values)
        {
            foreach (var variation in battle.Variations)
            {
                if (variation == null) continue;
                variation.BonusesName = OffsetToNameMap[(UInt16)variation.BonusesOffset];
                variation.PlacementsNames = OffsetToNameMap[variation.PlacementTableOffset];
                variation.SurprisePlacementsNames = OffsetToNameMap[variation.SurpriseTableOffset];
            }
            name = $"{str}{count++}";
            Type1Battles[name] = battle;
        }
        str = "AutoBattle";
        count = 0;
        foreach (var battle in file.AutoBattles)
        {
            name = $"{str}{count++}";
            AutoBattles[name] = battle.Value;
        }
    }

    internal static async Task<BTSET2Json> Deserialize(string dirPath)
    {
        string bonuses = Program.Bonuses;
        string models = Program.Models;
        string placements = Program.Placements;
        string battles1 = Program.Type1Battles;
        string autobattles = Program.AutoBattles;

        BTSET2Json json = new();
		//hack to avoid reflection magic
		string bonusesTable = await File.ReadAllTextAsync(Path.Combine(dirPath, bonuses));
		if (bonusesTable.Contains("Unk11"))
		{
			//THIRD Bonuses
			json.BonusesTable = JsonConvert.DeserializeObject<Dictionary<string, THIRD_Bonuses>>(bonusesTable)?.ToDictionary(w => w.Key, w => w.Value as Bonuses) ?? throw new FileNotFoundException(bonuses);
		}
		else
		{
			//FC/SC Bonuses
			json.BonusesTable = JsonConvert.DeserializeObject<Dictionary<string, FC_SC_Bonuses>>(bonusesTable)?.ToDictionary(w => w.Key, w => w.Value as Bonuses) ?? throw new FileNotFoundException(bonuses);
		}
		//
		json.ModelTable = JsonConvert.DeserializeObject<Dictionary<string, ModelEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, models))) ?? throw new FileNotFoundException(models);
        json.PlacementTable = JsonConvert.DeserializeObject<Dictionary<string, Placement[]>>(await File.ReadAllTextAsync(Path.Combine(dirPath, placements))) ?? throw new FileNotFoundException(placements);
        json.Type1Battles = JsonConvert.DeserializeObject<Dictionary<string, BattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, battles1))) ?? throw new FileNotFoundException(battles1);
        json.AutoBattles = JsonConvert.DeserializeObject<Dictionary<string, AutoBattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, autobattles))) ?? throw new FileNotFoundException(autobattles);

        foreach (var placementTable in json.PlacementTable)
        {
            if (placementTable.Value.Length != 8) throw new InvalidDataException($"Error in {placementTable.Key}: Each placement table must have exactly 8 entries!");
        }
        foreach (var model in json.ModelTable)
        {
            model.Value.Filename = model.Key;
        }
        foreach (var battle in json.Type1Battles)
        {
            battle.Value.VariationWeights = battle.Value.JsonBytes.Select(w => (byte)w).ToArray();
            if (battle.Value.VariationWeights.Length != 4) throw new InvalidDataException($"Error in {battle.Key}: Each battle must have exactly 4 variation weights");
            if (battle.Value.Variations.Length > 4) throw new InvalidDataException($"Error in {battle.Key}: Each battle must have at most 4 variations");
            foreach (var variation in battle.Value.Variations)
            {
                if (variation == null) continue;
                if (variation.Enemies.Length != 8) throw new InvalidDataException($"Error in {battle.Key}: Each battle variation must have exactly 8 enemy entries, even if empty!");
                foreach (EnemyEntry enemy in variation.Enemies)
                {
                    enemy.DataTable = UInt16.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
                }
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
        CalculateBonusesOffsets(ref curOffset);
		CalculateModelTableOffsets(ref curOffset);
		CalculatePlacementTableOffsets(ref curOffset);

		BattleEntryOffset_Offset = curOffset;
        CalculateType1BattleOffsetsPart1(ref curOffset);

        AutoBattleEntryOffset_Offset = curOffset;
        CalculateAutoBattleOffsetsPart1(ref curOffset);

		CalculateType1BattleOffsetsPart2(ref curOffset);
        CalculateAutoBattleOffsetsPart2(ref curOffset);

        if (curOffset > UInt16.MaxValue) throw new InvalidDataException("Compiled file too big!");
    }

    internal byte[] Compile()
    {
        CalculateOffsets();
        using var ms = new MemoryStream(0x32F0);
        using var bw = new BinaryWriter(ms);

        bw.Write(BattleEntryOffset_Offset);
        bw.Write(AutoBattleEntryOffset_Offset);
#if !DEBUG
        //Debug Header
        bw.Write((UInt16)BonusesTable.Count);
        bw.Write((UInt16)ModelTable.Count);
        bw.Write((UInt16)PlacementTable.Count);
        bw.Write((UInt16)(Type1Battles.Count + AutoBattles.Count));
#endif
        foreach (var bonuses in BonusesTable.Values)
        {
            bw.Write(bonuses.ToByteArray());
        }
        foreach (var model in ModelTable.Values)
        {
            bw.Write(model.ToByteArray());
        }
#if DEBUG && SC
		// other\BC5306.x appears a second time at the end for some unfathomable reason
		string nameExtra = ModelTable["other\\BC5306.x"].Filename;
        bw.Write(new byte[] { 0x00, 0x00 });
        bw.Write((UInt16)(ms.Position + 2));
        bw.Write(Encoding.ASCII.GetBytes(nameExtra).Append<byte>(0).ToArray());
#endif
		foreach (var placements in PlacementTable.Values)
        {
            foreach (Placement plac in placements)
            {
                bw.Write(plac.ToByteArray());
            }
        }

        foreach (var battle in Type1Battles.Values)
        {
            bw.Write(battle.Offset);
#if DEBUG && SC
            // offset 0x15E3 appears twice in a row for some unfathomable reason
            if (battle.Offset == 0x15E3)
            {
                bw.Write(battle.Offset);
            }
#endif
		}
		foreach (var battle in AutoBattles.Values)
        {
            bw.Write(battle.Offset);
        }

        foreach (var battle in Type1Battles.Values)
        {
            bw.Write(battle.ToByteArray());
        }
        foreach (var battle in AutoBattles.Values)
        {
            bw.Write(battle.ToByteArray());
        }

        return ms.ToArray();
    }
}
