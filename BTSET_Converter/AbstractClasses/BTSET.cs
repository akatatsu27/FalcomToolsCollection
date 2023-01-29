using Shared;

namespace BTSET_Converter;

internal abstract class BTSET : BTLSET
{
	internal virtual Dictionary<UInt16, BattleEntry> Type1Battles { get; set; }

	//Unfortunately the below information isn't encoded anywhere
	protected const ushort SC_BONUSES_NUM = 12;
	protected const ushort SC_MODELS_NUM = 122;
	protected const ushort SC_PLACEMENTS_NUM = 56;

	protected const ushort THIRD_BONUSES_NUM = 15;
	protected const ushort THIRD_MODELS_NUM = 188;
	protected const ushort THIRD_PLACEMENTS_NUM = 84;
}
