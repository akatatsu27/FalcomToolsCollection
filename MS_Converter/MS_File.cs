using System.Diagnostics;
using Shared;

namespace MS_Converter;

public class MS_File : FileClass
{
	public ushort AS_Index;
	public ushort AS_dir;
	public ushort Level;
	public uint HPMax;
	public uint HPInitial;
	public ushort EPMax;
	public ushort EPInitial;
	public ushort CPMax;
	public ushort CPInitial;
	public ushort SPD;
	public ushort MoveSpd;
	public ushort MOV;
	public ushort STR;
	public ushort DEF;
	public ushort ATS;
	public ushort ADF;
	public ushort DEX;
	public ushort AGL;
	public ushort RNG;
	public ushort Unk_2A;
	public ushort EXP;
	public ushort Unk_2E;
	public AiType AI_Type;
	public byte[] Unk_32 = new byte[6];
	public byte Flags;
	public byte DeathFlags;
	public byte UnderAttackFlags;
	public byte[] Unk_3B = new byte[5];
	public Gender Gender;
	public byte[] Unk_41 = new byte[9];
	public ushort ActorSize;
	public byte[] Unk_4C = new byte[0xA];
	public ushort SymbolTextureFileIndex;
	public ushort SymbolTextureFileDir;
	public StatusResistanceEnum StatusResistance;
	public byte[] Unk_5E = new byte[0xB];
	public ushort EarthElementalEfficacy;
	public ushort WaterElementalEfficacy;
	public ushort FireElementalEfficacy;
	public ushort WindElementalEfficacy;
	public ushort TimeElementalEfficacy;
	public ushort SpaceElementalEfficacy;
	public ushort MirageElementalEfficacy;
	public byte EarthSepith;
	public byte WaterSepith;
	public byte FireSepith;
	public byte WindSepith;
	public byte TimeSepith;
	public byte SpaceSepith;
	public byte MirageSepith;
	public byte[] Unk_7E = new byte[6];
	public ushort[] Equip = new ushort[5];
	public ushort[] Orb = new ushort[4];
	public AbilityAI NormalAttack;
	public byte[] ZeroPadding = new byte[8];
	public byte ArtsCount;
	public AbilityAI[] Arts;
	public byte CraftsCount;
	public AbilityAI[] Crafts;
	public byte SCraftsCount;
	public AbilityAI[] SCrafts;
	public byte AbilityDataCount;
	public AbilityData[] AbilityData;
	public byte[] UnkBeforeName = new byte[4];
	/// <summary>
	/// Shift-JIS, null terminated.
	/// </summary>
	public string Name;
	/// <summary>
	/// Shift-JIS, null terminated.
	/// </summary>
	public string Description;

	public static async Task<MS_File> FromFile(string filepath)
	{
		MS_File file = new();
		await file.Parse(filepath);
		return file;
	}

