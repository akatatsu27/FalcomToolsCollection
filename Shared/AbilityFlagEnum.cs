using Newtonsoft.Json;

namespace Shared;

[Flags]
[JsonConverter(typeof(BitFlagsToArrayConverter<AbilityFlagEnum>))]
public enum AbilityFlagEnum : ushort
{
	NONE = 0,
	/// <summary>
	/// Tear and Curia, but not Thelas or crafts
	/// </summary>
	HEAL =		0b0000_0000_0000_0001,
	_02 =		0b0000_0000_0000_0010,
	_04 =		0b0000_0000_0000_0100,
	/// <summary>
	/// unused?
	/// </summary>
	_08 = 0b0000_0000_0000_1000,
	HITS_ENEMY = 0b0000_0000_0001_0000,
	/// <summary>
	/// Thelas
	/// </summary>
	HITS_DEAD = 0b0000_0000_0010_0000,
	/// <summary>
	/// Maybe affects whether the target does damaged animation?
	/// </summary>
	BENEFICIAL= 0b0000_0000_0100_0000,
	HITS_ALLY = 0b0000_0000_1000_0000,
	_0100 =		0b0000_0001_0000_0000,
	/// <summary>
	/// unused?
	/// </summary>
	_0200 =		0b0000_0010_0000_0000,
	/// <summary>
	/// Used on some of Tita's skills, perhaps to grab the aoe from the weapon
	/// </summary>
	CANNON =	0b0000_0100_0000_0000,
	/// <summary>
	/// unused?
	/// </summary>
	_0800 =		0b0000_1000_0000_0000,
	_1000 =		0b0001_0000_0000_0000,
	/// <summary>
	/// Used on all arts; might affect whether it uses atk/def or ats/adf?
	/// </summary>
	MAGIC =		0b0010_0000_0000_0000,
	_4000 =		0b0100_0000_0000_0000,
	/// <summary>
	/// unused?
	/// </summary>
	_8000 =		0b1000_0000_0000_0000,
}
