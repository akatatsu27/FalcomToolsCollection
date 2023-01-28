using Newtonsoft.Json;

namespace BTSET_Converter;

internal abstract class Bonuses
{
	[JsonProperty(Required = Required.Always)] public abstract byte Unk00 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Hp10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Hp50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Ep10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Ep50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Cp10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Cp50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Atk10 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Atk50 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Unk09 { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Unk0A { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Sepith { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Crit { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Unk0D { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Unk0E { get; protected set; }
	[JsonProperty(Required = Required.Always)] public abstract byte Unk0F { get; protected set; }

	//Metadata
	protected List<Variation> UsedByList = new();
	internal UInt16 Offset;
	internal abstract Bonuses AddReference(Variation variation);

	internal abstract byte[] ToByteArray();
}
