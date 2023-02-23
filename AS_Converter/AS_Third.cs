using System.Diagnostics;
using System.Text;
using Shared;

namespace AS_Converter;

internal class AS_Third
{
	internal ushort CraftOffsetTableOffset;
	internal ushort CraftOffsetTableOffsetEnd;
	internal ushort Bones3DOffset;
	internal List<ChipEntry> ChipEntries = new();
	internal string[] Model3D;
	internal Bones? Bones3D;
	internal List<ushort> FunctionOffsetTable = new();
	internal List<UnkBytesEntry> UnkBytes = new();
	internal List<Instruction> Instructions = new();
	internal class ChipEntry
    {
		internal DT_Entry CH;
		internal DT_Entry CP;

		internal ChipEntry(BinaryReader br)
		{
			CH = new(br);
			if (CH.Index == 0xFFFF) return;
			CP = new(br);
		}

		internal byte[] Serialize()
		{
			if (CH.Index == 0xFFFF) throw new InvalidOperationException("The terminator is excluded from the list!!!");
			return CH.Serialize().Concat(CP.Serialize()).ToArray();
		}
	}
	internal class DT_Entry
    {
		internal ushort Index;
		internal ushort DT_Table;

		internal DT_Entry(BinaryReader br)
		{
			Index = br.ReadUInt16();
			DT_Table = br.ReadUInt16();
		}

		internal byte[] Serialize()
		{
			return BitConverter.GetBytes(Index).Concat(BitConverter.GetBytes(DT_Table)).ToArray();
		}
	}
	internal class UnkBytesEntry
    {
		internal byte Unk00;
		internal byte Unk01;

		internal UnkBytesEntry(BinaryReader br)
		{
			Unk00 = br.ReadByte();
			Unk01 = br.ReadByte();
		}
		internal byte[] Serialize()
		{
			return new byte[] { Unk00, Unk01 };
		}
	}
	internal class Bones
    {
		internal byte Unk00;
		internal string[] Bones3d;

		internal Bones(BinaryReader br)
		{
			Unk00 = br.ReadByte();
			Bones3d = br.ReadASCII_CStringArray();
		}
		internal byte[] Serialize()
		{
			List<byte> data = new() { Unk00 };
			data.AddRange(Bones3d.ToASCII_ByteArrayCString_NullTermArray());
			return data.ToArray();
		}
	}
	internal static AS_Third ReadBinary(byte[] data)
	{
		AS_Third file = new();
		file.ParseBinary(data);
		return file;
	}
	private void ParseBinary(byte[] data)
	{
		using MemoryStream ms = new(data);
		using BinaryReader br = new(ms, Encoding.ASCII);

		CraftOffsetTableOffset = br.ReadUInt16();
		CraftOffsetTableOffsetEnd = br.ReadUInt16();
		if (CraftOffsetTableOffsetEnd < CraftOffsetTableOffset) throw new InvalidDataException("The journey cannot end before it begins!");
		Bones3DOffset = br.ReadUInt16();
		
		while(true)
		{
			ChipEntry chip = new(br);
			ChipEntries.Add(chip);

			if(ChipEntries.Last().CH.DT_Table == 0xFFFF)
			{
				ChipEntries.RemoveAt(ChipEntries.Count - 1);
				break;
			}
		}
		string[] model3D = br.ReadASCII_CStringArray();

		Model3D = model3D;

		if(Bones3DOffset != 0)
		{
			Bones3D = new(br);
		}
		ushort functionTblEntries = (ushort)((CraftOffsetTableOffsetEnd - CraftOffsetTableOffset) / 2);
		for (ushort i = 0; i < functionTblEntries; i++)
		{
			FunctionOffsetTable.Add(br.ReadUInt16());
		}
		for(byte i = 0; i < 8; i++)
		{
			UnkBytes.Add(new(br));
		}
		while(br.BaseStream.Position != br.BaseStream.Length)
		{
			Instructions.Add(Instruction.Parse(br));
		}
	}

	private byte[] SerializeToBinary()
	{
		using MemoryStream ms = new();
		using BinaryWriter bw = new(ms, Encoding.ASCII);

		if (CraftOffsetTableOffsetEnd < CraftOffsetTableOffset) throw new InvalidDataException("The journey cannot end before it begins!");
		bw.Write(CraftOffsetTableOffset);
		bw.Write(CraftOffsetTableOffsetEnd);
		bw.Write(Bones3DOffset);

		for (byte i = 0; i < ChipEntries.Count; i++)
		{
			byte[] chip = ChipEntries[i].Serialize();
			bw.Write(chip);
		}
		bw.Write(0xFFFFFFFF);
		for (byte i = 0; i < Model3D.Length; i++)
		{
			byte[] data = Model3D[i].ToASCII_ByteArrayCString();
			bw.Write(data);
		}
		bw.Write((byte)0);
		if (Bones3DOffset != 0)
		{
			byte[] data = Bones3D.Serialize();
			bw.Write(data);
		}
		for (ushort i = 0; i < FunctionOffsetTable.Count; i++)
		{
			bw.Write(FunctionOffsetTable[i]);
		}
		for (byte i = 0; i < UnkBytes.Count; i++)
		{
			bw.Write(UnkBytes[i].Serialize());
		}
		for (byte i = 0; i < Instructions.Count; i++)
		{
			bw.Write(Instructions[i].Serialize());
		}
		return (bw.BaseStream as MemoryStream).ToArray();
	}

	internal static AS_Third ReadText(string fPath)
	{
		AS_Third file = new();
		file.ParseText(fPath);
		return file;
	}
	private void ParseText(string fPath)
	{
		string[] lines = File.ReadAllLines(fPath, Encoding.ASCII);
	}
}
