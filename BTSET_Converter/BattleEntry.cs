using Newtonsoft.Json;

namespace BTSET_Converter;

internal abstract class Battle
{
}

internal class BattleEntry : Battle
{
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Id;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Flags;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk04;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk06;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk08;
	internal UInt16 FieldOffset;
	[JsonIgnore] public byte[] VariationWeights = new byte[4];      
	[JsonProperty(Required = Required.Always)] public readonly Variation?[] Variations = { null, null, null, null }; // at most 4 different variations. First one is mandatory
	
	//Metadata
	internal UInt16 Offset;
	[JsonIgnore] internal ModelEntry BattlefieldEntry;
	[JsonProperty(Required = Required.Always)] public readonly string BattlefieldName;
	[JsonProperty("VariationWeights", Required = Required.Always)] public int[] JsonBytes;

	internal BattleEntry(ref UInt16 offset, BTSET file, bool canBeEmpty, bool SCISAHACKYGAME = false) // battles 202, 203, and 204 in SC have only 1 variation, despite having 3 non-zero variation weights
	{
		Offset = offset;

		Id = file.ReadUInt16(ref offset);
		Flags = file.ReadUInt16(ref offset);
		Unk04 = file.ReadUInt16(ref offset);
		Unk06 = file.ReadUInt16(ref offset);
		Unk08 = file.ReadUInt16(ref offset);
		FieldOffset = file.ReadUInt16(ref offset);
		VariationWeights[0] = file.ReadUInt8(ref offset);
		VariationWeights[1] = file.ReadUInt8(ref offset);
		VariationWeights[2] = file.ReadUInt8(ref offset);
		VariationWeights[3] = file.ReadUInt8(ref offset);

		BattlefieldEntry = file.ReadBattlefield(FieldOffset, this);
		if (canBeEmpty)
		{
			if (SCISAHACKYGAME && (Id == 1062))
			{
				Variations[0] = new Variation(ref offset, file);
			}
			else if (VariationWeights[0] != 0)
			{
				Variations[0] = new Variation(ref offset, file);
			}
		}
		else
		{
			Variations[0] = new Variation(ref offset, file);
		}

		if (SCISAHACKYGAME && (Id == 202 || Id == 203 || Id == 204 || Id == 1047 || Id == 1316)) goto SKIP;

		if (SCISAHACKYGAME && Id == 488)
		{
			Variations[1] = new Variation(ref offset, file);
		}
		else if (VariationWeights[1] != 0)
		{
			Variations[1] = new Variation(ref offset, file);
		}

		if (SCISAHACKYGAME && (Id == 205 ||Id == 488 || Id == 3901))
		{
			Variations[2] = new Variation(ref offset, file);
		}
		else if (VariationWeights[2] != 0)
		{
			Variations[2] = new Variation(ref offset, file);
		}

		if (SCISAHACKYGAME && (Id == 205))
		{
			Variations[3] = new Variation(ref offset, file);
		}
		else if (VariationWeights[3] != 0)
		{
			Variations[3] = new Variation(ref offset, file);
		}
		SKIP:
		BattlefieldName = BattlefieldEntry.Filename;
		JsonBytes = VariationWeights?.Select(x => (int)x).ToArray() ?? Array.Empty<int>();
	}

	//For Newtonsoft only
	public BattleEntry() { }

	internal byte[] ToByteArray()
	{
		IEnumerable<byte> data = BitConverter.GetBytes(Id).Concat(BitConverter.GetBytes(Flags))
			.Concat(BitConverter.GetBytes(Unk04)).Concat(BitConverter.GetBytes(Unk06))
			.Concat(BitConverter.GetBytes(Unk08)).Concat(BitConverter.GetBytes(FieldOffset))
			.Concat(VariationWeights);
		foreach(Variation? var in Variations)
		{
			if (var == null) continue;
			data = data.Concat(var.ToByteArray());
		}

		return data.ToArray();
	}
}
