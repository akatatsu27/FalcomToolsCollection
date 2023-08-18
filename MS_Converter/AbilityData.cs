using Shared;

namespace MS_Converter;

public class AbilityData
{
	public ushort ID;
	public AbilityFlagEnum AbilityFlags;
	public ElementEnum Element;
	public AbilityTarget Target;
	public EffectEnum Effect1;
	public EffectEnum Effect2;
	public ushort TargetParam1;
	public ushort TargetParam2;
	public ushort ChantDuration;
	public ushort CooldownDuration;
	public ushort Cost;
	public ushort Unk_12;
	public ushort Effect1Power;
	public ushort Effect1Duration;
	public ushort Effect2Power;
	public ushort Effect2Duration;

	/// <summary>
	/// Shift-JIS encoding, null terminated.
	/// </summary>
	public string Name;
	/// <summary>
	/// Shift-JIS encoding, null terminated.
	/// </summary>
	public string Description;

	public AbilityData() { }

	public AbilityData(ref ushort curOffset, FileClass mS_File)
	{
		ID = mS_File.ReadUInt16(ref curOffset);
		AbilityFlags = (AbilityFlagEnum)mS_File.ReadUInt16(ref curOffset);
		Element = (ElementEnum)mS_File.ReadUInt8(ref curOffset);
		Target = (AbilityTarget)mS_File.ReadUInt8(ref curOffset);
		Effect1 = (EffectEnum)mS_File.ReadUInt8(ref curOffset);
		Effect2 = (EffectEnum)mS_File.ReadUInt8(ref curOffset);
		TargetParam1 = mS_File.ReadUInt16(ref curOffset);
		TargetParam2 = mS_File.ReadUInt16(ref curOffset);
		ChantDuration = mS_File.ReadUInt16(ref curOffset);
		CooldownDuration = mS_File.ReadUInt16(ref curOffset);
		Cost = mS_File.ReadUInt16(ref curOffset);
		Unk_12= mS_File.ReadUInt16(ref curOffset);
		Effect1Power = mS_File.ReadUInt16(ref curOffset);
		Effect1Duration= mS_File.ReadUInt16(ref curOffset);
		Effect2Power = mS_File.ReadUInt16(ref curOffset);
		Effect2Duration = mS_File.ReadUInt16(ref curOffset);
		Name = mS_File.ReadShiftJISString(ref curOffset);
		Description = mS_File.ReadShiftJISString(ref curOffset);
	}

	public unsafe byte[] ToByteArray()
	{
		byte[] nameBytes = Name.ToSHIFTJIS_CString();
		byte[] descBytes = Description.ToSHIFTJIS_CString();
		byte[] byteArray = new byte[28 + nameBytes.Length + descBytes.Length];
		fixed (byte* bytes = byteArray)
		{
			*(ushort*)bytes = ID;
			*(ushort*)(bytes + 2) = (ushort)AbilityFlags;
			bytes[4] = (byte)Element;
			bytes[5] = (byte)Target;
			byteArray[6] = (byte)Effect1;
			byteArray[7] = (byte)Effect2;
			*(ushort*)(bytes + 8) = TargetParam1;
			*(ushort*)(bytes + 10) = TargetParam2;
			*(ushort*)(bytes + 12) = ChantDuration;
			*(ushort*)(bytes + 14) = CooldownDuration;
			*(ushort*)(bytes + 16) = Cost;
			*(ushort*)(bytes + 18) = Unk_12;
			*(ushort*)(bytes + 20) = Effect1Power;
			*(ushort*)(bytes + 22) = Effect1Duration;
			*(ushort*)(bytes + 24) = Effect2Power;
			*(ushort*)(bytes + 26) = Effect2Duration;
			nameBytes.CopyTo(byteArray, 28);
			descBytes.CopyTo(byteArray, 28 + nameBytes.Length);
		}
		return byteArray;
	}
}