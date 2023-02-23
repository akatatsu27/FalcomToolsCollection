using System.Diagnostics;
using System.Reflection;
using System.Reflection.Emit;
using Shared;

namespace AS_Converter;

internal abstract class Instruction 
{
	protected byte Opcode;

	internal byte[] Serialize()
	{
		List<byte> data = new();
		if(GetType().Name == nameof(AS_8E01_LOAD_X_FILE))
		{
			data.Add(0x01);
		}
		FieldInfo[] fields = GetType().GetFields();
		for (byte i = 0; i < fields.Length; i++)
		{
			Type fType = fields[i].FieldType;
			object? value = fields[i].GetValue(this);
			if (value == null) throw new InvalidDataException($"{GetType().Name}|{fType.Name} wtf why is this empty");
			if (fType == typeof(byte))
			{
				data.Add((byte)value);
			}
			else if (fType == typeof(ushort))
			{
				ushort val = (ushort)value;
				data.AddRange(BitConverter.GetBytes(val));
			}
			else if (fType == typeof(uint))
			{
				uint val = (uint)value;
				data.AddRange(BitConverter.GetBytes(val));
			}
			else if (fType == typeof(string))
			{
				string val = (string)value;
				data.AddRange(val.ToASCII_ByteArrayCString());
			}
			else if (fType == typeof(string[]))
			{
				string[] val = (string[])value;
				data.AddRange(val.ToASCII_ByteArrayCString_NullTermArray());
			}
			else
			{
				throw new NotImplementedException($"How could this happen to me~");
			}
		}
		return data.ToArray();
	}

