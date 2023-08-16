using Shared;

namespace MS_Converter;

public class AbilityData
{
	public ushort ID;
	public ushort AbilityFlags;
	public ElementEnum Element;
	public byte Target;
	public EffectEnum Effect1;
	public EffectEnum Effect2;
	public AbilityTarget TargetA;
	public AbilityTarget TargetB;
	public ushort ChantDuration;
	public ushort CooldownDuration;
	public ushort Cost;
	public ushort TargetingFlag;
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
		AbilityFlags = mS_File.ReadUInt16(ref curOffset);
		Element = (ElementEnum)mS_File.ReadUInt8(ref curOffset);
		Target = mS_File.ReadUInt8(ref curOffset);
		Effect1 = (EffectEnum)mS_File.ReadUInt8(ref curOffset);
		Effect2 = (EffectEnum)mS_File.ReadUInt8(ref curOffset);
		TargetA = (AbilityTarget)mS_File.ReadUInt16(ref curOffset);
		TargetB = (AbilityTarget)mS_File.ReadUInt16(ref curOffset);
		ChantDuration = mS_File.ReadUInt16(ref curOffset);
		CooldownDuration = mS_File.ReadUInt16(ref curOffset);
		Cost = mS_File.ReadUInt16(ref curOffset);
		TargetingFlag= mS_File.ReadUInt16(ref curOffset);
		Effect1Power = mS_File.ReadUInt16(ref curOffset);
		Effect1Duration= mS_File.ReadUInt16(ref curOffset);
		Effect2Power = mS_File.ReadUInt16(ref curOffset);
		Effect2Duration = mS_File.ReadUInt16(ref curOffset);
		Name = mS_File.ReadString(ref curOffset);
		Description = mS_File.ReadString(ref curOffset);
	}

	public unsafe byte[] ToByteArray()
	{
		byte[] byteArray = new byte[28 + Name.Length + 1 + Description.Length + 1];
		fixed (byte* bytes = byteArray)
		{
			*(ushort*)bytes = ID;
			*(ushort*)(bytes + 2) = AbilityFlags;
			bytes[4] = (byte)Element;
			bytes[5] = Target;
			byteArray[6] = (byte)Effect1;
			byteArray[7] = (byte)Effect2;
			*(ushort*)(bytes + 8) = (ushort)TargetA;
			*(ushort*)(bytes + 10) = (ushort)TargetB;
			*(ushort*)(bytes + 12) = ChantDuration;
			*(ushort*)(bytes + 14) = CooldownDuration;
			*(ushort*)(bytes + 16) = Cost;
			*(ushort*)(bytes + 18) = TargetingFlag;
			*(ushort*)(bytes + 20) = Effect1Power;
			*(ushort*)(bytes + 22) = Effect1Duration;
			*(ushort*)(bytes + 24) = Effect2Power;
			*(ushort*)(bytes + 26) = Effect2Duration;
			Name.ToSHIFTJIS_CString().CopyTo(byteArray, 28);
			Description.ToSHIFTJIS_CString().CopyTo(byteArray, 28 + Name.Length + 1);
		}
		return byteArray;
	}
}