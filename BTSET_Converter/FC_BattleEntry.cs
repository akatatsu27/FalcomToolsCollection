using System.Diagnostics;
using Newtonsoft.Json;

namespace BTSET_Converter;

internal class FC_BattleEntry
{
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Id;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Flags;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk04;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk06;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk08;
	[JsonProperty(Required = Required.Always)] public readonly UInt16 Unk0A;
	[JsonProperty(Required = Required.Always)] public readonly EnemyEntry[] Enemies = new EnemyEntry[8];
	[JsonIgnore] internal UInt16 PlacementTableOffset;
	[JsonIgnore] internal UInt16 SurpriseTableOffset;
	[JsonIgnore] internal UInt32 FieldOffset; // value is still < 0xFFFF
	[JsonProperty(Required = Required.Always)] public readonly UInt32 Bgm;
	[JsonIgnore] internal UInt32 BonusesOffset; // value is still < 0xFFFF

	//Metadata
	[JsonIgnore] internal UInt16 Offset;
	[JsonIgnore] internal ModelEntry BattlefieldEntry;
	[JsonIgnore] private Placement[] Placements = new Placement[8];
	[JsonIgnore] private Placement[] SurprisePlacements = new Placement[8];
	[JsonIgnore] private FC_SC_Bonuses Bonuses;

	[JsonProperty("Placements", Required = Required.Always)] public string PlacementsNames;
	[JsonProperty("SurprisePlacements", Required = Required.Always)] public string SurprisePlacementsNames;
	[JsonProperty(Required = Required.Always)] public readonly string BattlefieldName;
	[JsonProperty("Bonuses", Required = Required.Always)] public string BonusesName;

	public FC_BattleEntry(ref UInt16 curOffset, BTLSET file)
	{
		Offset = curOffset;

		Id = file.ReadUInt16(ref curOffset);
		Flags = file.ReadUInt16(ref curOffset);
		Unk04 = file.ReadUInt16(ref curOffset);
		Unk06 = file.ReadUInt16(ref curOffset);
		Unk08 = file.ReadUInt16(ref curOffset);
		Unk0A = file.ReadUInt16(ref curOffset);
		for(byte i = 0; i < 8; i++)
		{
			Enemies[i] = new EnemyEntry(ref curOffset, file);
		}
		PlacementTableOffset = file.ReadUInt16(ref curOffset);
		SurpriseTableOffset = file.ReadUInt16(ref curOffset);
		FieldOffset = file.ReadUInt32(ref curOffset);
		Debug.Assert(FieldOffset < 0xFFFF);
		Bgm = file.ReadUInt32(ref curOffset);
		BonusesOffset = file.ReadUInt32(ref curOffset);
		Debug.Assert(BonusesOffset < 0xFFFF);
		BattlefieldEntry = file.ReadBattlefield((UInt16)FieldOffset, this);
		BattlefieldName = BattlefieldEntry.Filename;
	}

	//For Newtonsoft only
	public FC_BattleEntry() { }

	internal byte[] ToByteArray()
	{
		IEnumerable<byte> data = BitConverter.GetBytes(Id).Concat(BitConverter.GetBytes(Flags))
			.Concat(BitConverter.GetBytes(Unk04)).Concat(BitConverter.GetBytes(Unk06))
			.Concat(BitConverter.GetBytes(Unk08)).Concat(BitConverter.GetBytes(Unk0A));			
		foreach (EnemyEntry enemy in Enemies)
		{
			data = data.Concat(enemy.ToByteArray());
		}
		data = data.Concat(BitConverter.GetBytes(PlacementTableOffset)).Concat(BitConverter.GetBytes(SurpriseTableOffset))
			.Concat(BitConverter.GetBytes(FieldOffset)).Concat(BitConverter.GetBytes(Bgm)).Concat(BitConverter.GetBytes(BonusesOffset));
		return data.ToArray();
	}
}
