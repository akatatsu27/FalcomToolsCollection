using System.Text;
using Shared;

namespace AS_Converter;

internal class AS_Third
{
	internal ushort CraftOffsetTableOffset;
	internal ushort CraftOffsetTableOffsetEnd;
	internal ushort Bones3DOffset;
	internal List<ChipEntry> ChipEntries = new();
	internal List<string> Model3D = new();
	internal Bones? Bones3D;
	internal List<ushort> FunctionOffsetTable = new();
	internal List<UnkBytesEntry> UnkBytes = new();
	internal List<Instruction> Instructions = new();
	internal class ChipEntry
    {
		internal DT_Entry CH;
		internal DT_Entry CP;

		public ChipEntry(BinaryReader br)
		{
			CH = new(br);
			if (CH.Index == 0xFFFF) return;
			CP = new(br);
		}
	}
	internal class DT_Entry
    {
		internal ushort Index;
		internal ushort DT_Table;

		public DT_Entry(BinaryReader br)
		{
			Index = br.ReadUInt16();
			DT_Table = br.ReadUInt16();
		}
	}
	internal class UnkBytesEntry
    {
		internal byte Unk00;
		internal byte Unk01;

		public UnkBytesEntry(BinaryReader br)
		{
			Unk00 = br.ReadByte();
			Unk01 = br.ReadByte();
		}
	}
	internal class Bones
    {
		internal byte Unk00;
		internal List<string> Bones3d = new();

		public Bones(BinaryReader br)
		{
			Unk00 = br.ReadByte();

			string bone;
			do
			{
				bone = br.ReadCString();
				Bones3d.Add(bone);
			} while (bone != string.Empty);
		}
	}
	internal static AS_Third Read(byte[] data)
	{
		AS_Third file = new();
		file.Parse(data);
		return file;
	}
	private void Parse(byte[] data)
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
		string model3D;
		while (true)
		{
			model3D = br.ReadCString();

			if (model3D == "\0")
				break;

			Model3D.Add(model3D);
		} 

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
		for (byte i = 0; i < 8; i++)
		{
			Instructions.Add(Instruction.Parse(br));
		}
	}
}