	private async Task Parse(string filePath)
	{
		UInt16 curOffset = 0;
		Bytes = await File.ReadAllBytesAsync(filePath);
		Coverage = new bool[Bytes.Length];

		AS_Index = ReadUInt16(ref curOffset);
		AS_dir = ReadUInt16(ref curOffset);
		Level = ReadUInt16(ref curOffset);
		HPMax = ReadUInt32(ref curOffset);
		HPInitial = ReadUInt32(ref curOffset);
		EPMax = ReadUInt16(ref curOffset);
		EPInitial = ReadUInt16(ref curOffset);
		CPMax = ReadUInt16(ref curOffset);
		CPInitial = ReadUInt16(ref curOffset);
		SPD = ReadUInt16(ref curOffset);
		MoveSpd = ReadUInt16(ref curOffset);
		MOV = ReadUInt16(ref curOffset);
		STR = ReadUInt16(ref curOffset);
		DEF = ReadUInt16(ref curOffset);
		ATS = ReadUInt16(ref curOffset);
		ADF = ReadUInt16(ref curOffset);
		DEX = ReadUInt16(ref curOffset);
		AGL = ReadUInt16(ref curOffset);
		RNG = ReadUInt16(ref curOffset);
		Unk_2A = ReadUInt16(ref curOffset);
		EXP = ReadUInt16(ref curOffset);
		Unk_2E = ReadUInt16(ref curOffset);
		AI_Type = (AiType)ReadUInt16(ref curOffset);
		for (int i = 0; i < 6; i++)
		{
			Unk_32[i] = ReadUInt8(ref curOffset);
		}
		Flags = ReadUInt8(ref curOffset);
		DeathFlags = ReadUInt8(ref curOffset);
		UnderAttackFlags = ReadUInt8(ref curOffset);
		for (int i = 0; i < 5; i++)
		{
			Unk_3B[i] = ReadUInt8(ref curOffset);
		}
		Gender = (Gender)ReadUInt8(ref curOffset);
		for (int i = 0; i < 9; i++)
		{
			Unk_41[i] = ReadUInt8(ref curOffset);
		}
		ActorSize = ReadUInt16(ref curOffset);
		for (int i = 0; i < 0xA; i++)
		{
			Unk_4C[i] = ReadUInt8(ref curOffset);
		}
		SymbolTextureFileIndex = ReadUInt16(ref curOffset);
		SymbolTextureFileDir = ReadUInt16(ref curOffset);
		StatusResistance = (StatusResistanceEnum)ReadUInt32(ref curOffset);
		for (int i = 0; i < 0xB; i++)
		{
			Unk_5E[i] = ReadUInt8(ref curOffset);
		}

		EarthElementalEfficacy = ReadUInt16(ref curOffset);
		WaterElementalEfficacy = ReadUInt16(ref curOffset);
		FireElementalEfficacy = ReadUInt16(ref curOffset);
		WindElementalEfficacy = ReadUInt16(ref curOffset);
		TimeElementalEfficacy = ReadUInt16(ref curOffset);
		SpaceElementalEfficacy = ReadUInt16(ref curOffset);
		MirageElementalEfficacy = ReadUInt16(ref curOffset);

		EarthSepith = ReadUInt8(ref curOffset);
		WaterSepith = ReadUInt8(ref curOffset);
		FireSepith = ReadUInt8(ref curOffset);
		WindSepith = ReadUInt8(ref curOffset);
		TimeSepith = ReadUInt8(ref curOffset);
		SpaceSepith = ReadUInt8(ref curOffset);
		MirageSepith = ReadUInt8(ref curOffset);
		for (int i = 0; i < 0x6; i++)
		{
			Unk_7E[i] = ReadUInt8(ref curOffset);
		}
		for (int i = 0; i < 5; i++)
		{
			Equip[i] = ReadUInt16(ref curOffset);
		}
		for (int i = 0; i < 4; i++)
		{
			Orb[i] = ReadUInt16(ref curOffset);
		}

		NormalAttack = new(ref curOffset, this);
		for (int i = 0; i < 8; i++)
		{
			ZeroPadding[i] = ReadUInt8(ref curOffset);
		}
		Debug.Assert(ZeroPadding.All(w => w == 0));
		ArtsCount = ReadUInt8(ref curOffset);
		Arts = new AbilityAI[ArtsCount];
		for (int i = 0; i < ArtsCount; i++)
		{
			Arts[i] = new(ref curOffset, this);
		}
		CraftsCount = ReadUInt8(ref curOffset);
		Crafts = new AbilityAI[CraftsCount];
		for (int i = 0; i < CraftsCount; i++)
		{
			Crafts[i] = new(ref curOffset, this);
		}
		SCraftsCount = ReadUInt8(ref curOffset);
		SCrafts = new AbilityAI[SCraftsCount];
		for (int i = 0; i < SCraftsCount; i++)
		{
			SCrafts[i] = new(ref curOffset, this);
		}
		AbilityDataCount = ReadUInt8(ref curOffset);
		AbilityData = new AbilityData[AbilityDataCount];
		for (int i = 0; i < AbilityDataCount; i++)
		{
			AbilityData[i] = new(ref curOffset, this);
		}
		for (int i = 0; i < 4; i++)
		{
			UnkBeforeName[i] = ReadUInt8(ref curOffset);
		}
		Name = ReadString(ref curOffset);
		Description = ReadString(ref curOffset);

		//Debug.Assert(IsFullCoverage);
		if (!IsFullCoverage)
		{
			Console.WriteLine($"{filePath}:\r\n Unknown Data the end of the file");
		}
	}
}