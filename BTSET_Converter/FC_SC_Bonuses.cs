using Newtonsoft.Json;

namespace BTSET_Converter;
internal class FC_SC_Bonuses : Bonuses
{
	[JsonProperty(Required = Required.Always)] public override byte Unk00 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Hp10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Hp50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Ep10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Ep50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Cp10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Cp50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Atk10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Atk50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Unk09 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Unk0A { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Sepith { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Crit { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Unk0D { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Unk0E { get; protected set; }
	[JsonProperty(Required = Required.Always)] public override byte Unk0F { get; protected set; }

	internal FC_SC_Bonuses(ref UInt16 offset, BTLSET file)
	{
		Offset = offset;

		Unk00 = file.ReadUInt8(ref offset);
		Hp10 = file.ReadUInt8(ref offset);
		Hp50 = file.ReadUInt8(ref offset);
		Ep10 = file.ReadUInt8(ref offset);
		Ep50 = file.ReadUInt8(ref offset);
		Cp10 = file.ReadUInt8(ref offset);
		Cp50 = file.ReadUInt8(ref offset);
		Atk10 = file.ReadUInt8(ref offset);
		Atk50 = file.ReadUInt8(ref offset);
		Unk09 = file.ReadUInt8(ref offset);
		Unk0A = file.ReadUInt8(ref offset);
		Sepith = file.ReadUInt8(ref offset);
		Crit = file.ReadUInt8(ref offset);
		Unk0D = file.ReadUInt8(ref offset);
		Unk0E = file.ReadUInt8(ref offset);
		Unk0F = file.ReadUInt8(ref offset);
	}
	//For Newtonsoft only
	public FC_SC_Bonuses() { }

	internal override Bonuses AddReference(Variation variation)
	{
		UsedByList.Add(variation);
		return this;
	}

	internal override byte[] ToByteArray()
	{
		return new byte[] { Unk00, Hp10, Hp50, Ep10, Ep50, Cp10, Cp50, Atk10, Atk50, Unk09, Unk0A, Sepith, Crit, Unk0D, Unk0E, Unk0F };
	}
}
