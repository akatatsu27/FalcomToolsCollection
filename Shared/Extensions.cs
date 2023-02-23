﻿using System.Buffers.Binary;
using System.Text;

namespace Shared
{
	public static class Extensions
	{
		public static ushort ReadU16(this byte[] bs, int position) => BinaryPrimitives.ReadUInt16LittleEndian(bs.AsSpan()[position..]);
		public static uint ReadU32(this byte[] bs, long position) => BinaryPrimitives.ReadUInt32LittleEndian(bs.AsSpan()[(int)position..]);
		public static string ReadCString(this BinaryReader br)
		{
			List<byte> strBytes = new();
			while (true)
			{
				byte theByte = br.ReadByte();
				strBytes.Add(theByte);

				if (theByte == 0)
					break;
			}
			
			string str = Encoding.ASCII.GetString(strBytes.ToArray());
			return str;
		}
		public static byte[] ToASCII_ByteArrayCString(this string str)
		{
			return Encoding.ASCII.GetBytes(str).Append<byte>(0).ToArray();
		}
		public static byte[] ToSHIFTJIS_CString(this string str)
		{
			Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
			var encoding = Encoding.GetEncoding("shift-jis");
			return encoding.GetBytes(str).Append((byte)0).ToArray();
		}
		public static string[] ReadASCII_CStringArray(this BinaryReader br)
		{
			List<string> data = new();
			string str;
			while (true)
			{
				str = br.ReadCString();

				if (str == "\0")
					break;

				data.Add(str);
			}
			return data.ToArray();
		}
		public static byte[] ToASCII_ByteArrayCString_NullTermArray(this string[] arr)
		{
			List<byte> data = new();
			for (int i = 0; i < arr.Length; i++)
			{
				data.AddRange(arr[i].ToASCII_ByteArrayCString());
			}
			data.Add(0);
			return data.ToArray();
		}
	}
}