#include "as_instruction.h"

instruction instruction::first_pass_binary(binary_context *const ctx)
{
#define b ctx->u8()
#define s ctx->u16()
#define i ctx->u32()
#define str ctx->cstring()

	instruction instr;
	instr.offset = ctx->position;
	instr.opcode = ctx->u8();
	switch (instr.opcode)
	{
	case 0x00: // no operands
		return instr;
	case 0x01:
		instr.targetOffset = s;
		return instr;
	case 0x02:
		ctx->position += 1 + 1;
		return instr;
	case 0x03:
		ctx->position += 1 + 2;
		return instr;
	case 0x04:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x05:
		ctx->position += 1 + 1 + 4;
		return instr;
	case 0x06:
		ctx->position += 4;
		return instr;
	case 0x07: // no operands
		return instr;
	case 0x08:
		ctx->position += 1 + 1 + 4 + 4 + 4;
		return instr;
	case 0x09:
		ctx->position += 1 + 1 + 4 + 4 + 4;
		return instr;
	case 0x0A:
		ctx->position += 1 + 1 + 1 + 4;
		return instr;
	case 0x0B:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x0C:
		ctx->position += 1 + 1 + 2 + 2 + 1;
		return instr;
	case 0x0D:
		ctx->position += 1 + 1 + 4 + 4 + 4 + 2 + 2;
		return instr;
	case 0x0E:
		ctx->position += 1 + 4 + 4 + 4 + 2 + 2;
		return instr;
	case 0x0F:
		ctx->position += 2 + 2;
		return instr;
	case 0x10:
		ctx->position += 2 + 2;
		return instr;
	case 0x11:
		ctx->position += 1 + 1 + 4 + 4 + 4 + 4 + 1;
		return instr;
	case 0x12:
		s; str;
		return instr;
	case 0x13:
		ctx->position += 2;
		return instr;
	case 0x14:
		ctx->position += 2;
		return instr;
	case 0x15:
		ctx->position += 1 + 1;
		return instr;
	case 0x16:
		ctx->position += 1 + 1;
		return instr;
	case 0x17:
		ctx->position += 1 + 1;
		return instr;
	case 0x18:
		ctx->position += 1 + 1 + 1 + 2 + 4 + 4 + 4 + 2 + 2 + 2 + 2 + 2 + 2 + 1;
		return instr;
	case 0x19:{
		b; b; str; s; i; i; i; s; s; s; s; s; s; b;
		return instr;}
	case 0x1A:
		ctx->position += 1 + 2;
		return instr;
	case 0x1B:
		ctx->position += 1 + 1;
		return instr;
	case 0x1C:
		ctx->position += 1;
		return instr;
	case 0x1D:
		ctx->position += 1 + 1 + 4;
		return instr;
	case 0x1E:
		ctx->position += 4;
		return instr;
	case 0x1F:
		ctx->position += 2 + 2 + 1;
		return instr;
	case 0x20:
		ctx->position += 1 + 1 + 1 + 1 + 4 + 4;
		return instr;
	case 0x21:
		ctx->position += 1 + 1 + 4 + 4;
		return instr;
	case 0x22:
		b; b; instr.targetOffset = s; b;
		return instr;
	case 0x23:
		ctx->position += 1 + 1;
		return instr;
	case 0x24:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x25:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x26:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x27:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x28:
		b; str; i;
		return instr;
	case 0x29:
		ctx->position += 1;
		return instr;
	case 0x2A:
		str; i;
		return instr;
	case 0x2B: // no operands
		return instr;
	case 0x2C:
		ctx->position += 1 + 2 + 2;
		return instr;
	case 0x2D:
		ctx->position += 1;
		return instr;
	case 0x2E:
		ctx->position += 1 + 4 + 4 + 4;
		return instr;
	case 0x2F:
		ctx->position += 1 + 1;
		return instr;
	case 0x30:{
		b;
		do
		{
			char* string = str;
			if (*string == 0)
			{
				break;
			}
		}
		while (1);
		break;}
	case 0x31:
		ctx->position += 1 + 4;
		return instr;
	case 0x32:
		ctx->position += 1 + 1;
		return instr;
	case 0x33:
		ctx->position += 1 + 1;
		return instr;
	case 0x34: // no operands
		return instr;
	case 0x35:
		ctx->position += 1 + 4 + 4 + 4 + 4;
		return instr;
	case 0x36:
		ctx->position += 4 + 4 + 4 + 4;
		return instr;
	case 0x37:
		ctx->position += 4 + 4 + 4 + 4;
		return instr;
	case 0x38:
		ctx->position += 4 + 4 + 4 + 4;
		return instr;
	case 0x39:
		ctx->position += 4 + 4;
		return instr;
	case 0x3A:
		ctx->position += 4 + 4;
		return instr;
	case 0x3B:
		ctx->position += 4 + 4;
		return instr;
	case 0x3C:
		ctx->position += 2 + 4;
		return instr;
	case 0x3D:
		ctx->position += 4 + 4 + 4 + 4;
		return instr;
	case 0x3E:
		ctx->position += 4 + 4;
		return instr;
	case 0x3F:
		ctx->position += 1;
		return instr;
	case 0x40:
		ctx->position += 1;
		return instr;
	case 0x41:
		ctx->position += 1;
		return instr;
	case 0x42:
		ctx->position += 1 + 4 + 1;
		return instr;
	case 0x43:
		ctx->position += 1 + 4 + 4;
		return instr;
	case 0x44:
		ctx->position += 1 + 4 + 4;
		return instr;
	case 0x45:
		ctx->position += 1 + 4;
		return instr;
	case 0x46:
		ctx->position += 1 + 4 + 4;
		return instr;
	case 0x47:
		ctx->position += 1;
		return instr;
	case 0x48:
		ctx->position += 1 + 4;
		return instr;
	case 0x49:
		ctx->position += 1 + 2;
		return instr;
	case 0x4A:
		ctx->position += 1;
		return instr;
	case 0x4B:
		b; b; i; instr.targetOffset = s;
		return instr;
	case 0x4C:
		instr.targetOffset = s;
		return instr;
	case 0x4D: // no operands
		return instr;
	case 0x4E: // no operands
		return instr;
	case 0x4F:
		ctx->position += 1 + 1;
		return instr;
	case 0x50:
		instr.targetOffset = s;
		return instr;
	case 0x51: // no operands
		return instr;
	case 0x52:
		ctx->position += 1;
		return instr;
	case 0x53:
		ctx->position += 1;
		return instr;
	case 0x54:
		ctx->position += 1;
		return instr;
	case 0x55:
		ctx->position += 2;
		return instr;
	case 0x56: // no operands
		return instr;
	case 0x57:
		ctx->position += 1 + 1;
		return instr;
	case 0x58:
		ctx->position += 1;
		return instr;
	case 0x59:
		ctx->position += 1 + 2;
		return instr;
	case 0x5A:
		ctx->position += 1 + 1 + 4;
		return instr;
	case 0x5B:
		ctx->position += 4;
		return instr;
	case 0x5C:
		ctx->position += 1 + 4;
		return instr;
	case 0x5D:
		ctx->position += 1 + 4;
		return instr;
	case 0x5E:
		ctx->position += 1;
		return instr;
	case 0x5F:
		ctx->position += 1 + 1;
		return instr;
	case 0x60:
		ctx->position += 1;
		return instr;
	case 0x61:
		ctx->position += 4;
		return instr;
	case 0x62:
		ctx->position += 1 + 1 + 1 + 1 + 2;
		return instr;
	case 0x63:
		ctx->position += 1 + 4;
		return instr;
	case 0x64:
		ctx->position += 2;
		return instr;
	case 0x65:
		ctx->position += 2 + 1;
		return instr;
	case 0x66:
		ctx->position += 2;
		return instr;
	case 0x67:
		str;
		return instr;
	case 0x68: // no operands
		return instr;
	case 0x69: // no operands
		return instr;
	case 0x6A:
		ctx->position += 1 + 4 + 4;
		return instr;
	case 0x6B: // no operands
		return instr;
	case 0x6C: // no operands
		return instr;
	case 0x6D:
		ctx->position += 4;
		return instr;
	case 0x6E:
		ctx->position += 4;
		return instr;
	case 0x6F:
		ctx->position += 1 + 1;
		return instr;
	case 0x70:
		ctx->position += 1 + 1 + 2 + 2;
		return instr;
	case 0x71:
		ctx->position += 1;
		return instr;
	case 0x72:
		ctx->position += 1;
		return instr;
	case 0x73:
		ctx->position += 1;
		return instr;
	case 0x74:
		ctx->position += 1;
		return instr;
	case 0x75:
		ctx->position += 2;
		return instr;
	case 0x76:
		ctx->position += 1;
		return instr;
	case 0x77:
		ctx->position += 1;
		return instr;
	case 0x78:
		ctx->position += 1;
		return instr;
	case 0x79:
		ctx->position += 1;
		return instr;
	case 0x7A:
		ctx->position += 1;
		return instr;
	case 0x7B:
		ctx->position += 2;
		return instr;
	case 0x7C:
		ctx->position += 1 + 1;
		return instr;
	case 0x7D:
		ctx->position += 1 + 4;
		return instr;
	case 0x7E:
		ctx->position += 4;
		return instr;
	case 0x7F:
		ctx->position += 4 + 4 + 4 + 1 + 4;
		return instr;
	case 0x80:
		ctx->position += 4;
		return instr;
	case 0x81:
		ctx->position += 1 + 1 + 2;
		return instr;
	case 0x82: // no operands
		return instr;
	case 0x83:
		ctx->position += 1;
		return instr;
	case 0x84:
		ctx->position += 1 + 2 + 2 + 2 + 4 + 1;
		return instr;
	case 0x85:
		ctx->position += 1 + 1 + 4;
		return instr;
	case 0x86:
		ctx->position += 2 + 2 + 2 + 1 + 4;
		return instr;
	case 0x87:
		ctx->position += 2 + 1;
		return instr;
	case 0x88:
		ctx->position += 2;
		return instr;
	case 0x89:
		ctx->position += 1;
		return instr;
	case 0x8A:
		ctx->position += 1 + 1;
		return instr;
	case 0x8B: // no operands
		return instr;
	case 0x8C: // no operands
		return instr;
	case 0x8D:
		ctx->position += 1 + 4 + 4 + 4 + 4;
		return instr;
	case 0x8E:{
		uint8 op1 = b;
		if (op1 == 0x01)
		{
			b; str;
		}
		else if (op1 == 0x0D)
		{
			b; i; i; i; i; i;
		}
		else
		{
			b; i; i; i; i;
		}
		return instr;
	}
	case 0x8F:
		ctx->position += 1;
		return instr;
	case 0x90:
		ctx->position += 1;
		return instr;
	case 0x91:
		ctx->position += 1;
		return instr;
	case 0x92:
		ctx->position += 1 + 1 + 4 + 4 + 4 + 2 + 4;
		return instr;
	case 0x93:
		b; b; str;
		return instr;
	case 0x94:
		b; str; i;
		return instr;
	case 0x95: // no operands
		return instr;
	case 0x96:
		b; str; s;
		return instr;
	case 0x97:
		ctx->position += 4 + 2 + 2;
		return instr;
	case 0x98:
		ctx->position += 1 + 1 + 4 + 4;
		return instr;
	case 0x99:
		ctx->position += 1;
		return instr;
	case 0x9A:
		ctx->position += 4;
		return instr;
	case 0x9B:
		ctx->position += 1;
		return instr;
	case 0x9C:
		ctx->position += 1;
		return instr;
	case 0x9D:
		ctx->position += 1;
		return instr;
	case 0x9E:
		ctx->position += 1;
		return instr;
	case 0x9F:
		ctx->position += 1 + 4;
		return instr;
	case 0xA0:
		b; i; s; str;
		return instr;
	case 0xA1:
		ctx->position += 1 + 4;
		return instr;
	case 0xA2:
		ctx->position += 1;
		return instr;
	case 0xA3:
		ctx->position += 1 + 1;
		return instr;
	case 0xA4:{
		uint8 op1 = b;
		if (op1 == 0x02)
		{
			s;
		}
		else if (op1 == 0x00)
		{
		}
		else if (op1 == 0x01)
		{
		}
		else
		{
			printf("%ls tried to read invalid opcode 0x%02X%02X at offset 0x%X\n", ctx->filename.c_str(), instr.opcode, op1, instr.offset);
			throw (69);
		}
		return instr;}
	case 0xA5:
		ctx->position += 1 + 1 + 4 + 4 + 1;
		return instr;
	case 0xA6:
		ctx->position += 1 + 1 + 1 + 4 + 4 + 4 + 4;
		return instr;
	case 0xA7:
		ctx->position += 1 + 2;
		return instr;
	case 0xA8:
		ctx->position += 1 + 1;
		return instr;
	case 0xA9:
		ctx->position += 4;
		return instr;
	case 0xAA:
		ctx->position += 4 + 4;
		return instr;
	case 0xAB:
		ctx->position += 1 + 1 + 1;
		return instr;
	case 0xAC:
		ctx->position += 1 + 1 + 4 + 4 + 1;
		return instr;
	case 0xAD:
		ctx->position += 1 + 1 + 4;
		return instr;
	case 0xAE:
		ctx->position += 2 + 4;
		return instr;
	case 0xAF:
		ctx->position += 1 + 1 + 4 + 4 + 4;
		return instr;
	case 0xB0:
		ctx->position += 1 + 2;
		return instr;
	case 0xB1:
		ctx->position += 1 + 2;
		return instr;
	default:
		printf("first_pass: %ls tried to read invalid opcode %X at offset 0x%X\n", ctx->filename.c_str(), instr.opcode, instr.offset);
		throw(69);
	}
	return instr;
#undef b
#undef s
#undef i
#undef str
#undef txt
}