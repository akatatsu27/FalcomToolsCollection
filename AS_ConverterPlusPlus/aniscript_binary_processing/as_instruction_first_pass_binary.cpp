#include "../as_instruction.h"
#include "../aniscript.h"

instruction instruction::first_pass_binary(binary_context *const ctx, aniscript* const ani)
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
	case 0x02:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x03:{
		uint8 target = b; uint16 op2 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x04:{
		uint8 target = b; uint8 op2 = b; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x05:
		ctx->position += as_05::size;
		return instr;
	case 0x06:
		ctx->position += sleep::size;
		return instr;
	case 0x07: // no operands
		return instr;
	case 0x08:{
		uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
		ani->targets_in_file.insert(target1);
		ani->targets_in_file.insert(target2);
		return instr;}
	case 0x09:
		ctx->position += as_09::size;
		return instr;
	case 0x0A:
		ctx->position += as_0a::size;
		return instr;
	case 0x0B:{
		uint8 target = b; uint8 op2 = b; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x0C:
		ctx->position += as_0c::size;
		return instr;
	case 0x0D:{
		uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint16 op6 = s; uint16 op7 = s;
		ani->targets_in_file.insert(target1);
		ani->targets_in_file.insert(target2);
		return instr;}
	case 0x0E:
		ctx->position += drop_down::size;
		return instr;
	case 0x0F:
		ctx->position += jump_to_target::size;
		return instr;
	case 0x10:
		ctx->position += jump_back::size;
		return instr;
	case 0x11:{
		uint8 target = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i; uint8 op7 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x12:
		s; str;
		return instr;
	case 0x13:
		ctx->position += release_effect::size;
		return instr;
	case 0x14:
		ctx->position += as_14::size;
		return instr;
	case 0x15:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x16:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x17:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x18:{
		uint8 op1 = b; uint8 op2 = b; uint8 target = b; uint16 op4 = s; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        uint16 op8 = s; uint16 op9 = s; uint16 op10 = s; uint32 op11 = s; uint16 op12 = s; uint32 op13 = s; uint8 op14 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x19:{
		b; b; str; s; i; i; i; s; s; s; s; s; s; b;
		return instr;}
	case 0x1A:
		ctx->position += as_1a::size;
		return instr;
	case 0x1B:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x1C:
		ctx->position += damage::size;
		return instr;
	case 0x1D:
		ctx->position += damage_anime::size;
		return instr;
	case 0x1E:
		ctx->position += as_1e::size;
		return instr;
	case 0x1F:
		ctx->position += as_1f::size;
		return instr;
	case 0x20:
		ctx->position += as_20::size;
		return instr;
	case 0x21:
		ctx->position += as_21::size;
		return instr;
	case 0x22:{
		uint8 target = b; uint8 op2 = b; instr.targetOffset = s; uint8 op4 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x23:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x24:{
		uint8 op1 = b; uint8 target = b; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x25:{
		uint8 op1 = b; uint8 target = b; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x26:
		ctx->position += as_26::size;
		return instr;
	case 0x27:
		ctx->position += as_27::size;
		return instr;
	case 0x28:
		b; str; i;
		return instr;
	case 0x29:
		ctx->position += as_29::size;
		return instr;
	case 0x2A:
		str; i;
		return instr;
	case 0x2B: // no operands
		return instr;
	case 0x2C:{
		uint8 target = b; uint16 op2 = s; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x2D:{
		uint8 target = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x2E:{
		uint8 target = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x2F:
		ctx->position += suspend_thread::size;
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
		ctx->position += as_31::size;
		return instr;
	case 0x32:
		ctx->position += as_32::size;
		return instr;
	case 0x33:
		ctx->position += as_33::size;
		return instr;
	case 0x34: // no operands
		return instr;
	case 0x35:
		ctx->position += keep_angle::size;
		return instr;
	case 0x36:
		ctx->position += as_36::size;
		return instr;
	case 0x37:
		ctx->position += rotation_angle::size;
		return instr;
	case 0x38:
		ctx->position += rotation_angle_v::size;
		return instr;
	case 0x39:
		ctx->position += set_angle::size;
		return instr;
	case 0x3A:
		ctx->position += tilt_angle::size;
		return instr;
	case 0x3B:
		ctx->position += rotation_angle_h::size;
		return instr;
	case 0x3C:
		ctx->position += as_3c::size;
		return instr;
	case 0x3D:
		ctx->position += shake_screen::size;
		return instr;
	case 0x3E:
		ctx->position += as_3e::size;
		return instr;
	case 0x3F:
		ctx->position += as_3f::size;
		return instr;
	case 0x40:
		ctx->position += as_40::size;
		return instr;
	case 0x41:{
		uint8 target = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x42:
		ctx->position += as_42::size;
		return instr;
	case 0x43:
		ctx->position += set_bk_color::size;
		return instr;
	case 0x44:
		ctx->position += zoom_angle::size;
		return instr;
	case 0x45:
		ctx->position += as_45::size;
		return instr;
	case 0x46:
		ctx->position += as_46::size;
		return instr;
	case 0x47:
		ctx->position += as_47::size;
		return instr;
	case 0x48:
		ctx->position += as_48::size;
		return instr;
	case 0x49:
		ctx->position += set_control::size;
		return instr;
	case 0x4A:
		ctx->position += as_4a::size;
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
		ctx->position += as_4f::size;
		return instr;
	case 0x50:
		instr.targetOffset = s;
		return instr;
	case 0x51: // no operands
		return instr;
	case 0x52:
		ctx->position += as_52::size;
		return instr;
	case 0x53:
		ctx->position += as_53::size;
		return instr;
	case 0x54:
		ctx->position += as_54::size;
		return instr;
	case 0x55:
		ctx->position += magic_cast_begin::size;
		return instr;
	case 0x56: // no operands
		return instr;
	case 0x57:
		ctx->position += as_57::size;
		return instr;
	case 0x58:
		ctx->position += beat_back::size;
		return instr;
	case 0x59:
		ctx->position += as_59::size;
		return instr;
	case 0x5A:
		ctx->position += as_5a::size;
		return instr;
	case 0x5B:
		ctx->position += as_5b::size;
		return instr;
	case 0x5C:{
		uint8 target = b; uint32 op2 = i;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x5D:{
		uint8 target = b; uint32 op2 = i;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x5E:
		ctx->position += as_5e::size;
		return instr;
	case 0x5F:{
		uint8 target = b; uint8 op2 = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x60:
		ctx->position += as_60::size;
		return instr;
	case 0x61:
		ctx->position += set_battle_speed::size;
		return instr;
	case 0x62:
		ctx->position += as_62::size;
		return instr;
	case 0x63:
		ctx->position += as_63::size;
		return instr;
	case 0x64:
		ctx->position += sound_effect::size;
		return instr;
	case 0x65:
		ctx->position += sound_effect_ex::size;
		return instr;
	case 0x66:
		ctx->position += as_66::size;
		return instr;
	case 0x67:
		str;
		return instr;
	case 0x68: // no operands
		return instr;
	case 0x69: // no operands
		return instr;
	case 0x6A:
		ctx->position += load_schip::size;
		return instr;
	case 0x6B: // no operands
		return instr;
	case 0x6C: // no operands
		return instr;
	case 0x6D:
		ctx->position += as_6d::size;
		return instr;
	case 0x6E:
		ctx->position += as_6e::size;
		return instr;
	case 0x6F:
		ctx->position += as_6f::size;
		return instr;
	case 0x70:
		ctx->position += as_70::size;
		return instr;
	case 0x71:
		ctx->position += as_71::size;
		return instr;
	case 0x72:
		ctx->position += as_72::size;
		return instr;
	case 0x73:
		ctx->position += as_73::size;
		return instr;
	case 0x74:
		ctx->position += as_74::size;
		return instr;
	case 0x75:
		ctx->position += as_75::size;
		return instr;
	case 0x76:
		ctx->position += as_76::size;
		return instr;
	case 0x77:
		ctx->position += as_77::size;
		return instr;
	case 0x78:
		ctx->position += set_eff_state::size;
		return instr;
	case 0x79:
		ctx->position += nop_79::size;
		return instr;
	case 0x7A:
		ctx->position += craft_end::size;
		return instr;
	case 0x7B:
		ctx->position += set_craft_end_flag::size;
		return instr;
	case 0x7C:
		ctx->position += as_7c::size;
		return instr;
	case 0x7D:
		ctx->position += as_7d::size;
		return instr;
	case 0x7E:
		ctx->position += as_7e::size;
		return instr;
	case 0x7F:
		ctx->position += blur_screen::size;
		return instr;
	case 0x80:
		ctx->position += as_80::size;
		return instr;
	case 0x81:
		ctx->position += as_81::size;
		return instr;
	case 0x82: // no operands
		return instr;
	case 0x83:
		ctx->position += sort_target::size;
		return instr;
	case 0x84:
		ctx->position += rotate_char::size;
		return instr;
	case 0x85:
		ctx->position += as_85::size;
		return instr;
	case 0x86:
		ctx->position += as_86::size;
		return instr;
	case 0x87:
		ctx->position += as_87::size;
		return instr;
	case 0x88:
		ctx->position += voice::size;
		return instr;
	case 0x89:{
		const unsigned char target = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x8A:
		ctx->position += clone::size;
		return instr;
	case 0x8B: // no operands
		return instr;
	case 0x8C: // no operands
		return instr;
	case 0x8D:
		ctx->position += zoom::size;
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
		ctx->position += as_8f::size;
		return instr;
	case 0x90:
		ctx->position += as_90::size;
		return instr;
	case 0x91:
		ctx->position += as_91::size;
		return instr;
	case 0x92:
		ctx->position += as_92::size;
		return instr;
	case 0x93:
		b; b; str;
		return instr;
	case 0x94:
		b; str; i;
		return instr;
	case 0x95: // no operands
		return instr;
	case 0x96:{
		const uint8 target = b; char *op2 = str; uint16 op3 = s;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x97:
		ctx->position += move_angle::size;
		return instr;
	case 0x98:
		ctx->position += as_98::size;
		return instr;
	case 0x99:
		ctx->position += as_99::size;
		return instr;
	case 0x9A:
		ctx->position += as_9a::size;
		return instr;
	case 0x9B:
		ctx->position += as_9b::size;
		return instr;
	case 0x9C:{
		const uint8 target = b;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0x9D:
		ctx->position += as_9d::size;
		return instr;
	case 0x9E:
		ctx->position += set_timer::size;
		return instr;
	case 0x9F:
		ctx->position += set_battle_mode::size;
		return instr;
	case 0xA0:
		b; i; s; str;
		return instr;
	case 0xA1:
		ctx->position += as_a1::size;
		return instr;
	case 0xA2:
		ctx->position += as_a2::size;
		return instr;
	case 0xA3:
		ctx->position += as_a3::size;
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
		ctx->position += as_a5::size;
		return instr;
	case 0xA6:
		ctx->position += as_a6::size;
		return instr;
	case 0xA7:
		ctx->position += battle_effect_end::size;
		return instr;
	case 0xA8:
		ctx->position += damage_voice::size;
		return instr;
	case 0xA9:
		ctx->position += nop_a9::size;
		return instr;
	case 0xAA:
		ctx->position += as_aa::size;
		return instr;
	case 0xAB:{
		uint8 op1 = b; uint8 target = b; b;
		if(op1 == 0 || op1 == 1) i;
		ani->targets_in_file.insert(target);
		return instr;}
	case 0xAC:
		ctx->position += as_ac::size;
		return instr;
	case 0xAD:
		ctx->position += as_ad::size;
		return instr;
	case 0xAE:
		ctx->position += as_ae::size;
		return instr;
	case 0xAF:
		ctx->position += as_af::size;
		return instr;
	case 0xB0:
		ctx->position += as_b0::size;
		return instr;
	case 0xB1:
		ctx->position += as_b1::size;
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