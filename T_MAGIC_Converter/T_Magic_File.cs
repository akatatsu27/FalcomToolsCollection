using System.Diagnostics;
using System.Text;
using Newtonsoft.Json;
using Shared;

namespace T_MAGIC_Converter;

public class T_Magic_File : FileClass
{
	[JsonIgnore] public ushort[] OffsetTable;
	public AbilityData[] Data;

	static T_Magic_File()
	{
		Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
		ShiftJis = Encoding.GetEncoding("shift-jis");
	}
	private static readonly Encoding ShiftJis;
	public T_Magic_File() { }

	public static async Task<T_Magic_File> FromFile(string filepath)
	{
		T_Magic_File file = new();
		await file.Parse(filepath);
		return file;
	}

	private async Task Parse(string filepath)
	{
		Bytes = await File.ReadAllBytesAsync(filepath);
		Coverage = new bool[Bytes.Length];

		List<ushort> offsets = new();
		ushort curOffset = 0;
		offsets.Add(ReadUInt16(ref curOffset));
		while (curOffset < offsets[0])
		{
			offsets.Add(ReadUInt16(ref curOffset));
		}
		//Debug.Assert(offsets.IsSorted()); //false
		Data = new AbilityData[offsets.Count];
		for (int i = 0; i < offsets.Count; i++)
		{
			Data[i] = new(ref curOffset, this);
		}
		//Data = Data.OrderBy(w => w.ID).ToArray();
		Debug.Assert(IsFullCoverage);
	}

	/// <summary>
	/// Preserves the order of appearance
	/// </summary>
	/// <returns></returns>
	public async Task<byte[]> ToByteArray()
	{
		OffsetTable = new ushort[Data.Length];
		ushort curOffset = (ushort)(OffsetTable.Length * sizeof(ushort));
		AbilityData[] ordered = Data.OrderBy(w => w.ID).ToArray();
		for (int i = 0; i < Data.Length; i++)
		{
			Data[i].Offset = curOffset;
			OffsetTable[Data[i].ID] = curOffset;
			curOffset += Data[i].Size;
		}
		BinaryWriter bw = new(new MemoryStream(), ShiftJis);
		for (int i = 0; i < OffsetTable.Length; i++)
			bw.Write(OffsetTable[i]);
		for (int i = 0; i < Data.Length; i++)
			bw.Write(Data[i].ToByteArray());
		byte[] asBytes = (bw.BaseStream as MemoryStream).ToArray();
		bw.Close();
		return asBytes;
	}
}