using System.Xml.Linq;
using Newtonsoft.Json;

namespace BTSET_Converter;

internal abstract class BTSETJson
{
	public virtual Dictionary<string, Bonuses> BonusesTable { get; set; }
	public virtual Dictionary<string, ModelEntry> ModelTable { get; set; }
	public virtual Dictionary<string, Placement[]> PlacementTable { get; set; }
	public virtual Dictionary<string, BattleEntry> Type1Battles { get; set; }
	public virtual Dictionary<string, AutoBattleEntry> AutoBattles { get; set; }

	//For Serialization
	[JsonIgnore] protected readonly Dictionary<UInt16, string> OffsetToNameMap = new();
	//For Deserialization
	[JsonIgnore] protected readonly Dictionary<string, UInt16> NameToOffsetMap = new();
	protected void CalculateBonusesOffsets(ref UInt16 curOffset)
	{
		foreach (var bonuses in BonusesTable)
		{
			string name = bonuses.Key;
			Bonuses entry = bonuses.Value;
			entry.Offset = curOffset;
			OffsetToNameMap[curOffset] = name;
			NameToOffsetMap[name] = curOffset;
			if (BonusesTable.Values.First().GetType() == typeof(THIRD_Bonuses))
				curOffset += 18; //sizeof(THIRD_Bonuses)
			else
				curOffset += 16; //sizeof(FC_SC_Bonuses)
		}
	}
	protected void CalculateModelTableOffsets(ref UInt16 curOffset)
	{
		foreach (var model in ModelTable)
		{
			string name = model.Key;
			ModelEntry entry = model.Value;
			entry.Offset = curOffset;
			entry.NameOffset = (UInt16)(curOffset + 4); // sizeof(unk00) + sizeof(NameOffset)
			OffsetToNameMap[curOffset] = name;
			NameToOffsetMap[name] = curOffset;
			curOffset += (UInt16)(4 + name.Length + 1); // sizeof(unk00) + sizeof(NameOffset) + name.Length + null byte
		}
#if DEBUG && SC
		// other\BC5306.x appears a second time at the end for some unfathomable reason
		string nameExtra = ModelTable["other\\BC5306.x"].Filename;
		curOffset += (UInt16)(4 + nameExtra.Length + 1); // sizeof(unk00) + sizeof(NameOffset) + name.Length + null byte
#endif
	}
	protected void CalculatePlacementTableOffsets(ref UInt16 curOffset)
	{
		foreach (var placements in PlacementTable)
		{
			string name = placements.Key;
			Placement[] entry = placements.Value;
			entry[0].Offset = curOffset;
			OffsetToNameMap[curOffset] = name;
			NameToOffsetMap[name] = curOffset;
			curOffset += (1 + 1 + 2) * 8; // (sizeof(X) + sizeof(Y) + sizeof(Rot)) * 8 placements
		}
	}
	protected void CalculateType1BattleOffsetsPart1(ref UInt16 curOffset)
	{
		foreach (var battle in Type1Battles)
		{
			string name = battle.Key;
			BattleEntry entry = battle.Value;
			entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];
			foreach (var variation in entry.Variations)
			{
				if (variation == null) continue;
				variation.BonusesOffset = NameToOffsetMap[variation.BonusesName];
				variation.PlacementTableOffset = NameToOffsetMap[variation.PlacementsNames];
				variation.SurpriseTableOffset = NameToOffsetMap[variation.SurprisePlacementsNames];
			}
#if DEBUG && THIRD
			if (GetType() == typeof(BTSET1Json))
			{
				//For some reason these battles are not present in the offset table
				if (battle.Value.Id == 3623 || battle.Value.Id == 529 || battle.Value.Id == 530) continue;
			}
#endif
			curOffset += 2;
		}
#if DEBUG && SC
		// offset 0x15E3 appears twice in a row in SC's BTSET2 for some unfathomable reason
		if(GetType() == typeof(BTSET2Json))
			curOffset += 2;
#endif
	}
	protected void CalculateAutoBattleOffsetsPart1(ref UInt16 curOffset)
	{
		foreach (var battle in AutoBattles)
		{
			string name = battle.Key;
			AutoBattleEntry entry = battle.Value;
			entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];
#if DEBUG && THIRD
			if (GetType() == typeof(BTSET1Json))
			{
				//For some reason these battles are not present in the offset table
				if (battle.Value.Id == 3623 || battle.Value.Id == 529 || battle.Value.Id == 530) continue;
			}
#endif
			curOffset += 2;
		}
	}
	protected void CalculateType1BattleOffsetsPart2(ref UInt16 curOffset)
	{
		foreach (var battle in Type1Battles)
		{
			string name = battle.Key;
			BattleEntry entry = battle.Value;
			entry.Offset = curOffset;
			curOffset += 16;
			foreach (var variation in entry.Variations)
			{
				if (variation == null) continue;
				curOffset += 48;
			}

		}
	}
	protected void CalculateAutoBattleOffsetsPart2(ref UInt16 curOffset)
	{
		foreach (var battle in AutoBattles)
		{
			string name = battle.Key;
			AutoBattleEntry entry = battle.Value;
			entry.Offset = curOffset;

			curOffset += 76; // sizeof(AutoBattleEntry)
		}
	}
}
