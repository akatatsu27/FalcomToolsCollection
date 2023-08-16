using Newtonsoft.Json;
using Shared;

namespace T_MAGIC_Converter;

public class T_Magic_File : FileClass
{
	[JsonIgnore] public ushort[] OffsetTable;
	public AbilityData[] Data;

	public static async Task<T_Magic_File> FromFile(string filepath)
	{
		T_Magic_File file = new();
		await file.Parse(filepath);
		return file;
	}

	private async Task Parse(string filepath)
	{
		List<ushort> offsets = new();
		ushort curOffset = 0;
		offsets.Add(ReadUInt16(ref curOffset));
		while(curOffset < offsets[0])
		{
			offsets.Add(ReadUInt16(ref curOffset));
		}
	}
}