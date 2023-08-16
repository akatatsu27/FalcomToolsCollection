namespace Shared;

public class FileClass
{
	protected byte[]? Bytes;
	protected bool[]? Coverage; // set to true if the byte has been read at least once. If all true => full coverage
	protected bool IsFullCoverage => Coverage.All(w => w == true);
	public byte ReadUInt8(ref UInt16 position)
	{
		byte num = Bytes[position];
		Coverage[position++] |= true;

		return num;
	}

	public UInt16 ReadUInt16(ref UInt16 position)
	{
		UInt16 num = Bytes.ReadU16(position);
		Coverage[position++] |= true;
		Coverage[position++] |= true;

		return num;
	}

	public UInt32 ReadUInt32(ref UInt16 position)
	{
		UInt32 num = Bytes.ReadU32(position);
		Coverage[position++] |= true;
		Coverage[position++] |= true;
		Coverage[position++] |= true;
		Coverage[position++] |= true;

		return num;
	}

	public string ReadString(ref UInt16 position)
	{
		UInt16 length = 0;
		while (Bytes[position + length] != 0) length++;
		string theString = System.Text.Encoding.ASCII.GetString(Bytes, position, length);
		for (int i = 0; i <= length; i++)
		{
			Coverage[position++] |= true;
		}
		return theString;
	}
}
