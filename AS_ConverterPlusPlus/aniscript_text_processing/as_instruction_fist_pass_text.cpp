#include "../aniscript.h"
#include "../as_instruction.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

instruction instruction::first_pass_text(std::list<string>::iterator &line, char *name, int len, size_t &cur_offset)
{
	uint32 hashed = hash(name, len);
	instruction instr;
	instr.offset = cur_offset++;
	instr.line = line;
	instr.name_hash = hashed;
	static char* args[20];
	static char* name_check;
	switch (hashed)
	{
	case end_event::hash:
		cur_offset += end_event::size;
		return instr;
	case go_to::hash:
		cur_offset += go_to::size;
		return instr;
	case select_sub_chip::hash:
		cur_offset += select_sub_chip::size;
		return instr;
	case deg::hash:
		cur_offset += deg::size;
		return instr;
	case as_04::hash:
		cur_offset += as_04::size;
		return instr;
	case as_05::hash:
		cur_offset += as_05::size;
		return instr;
	case sleep::hash:
		cur_offset += sleep::size;
		return instr;
	case update::hash:
		cur_offset += update::size;
		return instr;
	case teleport::hash:
		cur_offset += teleport::size;
		return instr;
	case as_09::hash:
		cur_offset += as_09::size;
		return instr;
	case as_0a::hash:
		cur_offset += as_0a::size;
		return instr;
	case turn::hash:
		cur_offset += turn::size;
		return instr;
	case as_0c::hash:
		cur_offset += as_0c::size;
		return instr;
	case jump::hash:
		cur_offset += jump::size;
		return instr;
	case drop_down::hash:
		cur_offset += drop_down::size;
		return instr;
	case jump_to_target::hash:
		cur_offset += jump_to_target::size;
		return instr;
	case jump_back::hash:
		cur_offset += jump_back::size;
		return instr;
	case move::hash:
		cur_offset += move::size;
		return instr;
	case hash("add_effect"):{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 2, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[1][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"add_effect\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 2 + (strlen(args[1]) - 2 + 1); //exclude beginning and end ", but add null byte to the length
		return instr;}
	case release_effect::hash:
		cur_offset += release_effect::size;
		return instr;
	case as_14::hash:
		cur_offset += as_14::size;
		return instr;
	case wait_effect::hash:
		cur_offset += wait_effect::size;
		return instr;
	case finish_effect::hash:
		cur_offset += finish_effect::size;
		return instr;
	case cancel_effect::hash:
		cur_offset += cancel_effect::size;
		return instr;
	case show_effect::hash:
		cur_offset += show_effect::size;
		return instr;
	case hash("show_3d_effect"):{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 14, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[2][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"show_3d_effect\" third operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + 1 + (strlen(args[2]) -2 + 1) + 2 + 4 + 4 + 4 + 2 + 2 + 2 + 2 + 2 + 2 + 1;
		return instr;}
	case as_1a::hash:
		cur_offset += as_1a::size;
		return instr;
	case select_chip::hash:
		cur_offset += select_chip::size;
		return instr;
	case damage::hash:
		cur_offset += damage::size;
		return instr;
	case damage_anime::hash:
		cur_offset += damage_anime::size;
		return instr;
	case as_1e::hash:
		cur_offset += as_1e::size;
		return instr;
	case as_1f::hash:
		cur_offset += as_1f::size;
		return instr;
	case as_20::hash:
		cur_offset += as_20::size;
		return instr;
	case as_21::hash:
		cur_offset += as_21::size;
		return instr;
	case begin_thread::hash:
		cur_offset += begin_thread::size;
		return instr;
	case wait_thread::hash:
		cur_offset += wait_thread::size;
		return instr;
	case set_chip_mode_flag::hash:
		cur_offset += set_chip_mode_flag::size;
		return instr;
	case clear_chip_mode_flag::hash:
		cur_offset += clear_chip_mode_flag::size;
		return instr;
	case as_26::hash:
		cur_offset += as_26::size;
		return instr;
	case as_27::hash:
		cur_offset += as_27::size;
		return instr;
	case hash("char_say"):{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[1][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"char_say\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + (strlen(args[1]) - 2 + 1) + 4; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case as_29::hash:
		cur_offset += as_29::size;
		return instr;
	case hash("tip_text"):{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 2, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[0][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"tip_text\" first operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += (strlen(args[0]) - 2 + 1) + 4; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case as_2b::hash:
		cur_offset += as_2b::size;
		return instr;
	case shadow_begin::hash:
		cur_offset += shadow_begin::size;
		return instr;
	case shadow_end::hash:
		cur_offset += shadow_end::hash;
		return instr;
	case shake_char::hash:
		cur_offset += shake_char::size;
		return instr;
	case suspend_thread::hash:
		cur_offset += suspend_thread::size;
		return instr;
	case hash("as_30"):{
		auto copy = *line;
		size_t cnt = boost::count(copy, ',') + 1;
		if(!parse_assembly_instruction(copy, &name_check, cnt, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		size_t total_len = 1;
		for(int i = 1; i < cnt; i++)
		{
			if(args[i][0] != '\"')
			{
				instr.opcode = 0xFF;
				printf("[ERROR] \"as_30\" all operands except the first must be strings!\n\t%s", line->data());
				return instr;
			}
			total_len += (strlen(args[i]) -2 + 1);
		}
		cur_offset += total_len;
		return instr;}
	case as_31::hash:
		cur_offset += as_31::size;
		return instr;
	case as_32::hash:
		cur_offset += as_32::size;
		return instr;
	case as_33::hash:
		cur_offset += as_33::size;
		return instr;
	case as_34::hash:
		cur_offset += as_34::size;
		return instr;
	case keep_angle::hash:
		cur_offset += keep_angle::size;
		return instr;
	case as_36::hash:
		cur_offset += as_36::size;
		return instr;
	case rotation_angle::hash:
		cur_offset += rotation_angle::size;
		return instr;
	case rotation_angle_v::hash:
		cur_offset += rotation_angle_v::size;
		return instr;
	case set_angle::hash:
		cur_offset += set_angle::size;
		return instr;
	case tilt_angle::hash:
		cur_offset += tilt_angle::size;
		return instr;
	case rotation_angle_h::hash:
		cur_offset += rotation_angle_h::size;
		return instr;
	case as_3c::hash:
		cur_offset += as_3c::size;
		return instr;
	case shake_screen::hash:
		cur_offset += shake_screen::size;
		return instr;
	case as_3e::hash:
		cur_offset += as_3e::size;
		return instr;
	case as_3f::hash:
		cur_offset += as_3f::size;
		return instr;
	case as_40::hash:
		cur_offset += as_40::size;
		return instr;
	case lock_angle::hash:
		cur_offset += lock_angle::size;
		return instr;
	case as_42::hash:
		cur_offset += as_42::size;
		return instr;
	case set_bk_color::hash:
		cur_offset += set_bk_color::size;
		return instr;
	case zoom_angle::hash:
		cur_offset += zoom_angle::size;
		return instr;
	case as_45::hash:
		cur_offset += as_45::size;
		return instr;
	case as_46::hash:
		cur_offset += as_46::size;
		return instr;
	case as_47::hash:
		cur_offset += as_47::size;
		return instr;
	case as_48::hash:
		cur_offset += as_48::size;
		return instr;
	case set_control::hash:
		cur_offset += set_control::size;
		return instr;
	case as_4a::hash:
		cur_offset += as_4a::size;;
		return instr;
	case random::hash:
		cur_offset += random::size;
		return instr;
	case loop_target_beg::hash:
		cur_offset += loop_target_beg::size;
		return instr;
	case reset_loop_target::hash:
		cur_offset += reset_loop_target::size;
		return instr;
	case loop_target_end::hash:
		cur_offset += loop_target_end::size;
		return instr;
	case as_4f::hash:
		cur_offset += as_4f::size;
		return instr;
	case call::hash:
		cur_offset += call::size;
		return instr;
	case ret::hash:
		cur_offset += ret::size;
		return instr;
	case as_52::hash:
		cur_offset += as_52::size;
		return instr;
	case as_53::hash:
		cur_offset += as_53::size;
		return instr;
	case as_54::hash:
		cur_offset += as_54::size;
		return instr;
	case magic_cast_begin::hash:
		cur_offset += magic_cast_begin::size;
		return instr;
	case magic_cast_end::hash:
		cur_offset += magic_cast_end::size;
		return instr;
	case as_57::hash:
		cur_offset += as_57::size;
		return instr;
	case beat_back::hash:
		cur_offset += beat_back::size;
		return instr;
	case as_59::hash:
		cur_offset += as_59::size;
		return instr;
	case as_5a::hash:
		cur_offset += as_5a::size;
		return instr;
	case as_5b::hash:
		cur_offset += as_5b::size;
		return instr;
	case show::hash:
		cur_offset += show::size;
		return instr;
	case hide::hash:
		cur_offset += hide::size;
		return instr;
	case as_5e::hash:
		cur_offset += as_5e::size;
		return instr;
	case as_5f::hash:
		cur_offset += as_5f::size;
		return instr;
	case as_60::hash:
		cur_offset += as_60::size;
		return instr;
	case set_battle_speed::hash:
		cur_offset += set_battle_speed::size;
		return instr;
	case as_62::hash:
		cur_offset += as_62::size;
		return instr;
	case as_63::hash:
		cur_offset += as_63::size;
		return instr;
	case sound_effect::hash:
		cur_offset += sound_effect::size;
		return instr;
	case sound_effect_ex::hash:
		cur_offset += sound_effect_ex::size;
		return instr;
	case as_66::hash:
		cur_offset += as_66::size;
		return instr;
	case hash("scraft_cut_in"):{
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 1, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[0][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"scraft_cut_in\" operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += (strlen(args[0]) - 2 + 1) ; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case nop_68::hash:
		cur_offset += nop_68::size;
		return instr;
	case release_texture::hash:
		cur_offset += release_texture::size;
		return instr;
	case load_schip::hash:
		cur_offset += load_schip::size;
		return instr;
	case reset_scraft_chip::hash:
		cur_offset += reset_scraft_chip::size;
		return instr;
	case die::hash:
		cur_offset += die::size;
		return instr;
	case as_6d::hash:
		cur_offset += as_6d::size;
		return instr;
	case as_6e::hash:
		cur_offset += as_6e::size;
		return instr;
	case as_6f::hash:
		cur_offset += as_6f::size;
		return instr;
	case as_70::hash:
		cur_offset += as_70::size;
		return instr;
	case as_71::hash:
		cur_offset += as_71::size;
		return instr;
	case as_72::hash:
		cur_offset += as_72::size;
		return instr;
	case as_73::hash:
		cur_offset += as_73::size;
		return instr;
	case as_74::hash:
		cur_offset += as_74::size;
		return instr;
	case as_75::hash:
		cur_offset += as_75::size;
		return instr;
	case as_76::hash:
		cur_offset += as_76::size;
		return instr;
	case as_77::hash:
		cur_offset += as_77::size;
		return instr;
	case set_eff_state::hash:
		cur_offset += set_eff_state::size;
		return instr;
	case nop_79::hash:
		cur_offset += nop_79::size;
		return instr;
	case craft_end::hash:
		cur_offset += craft_end::size;
		return instr;
	case set_craft_end_flag::hash:
		cur_offset += set_craft_end_flag::size;
		return instr;
	case as_7c::hash:
		cur_offset += as_7c::size;
		return instr;
	case as_7d::hash:
		cur_offset += as_7d::size;
		return instr;
	case as_7e::hash:
		cur_offset += as_7e::size;
		return instr;
	case blur_screen::hash:
		cur_offset += blur_screen::size;
		return instr;
	case as_80::hash:
		cur_offset += as_80::size;
		return instr;
	case as_81::hash:
		cur_offset += as_81::size;
		return instr;
	case as_82::hash:
		cur_offset += as_82::size;
		return instr;
	case sort_target::hash:
		cur_offset += sort_target::size;
		return instr;
	case rotate_char::hash:
		cur_offset += rotate_char::size;
		return instr;
	case as_85::hash:
		cur_offset += as_85::size;
		return instr;
	case as_86::hash:
		cur_offset += as_86::size;
		return instr;
	case as_87::hash:
		cur_offset += as_87::size;
		return instr;
	case voice::hash:
		cur_offset += voice::size;
		return instr;
	case save_cur_pos::hash:
		cur_offset += save_cur_pos::size;
		return instr;
	case clone::hash:
		cur_offset += clone::size;
		return instr;
	case use_item_begin::hash:
		cur_offset += use_item_begin::size;
		return instr;
	case use_item_end::hash:
		cur_offset += use_item_end::size;
		return instr;
	case zoom::hash:
		cur_offset += zoom::size;
		return instr;
	case hash("load_x_file"):
		cur_offset += 1; //2 byte opcode
		{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 2, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[1][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"load_x_file\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + (strlen(args[1]) - 2 + 1);
		return instr;}
	case as_8e0d::hash: //2 byte opcode
		cur_offset += as_8e0d::size;
		return instr;
	case as_8e::hash:
		cur_offset += as_8e::size;
		return instr;
	case as_8f::hash:
		cur_offset += as_8f::size;
		return instr;
	case as_90::hash:
		cur_offset += as_90::size;
		return instr;
	case as_91::hash:
		cur_offset += as_91::size;
		return instr;
	case as_92::hash:
		cur_offset += as_92::size;
		return instr;
	case hash("as_93"):{
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[2][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"as_93\" third operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + 1 + (strlen(args[0]) - 2 + 1) ; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case hash("as_94"):{
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[1][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"as_94\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + (strlen(args[1]) - 2 + 1) + 4; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case as_95::hash:
		cur_offset += as_95::size;
		return instr;
	case hash("set_angle_target"):{
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[1][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"set_angle_target\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + (strlen(args[1]) - 2 + 1) + 2; //exclude beginning and end ", but add null byte to the length
		return instr;}
	case move_angle::hash:
		cur_offset += move_angle::size;
		return instr;
	case as_98::hash:
		cur_offset += as_98::size;
		return instr;
	case as_99::hash:
		cur_offset += as_99::size;
		return instr;
	case as_9a::hash:
		cur_offset += as_9a::size;
		return instr;
	case as_9b::hash:
		cur_offset += as_9b::size;
		return instr;
	case reset_chip_status::hash:
		cur_offset += reset_chip_status::size;
		return instr;
	case as_9d::hash:
		cur_offset += as_9d::size;
		return instr;
	case set_timer::hash:
		cur_offset += set_timer::size;
		return instr;
	case set_battle_mode::hash:
		cur_offset += set_battle_mode::size;
		return instr;
	case hash("as_a0"):{
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 4, args))
		{
			instr.opcode = 0xFF;
			return instr;
		}
		if(args[3][0] != '\"')
		{
			instr.opcode = 0xFF;
			printf("[ERROR] \"as_a0\" second operand must be a string!\n\t%s", line->data());
			return instr;
		}
		cur_offset += 1 + 4 + 2 + (strlen(args[3]) - 2 + 1); //exclude beginning and end ", but add null byte to the length
		return instr;}
	case as_a1::hash:
		cur_offset += as_a1::size;
		return instr;
	case as_a2::hash:
		cur_offset += as_a2::size;
		return instr;
	case as_a3::hash:
		cur_offset += as_a3::size;
		return instr;
	case as_a402::hash: // 2 byte opcode
		cur_offset += as_a402::size;
		return instr;
	case as_a400::hash: // 2 byte opcode
		cur_offset += as_a400::size;
		return instr;
	case as_a401::hash: // 2 byte opcode
		cur_offset += as_a401::size;
		return instr;
	case as_a5::hash:
		cur_offset += as_a5::size;
		return instr;
	case as_a6::hash:
		cur_offset += as_a6::size;
		return instr;
	case battle_effect_end::hash:
		cur_offset += battle_effect_end::size;
		return instr;
	case damage_voice::hash:
		cur_offset += damage_voice::size;
		return instr;
	case nop_a9::hash:
		cur_offset += nop_a9::size;
		return instr;
	case as_aa::hash:
		cur_offset += as_aa::size;
		return instr;
	case as_ab::hash:
		cur_offset += as_ab::size;
		return instr;
	case as_ac::hash:
		cur_offset += as_ac::size;
		return instr;
	case as_ad::hash:
		cur_offset += as_ad::size;
		return instr;
	case as_ae::hash:
		cur_offset += as_ae::size;
		return instr;
	case as_af::hash:
		cur_offset += as_af::size;
		return instr;
	case as_b0::hash:
		cur_offset += as_b0::size;
		return instr;
	case as_b1::hash:
		cur_offset += as_b1::size;
		return instr;
	default:
		printf("[ERROR] invalid as_instruction name:\t%.*s", len, name);
		instr.opcode = 0xFF;
		return instr;
	}
}