	internal static Instruction Parse(BinaryReader br)
	{
		byte opcode = br.ReadByte();
		if (opcode == 0x8E)
		{
			byte nextByte = br.ReadByte();
			if (nextByte == 0x01)
			{
				return new AS_8E01_LOAD_X_FILE() { Opcode = 0x8E, Op1 = br.ReadByte(), Op2 = br.ReadCString() };
			}
			else if (nextByte == 0x0D)
			{
				return new AS_8E0D () { Opcode = 0x8E, Op1 = nextByte, Op2 = br.ReadByte(), Op3 = br.ReadUInt32(), Op4 = br.ReadUInt32(), Op5 = br.ReadUInt32(), Op6 = br.ReadUInt32(), Op7 = br.ReadUInt32() };
			}
			else
			{
				if (nextByte == 0x0D || nextByte == 0x0E || nextByte == 0x0F) throw new InvalidDataException($"8E{nextByte.ToString("X2")} isn't a valid opcode");
				return new AS_8EXX() { Opcode = 0x8E, Op1 = nextByte, Op2 = br.ReadByte(), Op3 = br.ReadUInt32(), Op4 = br.ReadUInt32(), Op5 = br.ReadUInt32(), Op6 = br.ReadUInt32() };
			}
		}
		else if (opcode == 0xA4)
		{
			byte nextByte = br.ReadByte();
			if (nextByte == 0x02)
			{
				return new AS_A402() { Op1 = br.ReadUInt16() };
			}
			else
			{
				if (nextByte == 0x00 || nextByte == 0x01)
					return new AS_A4() { Op1 = nextByte };
				else throw new InvalidDataException($"A4{nextByte.ToString("X2")} isn't a valid opcode");
			}
		}
		if (opcode > 0xB1) throw new InvalidDataException($"Tried to read invalid opcode at offset 0x{(br.BaseStream.Position - 1).ToString("X")}");
		Type instrT = Assembly.GetAssembly(typeof(Instruction)).GetTypes().Where(w => w.Name.StartsWith($"AS_{opcode.ToString("X2")}")).First();
		object instr = Activator.CreateInstance(instrT);
		FieldInfo[] fields = instrT.GetFields(BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.FlattenHierarchy | BindingFlags.Public);
		for (byte i = 0; i < fields.Length; i++)
		{
			if (fields[i].Name == nameof(Opcode))
			{
				fields[i].SetValue(instr, opcode);
				continue;
			}
			Type fType = fields[i].FieldType;
			object val;
			if(fType == typeof(byte))
			{
				val = br.ReadByte();

			}
			else if (fType == typeof(ushort))
			{
				val = br.ReadUInt16();
			}
			else if (fType == typeof(uint))
			{
				val = br.ReadUInt32();
			}
			else if (fType == typeof(string))
			{
				val = br.ReadCString();
			}
			else if (fType == typeof(string[]))
			{
				string[] data = br.ReadASCII_CStringArray();
				val = data;
			}
			else
			{
				throw new NotImplementedException($"Parsing Type {fType.Name} is not implemented");
			}
			fields[i].SetValue(instr, val);
		}
		Instruction inst = instr as Instruction;
		if (inst.Opcode == 0x50)
		{
			if ((inst as AS_50_CALL).Op1 > br.BaseStream.Length) throw new InvalidDataException("Call offset bigger than file size");
		}
		return inst;
	}
}
internal class AS_00_END : Instruction { }
internal class AS_01_GOTO : Instruction { ushort Offset; }
internal class AS_02_SELECT_SUB_CHIP : Instruction { byte Op1; byte Op2; }
internal class AS_03_DEG : Instruction { byte Op1; ushort Op2; }
internal class AS_04 : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_05 : Instruction { byte Op1; byte Op2; uint Op3; }
internal class AS_06_SLEEP : Instruction { uint Op1; }
internal class AS_07_CLEAR_EFFECT : Instruction { /* no operands */ }
internal class AS_08_TELEPORT : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; }
internal class AS_09 : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; }
internal class AS_0A : Instruction { byte Op1; byte Op2; byte Op3; uint Op4; }
internal class AS_0B_TURN : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_0C : Instruction { byte Op1; byte Op2; ushort Op3; ushort Op4; byte Op5; }
internal class AS_0D_JUMP : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; ushort Op6; ushort Op7; }
internal class AS_0E_DROP_DOWN : Instruction { byte Op1; uint Op2; uint Op3; uint Op4; ushort Op5; ushort Op6; }
internal class AS_0F_JUMP_TO_TARGET : Instruction { ushort Op1; ushort Op2; }
internal class AS_10_JUMP_BACK : Instruction { ushort Op1; ushort Op2; }
internal class AS_11_MOVE : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; uint Op6; byte Op7; }
internal class AS_12_ADD_EFFECT : Instruction { ushort Op1; string Op2; }
internal class AS_13_RELEASE_EFFECT : Instruction { ushort Op1; }
internal class AS_14 : Instruction { ushort Op1; }
internal class AS_15_WAIT_EFFECT : Instruction { byte Op1; byte Op2; }
internal class AS_16_FINISH_EFFECT : Instruction { byte Op1; byte Op2; }
internal class AS_17_CANCEL_EFFECT : Instruction { byte Op1; byte Op2; }
internal class AS_18_SHOW_EFFECT : Instruction
{
	byte Op1; byte Op2; byte Op3; ushort Op4; uint Op5; uint Op6; uint Op7;
	ushort Op8; ushort Op9; ushort Op10; ushort Op11; ushort Op12; ushort Op13; byte Op14;
}
internal class AS_19_SHOW_3D_EFFECT : Instruction
{
	byte Op1; byte Op2; string Op3; ushort Op4; uint Op5; uint Op6; uint Op7;
	ushort Op8; ushort Op9; ushort Op10; ushort Op11; ushort Op12; ushort Op13; byte Op14;
}
internal class AS_1A : Instruction { byte Op1; ushort Op2; }
internal class AS_1B_SELECT_CHIP : Instruction { byte Op1; byte Op2; }
internal class AS_1C_DAMAGE : Instruction { byte Op1; }
internal class AS_1D_DAMAGE_ANIME : Instruction { byte Op1; byte Op2; uint Op3; }
internal class AS_1E : Instruction { uint Op1; }
internal class AS_1F : Instruction { ushort Op1; ushort Op2; byte Op3; }
internal class AS_20 : Instruction { byte Op1; byte Op2; byte Op3; byte Op4; uint Op5; uint Op6; }
internal class AS_21 : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; }
internal class AS_22_BEGIN_THREAD : Instruction { byte Op1; byte Op2; ushort Op3; byte Op4; }
internal class AS_23_WAIT_THREAD : Instruction { byte Op1; byte Op2; }
internal class AS_24_SET_CHIP_MODE_FLAG : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_25_CLEAR_CHIP_MODE_FLAG : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_26 : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_27 : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_28_CHAR_SAY : Instruction { byte Op1; string Op2; uint Op3; }
internal class AS_29 : Instruction { byte Op1; }
internal class AS_2A_TIP_TEXT : Instruction { string Op1; uint Op2; }
internal class AS_2B : Instruction { /* no operands */ }
internal class AS_2C_SHADOW_BEGIN : Instruction { byte Op1; ushort Op2; ushort Op3; }
internal class AS_2D_SHADOW_END : Instruction { byte Op1; }
internal class AS_2E_SHAKE_CHAR : Instruction { byte Op1; uint Op2; uint Op3; uint Op4; }
internal class AS_2F_SUSPEND_THREAD : Instruction { byte Op1; byte Op2; }
internal class AS_30 : Instruction { byte Op1; string[] Op2; }
internal class AS_31 : Instruction { byte Op1; uint Op2; }
internal class AS_32 : Instruction { byte Op1; byte Op2; }
internal class AS_33 : Instruction { byte Op1; byte Op2; }
internal class AS_34 : Instruction { /* no operands */ }
internal class AS_35_KEEP_ANGLE : Instruction { byte Op1; uint Op2; uint Op3; uint Op4; uint Op5; }
internal class AS_36 : Instruction { uint Op1; uint Op2; uint Op3; uint Op4; }
internal class AS_37_ROTATION_ANGLE : Instruction { uint Op1; uint Op2; uint Op3; uint Op4; }
internal class AS_38_ROTATION_ANGLE_V : Instruction { uint Op1; uint Op2; uint Op3; uint Op4; }
internal class AS_39_SET_ANGLE : Instruction { uint Op1; uint Op2; }
internal class AS_3A_TILT_ANGLE : Instruction { uint Op1; uint Op2; }
internal class AS_3B_ROTATION_ANGLE_H : Instruction { uint Op1; uint Op2; }
internal class AS_3C : Instruction { ushort Op1; uint Op2; }
internal class AS_3D_SHAKE_SCREEN : Instruction { uint Op1; uint Op2; uint Op3; uint Op4; }
internal class AS_3E : Instruction { uint Op1; uint Op2; }
internal class AS_3F : Instruction { byte Op1; }
internal class AS_40 : Instruction { byte Op1; }
internal class AS_41_LOCK_ANGLE : Instruction { byte Op1; }
internal class AS_42 : Instruction { byte Op1; uint Op2; byte Op3; }
internal class AS_43_SET_BK_COLOR : Instruction { byte Op1; uint Op2; uint Op3; }
internal class AS_44_ZOOM_ANGLE : Instruction { byte Op1; uint Op2; uint Op3; }
internal class AS_45 : Instruction { byte Op1; uint Op2; }
internal class AS_46 : Instruction { byte Op1; uint Op2; uint Op3; }
internal class AS_47 : Instruction { byte Op1; }
internal class AS_48 : Instruction { byte Op1; uint Op2; }
internal class AS_49_SET_CONTROL : Instruction { byte Op1; ushort Op2; }
internal class AS_4A : Instruction { byte Op1; }
internal class AS_4B_RANDOM : Instruction { byte Op1; byte Op2; uint Op3; ushort Op4; }
internal class AS_4C_LOOP_TARGET_BEG : Instruction { ushort Op1; }
internal class AS_4D_RESET_LOOP_TARGET : Instruction { /* no operands */ }
internal class AS_4E_LOOP_TARGET_END : Instruction { /* no operands */ }
internal class AS_4F : Instruction { byte Op1; byte Op2; }
internal class AS_50_CALL : Instruction { public ushort Op1; }
internal class AS_51_RET : Instruction { /* no operands */ }
internal class AS_52 : Instruction { byte Op1; }
internal class AS_53 : Instruction { byte Op1; }
internal class AS_54 : Instruction { byte Op1; }
internal class AS_55_MAGIC_CAST_BEGIN : Instruction { ushort Op1; }
internal class AS_56_MAGIC_CAST_END : Instruction { /* no operands */ }
internal class AS_57 : Instruction { byte Op1; byte Op2; }
internal class AS_58_BEAT_BACK : Instruction { byte Op1; }
internal class AS_59 : Instruction { byte Op1; ushort Op2; }
internal class AS_5A : Instruction { byte Op1; byte Op2; uint Op3; }
internal class AS_5B : Instruction { uint Op1; }
internal class AS_5C_SHOW : Instruction { byte Op1; uint Op2; }
internal class AS_5D_HIDE : Instruction { byte Op1; uint Op2; }
internal class AS_5E : Instruction { byte Op1; }
internal class AS_5F : Instruction { byte Op1; byte Op2; }
internal class AS_60 : Instruction { byte Op1; }
internal class AS_61_SET_BATTLE_SPEED : Instruction { uint Op1; }
internal class AS_62 : Instruction { byte Op1; byte Op2; byte Op3; byte Op4; ushort Op5; }
internal class AS_63 : Instruction { byte Op1; uint Op2; }
internal class AS_64_SOUND_EFFECT : Instruction { ushort Op1; }
internal class AS_65_SOUND_EFFECT_EX : Instruction { ushort Op1; byte Op2; }
internal class AS_66 : Instruction { ushort Op1; }
internal class AS_67_SCRAFT_CUT_IN : Instruction { string Op1; }
internal class AS_68 : Instruction { /* no operands */ }
internal class AS_69_RELEASE_TEXTURE : Instruction { /* no operands */ }
internal class AS_6A_LOAD_SCHIP : Instruction { byte Op1; uint Op2; uint Op3; }
internal class AS_6B_RESET_SCRAFT_CHIP : Instruction { /* no operands */ }
internal class AS_6C_DIE : Instruction { /* no operands */ }
internal class AS_6D : Instruction { uint Op1; }
internal class AS_6E : Instruction { uint Op1; }
internal class AS_6F : Instruction { byte Op1; byte Op2; }
internal class AS_70 : Instruction { byte Op1; byte Op2; ushort Op3; ushort Op4; }
internal class AS_71 : Instruction { byte Op1; }
internal class AS_72 : Instruction { byte Op1; }
internal class AS_73 : Instruction { byte Op1; }
internal class AS_74 : Instruction { byte Op1; }
internal class AS_75 : Instruction { ushort Op1; }
internal class AS_76 : Instruction { byte Op1; }
internal class AS_77 : Instruction { byte Op1; }
internal class AS_78_SET_EFF_STATE : Instruction { byte Op1; }
internal class AS_79 : Instruction { byte Op1; }
internal class AS_7A_CRAFT_END : Instruction { byte Op1; }
internal class AS_7B_SET_CRAFT_END_FLAG : Instruction { ushort Op1; }
internal class AS_7C : Instruction { byte Op1; byte Op2; }
internal class AS_7D : Instruction { byte Op1; uint Op2; }
internal class AS_7E : Instruction { uint Op1; }
internal class AS_7F_BLUR_SCREEN : Instruction { uint Op1; uint Op2; uint Op3; byte Op4; uint Op5; }
internal class AS_80 : Instruction { uint Op1; }
internal class AS_81 : Instruction { byte Op1; byte Op2; ushort Op3; }
internal class AS_82 : Instruction { /* no operands */ }
internal class AS_83_SORT_TARGET : Instruction { byte Op1; }
internal class AS_84_ROTATE_CHAR : Instruction { byte Op1; ushort Op2; ushort Op3; ushort Op4; uint Op5; byte Op6; }
internal class AS_85 : Instruction { byte Op1; byte Op2; uint Op3; }
internal class AS_86 : Instruction { ushort Op1; ushort Op2; ushort Op3; byte Op4; uint Op5; }
internal class AS_87 : Instruction { ushort Op1; byte Op2; }
internal class AS_88_VOICE : Instruction { ushort Op1; }
internal class AS_89_SAVE_CUR_POS : Instruction { byte Op1; }
internal class AS_8A_CLONE : Instruction { byte Op1; byte Op2; }
internal class AS_8B_USE_ITEM_BEGIN : Instruction { /* no operands */ }
internal class AS_8C_USE_ITEM_END : Instruction { /* no operands */ }
internal class AS_8D_ZOOM : Instruction { byte Op1; uint Op2; uint Op3; uint Op4; uint Op5; }
internal class AS_8E01_LOAD_X_FILE : Instruction { public byte Op1; public string Op2; } //8E01 is a two-byte opcode, the 0x01 after the 0x8E won't be counted as an operand
internal class AS_8E0D : Instruction { public byte Op1; public byte Op2; public uint Op3; public uint Op4; public uint Op5; public uint Op6; public uint Op7; }
internal class AS_8EXX : Instruction { public byte Op1; public byte Op2; public uint Op3; public uint Op4; public uint Op5; public uint Op6; } // 1-byte 0x8E is the opcode
internal class AS_8F : Instruction { byte Op1; }
internal class AS_90 : Instruction { byte Op1; }
internal class AS_91 : Instruction { byte Op1; }
internal class AS_92 : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; ushort Op6; uint Op7; }
internal class AS_93 : Instruction { byte Op1; byte Op2; string Op3; }
internal class AS_94 : Instruction { byte Op1; string Op2; uint Op3; }
internal class AS_95 : Instruction { /* no operands */ }
internal class AS_96_SET_ANGLE_TARGET : Instruction { byte Op1; string Op2; ushort Op3; }
internal class AS_97_MOVE_ANGLE : Instruction { uint Op1; ushort Op2; ushort Op3; }
internal class AS_98 : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; }
internal class AS_99 : Instruction { byte Op1; }
internal class AS_9A : Instruction { uint Op1; }
internal class AS_9B : Instruction { byte Op1; }
internal class AS_9C_RESET_CHIP_STATUS : Instruction { byte Op1; }
internal class AS_9D : Instruction { byte Op1; }
internal class AS_9E_SET_TIMER : Instruction { byte Op1; }
internal class AS_9F_SET_BATTLE_MODE : Instruction { byte Op1; uint Op2; }
internal class AS_A0 : Instruction { byte Op1; uint Op2; ushort Op3; string Op4; }
internal class AS_A1 : Instruction { byte Op1; uint Op2; }
internal class AS_A2 : Instruction { byte Op1; }
internal class AS_A3 : Instruction { byte Op1; byte Op2; }
internal class AS_A4 : Instruction { public byte Op1; }
internal class AS_A402 : Instruction { public ushort Op1; } // The first byte is part of the 2-byte Opcode
internal class AS_A5 : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; byte Op5; }
internal class AS_A6 : Instruction { byte Op1; byte Op2; byte Op3; uint Op4; uint Op5; uint Op6; uint Op7; }
internal class AS_A7_BATTLE_EFFECT_END : Instruction { byte Op1; ushort Op2; }
internal class AS_A8_DAMAGE_VOICE : Instruction { byte Op1; byte Op2; }
internal class AS_A9 : Instruction { uint Op1; }
internal class AS_AA : Instruction { uint Op1; uint Op2; }
internal class AS_AB : Instruction { byte Op1; byte Op2; byte Op3; }
internal class AS_AC : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; byte Op5; }
internal class AS_AD : Instruction { byte Op1; byte Op2; uint Op3; }
internal class AS_AE : Instruction { ushort Op1; uint Op2; }
internal class AS_AF : Instruction { byte Op1; byte Op2; uint Op3; uint Op4; uint Op5; }
internal class AS_B0 : Instruction { byte Op1; ushort Op2; }
internal class AS_B1 : Instruction { byte Op1; ushort Op2; }