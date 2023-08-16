namespace Shared;

public enum EffectEnum : byte
{
	NONE = 0,
	PHYS_DAMAGE = 1,
	MAG_DAMAGE = 2,
	HEAL = 3,
	/// <summary>
	/// Fortune Coin, args=50,100
	/// </summary>
	_4 = 4,
	/// <summary>
	/// Used on four unnamed skills in FC
	/// </summary>
	_5 = 5,
	IMPEDE = 6,
	DELAY = 7,
	HP_ABSORB = 8,
	EP_ABSORB = 9,
	POISON = 10,
	FREEZE = 11,
	PETRIFY = 12,
	SLEEP = 13,
	MUTE = 14,
	BLIND = 15,
	SEAL = 16,
	CONFUSE = 17,
	FAINT = 18,
	DEATH = 19,
	/// <summary>
	/// Used by Reverie, no idea what it does
	/// </summary>
	COOLING = 20,
	IMMUNITY = 21,
	BREAK_FLOOR = 22,
	BREAK_ITEM = 23,
	CP_ABSORB = 24,
	STR_UP = 25,
	STR_DN = 26,
	DEF_UP = 27,
	DEF_DN = 28,
	SPD_UP = 29,
	SPD_DN = 30,
	ADF_UP = 31,
	ADF_DN = 32,
	AGL_UP = 33,
	AGL_DN = 34,
	AT_ADVANCE = 35,
	MAX_HP_DN = 36,
	MOV_UP = 37,
	RAGE = 38,
	MOV_DN = 39,
	CURE = 40,
	RESURRECT = 41,
	/// <summary>
	/// Reverie's Anti-Skill Barrier
	/// </summary>
	ANTI_SKILL = 42,
	/// <summary>
	/// Reverie's Anti-Magic Barrier
	/// </summary>
	ANTI_MAGIC = 43,

	FATTEN   = 45,
	_46   = 46,
	SHRINK   = 47,
	HEAL_PERCENT   = 48,

	/// <summary>
	/// S-Tablet
	/// </summary>
	CURE_LOWERED_STATUS = 49,
	/// <summary>
	/// Herb Sandwich
	/// </summary>
	CURE_POISON = 50,
	/// <summary>
	/// Hot-Hot Potato Fry
	/// </summary>
	CURE_FREEZE = 51,  
	/// <summary>
	/// Corner Castelia
	/// </summary>
	CURE_PETRIFY = 52,
	/// <summary>
	/// Royal Gelato, Nap Killer
	/// </summary>
	CURE_SLEEP = 53,
	/// <summary>
	/// Insulating Tape
	/// </summary>
	CURE_MUTE = 54,
	/// <summary>
	/// Passion Omelet
	/// </summary>
	CURE_BLIND = 55,
	/// <summary>
	/// Miso-Stewed Fish
	/// </summary>
	CURE_SEAL = 56,
	/// <summary>
	/// Mocking Pie
	/// </summary>
	CURE_CONFUSE = 57,
	/// <summary>
	/// Sea 'Bubbles'
	/// </summary>
	CURE_FAINT = 58,
	/// <summary>
	/// Smelling Salts
	/// </summary>
	CURE_CONFUSE_SLEEP_FAINT = 59,
	/// <summary>
	/// Softening Balm
	/// </summary>
	CURE_FREEZE_PETRIFY = 60,
	/// <summary>
	/// Purging Balm
	/// </summary>
	CURE_POISON_SEAL_BLIND = 61,
	/// <summary>
	/// Sacrifice Arrow, Zeram Powder, Zeram Capsule (Does this include resurrect too?)
	/// </summary>
	CP_UP = 62,
	/// <summary>
	/// Royal Crepe
	/// </summary>
	CURE_POISON_CONFUSE = 63,
	/// <summary>
	/// Bone Boullion, Mighty Juice, Spiral Noodles, Mighty Essence
	/// </summary>
	STR_DEF_UP = 64,
	/// <summary>
	/// Kaempfer
	/// </summary>
	STR_DEF_DN = 65,

	/// <summary>
	/// Cure debuffs (49?)
	/// </summary>
	VITAL_CANNON = 68,
	/// <summary>
	/// Impede crafts (Those don't take time though)
	/// </summary>
	SILENT_CROSS = 69,
	/// <summary>
	/// Revive/heal/def up if 200. Four parameters.
	/// </summary>
	LICHTKREIS = 70,
	/// <summary>
	/// Random status
	/// </summary>
	JUDGEMENT_CARD = 71,
	/// <summary>
	/// Premium Herb Tea, Flame Tongue Stew, Seafood Jelly, Mystery Crepe
	/// </summary>
	CURE_SEAL_MUTE_CONFUSE = 72,
	/// <summary>
	/// Anarchy Soup, Crimson Platter, Roast Fish
	/// </summary>
	CURE_FAINT_SLEEP_PETRIFY = 73,
	/// <summary>
	/// Fevered Gaze, Fruit Kingdom, Ambrosial Egg, Flower Milkshake
	/// </summary>
	CURE_POISON_BLIND_FREEZE = 74,
	/// <summary>
	/// Turnin' Tempura, Sweeeeet Crepe, Heavenly Tempura
	/// </summary>
	MOV1_SPD10P = 75,
	/// <summary>
	/// Rainbow Surprise, Aurora Ball (71?)
	/// </summary>
	RANDOM_STATUS2 = 76,

	/// <summary>
	/// Block 1 or 2 attacks
	/// </summary>
	GRAIL_SPHERE = 90,
	/// <summary>
	/// Also True Barrage
	/// </summary>
	TRUE_PUMMEL = 91,

	/// <summary>
	/// Dunno how this differes from EP_ABSORB
	/// </summary>
	EP_DRAIN = 92 ,
	STEAL    = 93,
	/// <summary>
	/// Surpreme Evil Eye
	/// </summary>
	CONFUSE2 = 94 ,

	_95 = 95,

	_67  = 67,
	_98  = 98,
	_99  = 99,
	_100 = 100,
	_101 = 101,
	_102 = 102,
	_103 = 103,
	_104 = 104,
	_106 = 106,
	_107 = 107,
	_108 = 108,
	_111 = 111,
	_112 = 112,
	_113 = 113,
	_114 = 114,
	_115 = 115,
	_116 = 116,
	_117 = 117,
	_118 = 118,
	_119 = 119,
	_120 = 120,
	_121 = 121,
	_122 = 122,
	_124 = 124,
	_125 = 125,
	_126 = 126,
}
