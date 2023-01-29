using Newtonsoft.Json;

namespace BTSET_Converter;

internal abstract class BTSETJson : BTLSETJson
{
	public abstract Dictionary<string, BattleEntry> Type1Battles { get; set; }
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
}
