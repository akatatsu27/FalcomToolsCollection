namespace Shared;

public enum AbilityTarget : ushort
{
	NONE = 0,
	WALK_TARGET = 1,
	WALK_TARGET_AREA = 2,
	WALK_TARGET_LINE = 3,
	TARGET = 4,
	TARGET_AREA = 5,
	COMBO = 6,
	SET_AREA = 11,
	SET_LINE = 12,
	ALL = 13,
	SELF_AREA = 14,
	WALK_SET_AREA = 15,
	/// <summary>
	/// Ragnard
	/// </summary>
	_16 = 16,
	_17 = 17,
	FLOOR_TILE = 18,
	TRANSFORM = 19,
	WALK = 50,
	_100 = 100,
}
