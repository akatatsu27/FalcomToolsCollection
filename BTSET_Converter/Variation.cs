using Newtonsoft.Json;

namespace BTSET_Converter;

internal class Variation
{
    [JsonProperty(Required = Required.Always)] public readonly EnemyEntry[] Enemies = new EnemyEntry[8];
    [JsonIgnore] internal UInt16 PlacementTableOffset;
    [JsonIgnore] internal UInt16 SurpriseTableOffset;
    [JsonProperty(Required = Required.Always)] public readonly UInt32 BgmNormal;
    [JsonProperty(Required = Required.Always)] public readonly UInt32 BgmCritical;
    [JsonIgnore] internal UInt32 BonusesOffset;

    //Metadata
    private UInt16 Offset;
    private Placement[] Placements = new Placement[8];
    private Placement[] SurprisePlacements = new Placement[8];
    private Bonuses Bonuses;

    [JsonProperty("Placements", Required = Required.Always)] public string PlacementsNames;
    [JsonProperty("SurprisePlacements", Required = Required.Always)] public string SurprisePlacementsNames;
    [JsonProperty("Bonuses", Required = Required.Always)] public string BonusesName;

    internal Variation(ref UInt16 offset, BTSET file)
    {
        Offset = offset;

        for(byte i = 0; i < 8; i++)
        {
            Enemies[i] = new EnemyEntry(ref offset, file);
        }
        PlacementTableOffset = file.ReadUInt16(ref offset);
        SurpriseTableOffset = file.ReadUInt16(ref offset);
        BgmNormal = file.ReadUInt32(ref offset);
        BgmCritical = file.ReadUInt32(ref offset);
        BonusesOffset = file.ReadUInt32(ref offset);


        UInt16 curOffset = PlacementTableOffset;
        Placements = file.ReadPlacement(ref curOffset, this);
        SurprisePlacements = file.ReadPlacement(ref curOffset, this);

        Bonuses = file.ReadBonuses((UInt16)BonusesOffset, this);
    }

    //For Newtonsoft only
    public Variation() { }

    internal byte[] ToByteArray()
    {
        byte[] data = Array.Empty<byte>();
        foreach(EnemyEntry entry in Enemies)
        {
            data = data.Concat(entry.ToByteArray()).ToArray();
        }

        return data.Concat(BitConverter.GetBytes(PlacementTableOffset)).Concat(BitConverter.GetBytes(SurpriseTableOffset))
            .Concat(BitConverter.GetBytes(BgmNormal)).Concat(BitConverter.GetBytes(BgmCritical))
            .Concat(BitConverter.GetBytes(BonusesOffset)).ToArray();
    }
}
