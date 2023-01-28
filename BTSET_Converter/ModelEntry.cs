using System.Diagnostics;
using System.Text;
using Newtonsoft.Json;

namespace BTSET_Converter;

internal class ModelEntry
{
    [JsonProperty(Required = Required.Always)] public readonly UInt16 Unk00;
    [JsonIgnore] public UInt16 NameOffset;
    [JsonIgnore] public string Filename; // null-terminated ASCII string

    //Metadata
    List<Battle> UsedByList = new();
    [JsonIgnore] internal UInt16 Offset;

    internal ModelEntry(ref UInt16 offset, BTSET file)
    {
        Offset = offset;

        Unk00 = file.ReadUInt16(ref offset);
        NameOffset = file.ReadUInt16(ref offset);
        Filename = file.ReadString(ref offset);
    }

    //For Newtonsoft only
    public ModelEntry() { }

    internal ModelEntry AddReference(Battle battleEntry)
    {
        UsedByList.Add(battleEntry);
        return this;
    }

    internal byte[] ToByteArray()
    {
        byte[] data = BitConverter.GetBytes(Unk00).Concat(BitConverter.GetBytes(NameOffset)).Concat(Encoding.ASCII.GetBytes(Filename)).Append<byte>(0).ToArray();
        return data;
    }
}
