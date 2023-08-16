using Shared;

namespace BTSET_Converter;

internal abstract class BTLSET : FileClass
{
	internal abstract Dictionary<UInt16, Bonuses> BonusesTable { get; set; }
	internal abstract Dictionary<UInt16, ModelEntry> ModelTable { get; set; }
	internal abstract Dictionary<UInt16, Placement[]> PlacementTable { get; set; }
	internal abstract Dictionary<UInt16, AutoBattleEntry> AutoBattles { get; set; }

	internal ModelEntry ReadBattlefield(UInt16 offset, Battle battleEntry)
		=> ModelTable[offset].AddReference(battleEntry);
	internal ModelEntry ReadBattlefield(UInt16 offset, FC_BattleEntry battleEntry)
		=> ModelTable[offset].AddReference(battleEntry);

	internal Bonuses ReadBonuses(UInt16 offset, Variation variation)
		=> BonusesTable[offset].AddReference(variation);

	internal Placement[] ReadPlacement(ref UInt16 offset, Variation variation)
		=> PlacementTable[offset];
}
