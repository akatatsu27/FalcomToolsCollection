using System.Diagnostics;

namespace MS_Converter;

/// <summary>
/// Used for arts, crafts, and miscellaneous abilities
/// </summary>
public class AbilityAI
{
	public AbilityCondition Condition;
	public byte Probability;
	public AbilityAITarget Target;
	public byte TargetCondition;
	public byte ArtChant_AS_Effect_Index;
	public byte AS_Effect_Index;
	/// <summary>
	/// Values below 1000 refer to abilities in T_MAGIC, and values starting from 1000 refer to abilities in the MS file
	/// </summary>
	public ushort AbilityDataIndex;
	public ushort Unk_08;
	public ushort Unk_0A;
	public uint Unk_0C;
	public ushort Unk_10;
	public ushort Unk_12;
	public uint Unk_14;

	public AbilityAI() { }
	public AbilityAI(ref ushort curOffset, MS_File mS_File)
	{
		Condition = (AbilityCondition)mS_File.ReadUInt8(ref curOffset);
		Probability = mS_File.ReadUInt8(ref curOffset);
		Target = (AbilityAITarget)mS_File.ReadUInt8(ref curOffset);
		TargetCondition = mS_File.ReadUInt8(ref curOffset);
		ArtChant_AS_Effect_Index = mS_File.ReadUInt8(ref curOffset);
		AS_Effect_Index = mS_File.ReadUInt8(ref curOffset);
		AbilityDataIndex = mS_File.ReadUInt16(ref curOffset);
		Unk_08 = mS_File.ReadUInt16(ref curOffset);
		Unk_0A = mS_File.ReadUInt16(ref curOffset);
		Unk_0C = mS_File.ReadUInt32(ref curOffset);
		Unk_10 = mS_File.ReadUInt16(ref curOffset);
		Unk_12 = mS_File.ReadUInt16(ref curOffset);
		Unk_14 = mS_File.ReadUInt32(ref curOffset);
	}

	internal unsafe byte[] ToByteArray()
	{
		byte[] byteArray = new byte[24];
		fixed (byte* bytes = byteArray)
		{
			bytes[0] = (byte)Condition;
			bytes[1] = Probability;
			bytes[2] = (byte)Target;
			bytes[3] = TargetCondition;
			bytes[4] = ArtChant_AS_Effect_Index;
			bytes[5] = AS_Effect_Index;
			*(ushort*)(bytes + 6) = AbilityDataIndex;
			*(ushort*)(bytes + 8) = Unk_08;
			*(ushort*)(bytes + 0x0A) = Unk_0A;
			*(uint*)(bytes + 0x0C) = Unk_0C;
			*(ushort*)(bytes + 0x10) = Unk_10;
			*(ushort*)(bytes + 0x12) = Unk_12;
			*(uint*)(bytes + 0x14) = Unk_14;
		}
		return byteArray;
	}
}

public enum AbilityCondition : byte
{
	NULL = 0,
	ATTACK1 = 1,
	ATTACK2 = 2,
	PINCH = 3,
	ATTACK3 = 4,
	_6 = 6,
	_7 = 7,
	_8 = 8,
	_10 = 10,
	DEATH = 11,
	HEAL = 13,
	REVIVE = 14,
	IMPEDE = 15,
	CURE = 19,
	BUFF = 20,
	/// <summary>
	/// Beta Drone - Anti-X Barrier
	/// </summary>
	_21 = 21,
	/// <summary>
	/// Reverie 2 - Functional Decline
	/// </summary>
	HP_LIMIT = 22,
	/// <summary>
	/// Reverie 2 - Cooling
	/// </summary>
	AFTER_SKILL = 23,
	_24 = 24,
	/// <summary>
	/// When targeted by arts?
	/// </summary>
	_25 = 25,
	/// <summary>
	/// Weissman undefined skill
	/// </summary>
	_26 = 26,
	/// <summary>
	/// Angel SP absorb
	/// </summary>
	_27 = 27,
}

public enum AbilityAITarget : byte
{
	_0 = 0,
	SELF = 1,
	FEMALE = 2,
	MALE = 3,
	/// <summary>
	/// According to Raven description
	/// </summary>
	WEAKEST = 5,
	_7 = 7,
	FOE = 8,
	_9 = 9,
	_10 = 10,
	CASTING = 11,
	_12 = 12,
	ALLY = 13,
	SURROUNDING = 14,
	REVERIE_LASER = 15,
	DEAD_ALLY = 17,
	ALL = 18,
	CURE = 19,

	/// <summary>
	/// Angel
	/// </summary>
	_20 = 20,
	/// <summary>
	/// Ragnard and Dragion
	/// </summary>
	_21 = 21,
	/// <summary>
	/// Ragnard, combined with target 16
	/// </summary>
	_22 = 22,
	_23 = 23,
	_24 = 24,
	_25 = 25,
	_26 = 26,
	_255 = 255,
}