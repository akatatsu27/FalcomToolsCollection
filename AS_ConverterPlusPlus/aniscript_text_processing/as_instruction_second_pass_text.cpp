#include "../as_instruction.h"
#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool instruction::second_pass_text(aniscript* const ani)
{
	auto u8 = [&](uint8 val) {ani->u8(val);};
	auto u16 = [&](uint16 val) {ani->u16(val);};
	static char* args[20];
	static char* name_check;
	unsigned long long num;
	#define b(x) num = aniscript::num_from_str(args[x], 1);\
	if(num == ULLONG_MAX) return true;\
	ani->u8((uint8)num);
	#define s(x) num = aniscript::num_from_str(args[x], 2);\
	if(num == ULLONG_MAX) return true;\
	ani->u16((uint16)num);
	#define i(x) num = aniscript::num_from_str(args[x], 4);\
	if(num == ULLONG_MAX) return true;\
	ani->u32((uint32)num);
	#define str(x) {for(size_t index = 1; args[x][index] != '\"'; index++)\
	ani->u8(args[x][index]); ani->u8(0x00);}
	switch (this->name_hash)
	{
	case end_event::hash: //no operands
		u8(end_event::opcode);
		return false;
	case go_to::hash:{
    	u8(go_to::opcode);
        if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
        if(args[0][0] != '$')
        {
            printf("[ERROR] %s missing $ in label operand: %s", go_to::name, args[0]);
            return true;
        }
        std::string jmp_target = std::string((const char*)(args[0] + 1));
        auto it = ani->label_to_offset_map.find(jmp_target);
        if(it == ani->label_to_offset_map.end())
        {
            printf("[ERROR] %s undefined label: %s", go_to::name, jmp_target.data());
            return true;
        }
        u16(it->second);
		return false;}
	case select_sub_chip::hash:
    	u8(select_sub_chip::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case deg::hash:
    	u8(deg::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case as_04::hash:
    	u8(as_04::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case as_05::hash:
    	u8(as_05::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case sleep::hash:
    	u8(sleep::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case update::hash:
    	u8(update::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case teleport::hash:
    	u8(teleport::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) i(2) i(3) i(4)
		return false;
	case as_09::hash:
    	u8(as_09::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) i(2) i(3) i(4)
		return false;
	case as_0a::hash:
    	u8(as_0a::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1) b(2) i(3)
		return false;
	case turn::hash:
    	u8(turn::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case as_0c::hash:
    	u8(as_0c::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) s(2) s(3) b(4)
		return false;
	case jump::hash:
    	u8(jump::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 7, args))
            return true;
		b(0) b(1) i(2) i(3) i(4) s(5) s(6)
		return false;
	case drop_down::hash:
    	u8(drop_down::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 6, args))
            return true;
		b(0) i(1) i(2) i(3) s(4) s(5)
		return false;
	case jump_to_target::hash:
    	u8(jump_to_target::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) s(1)
		return false;
	case jump_back::hash:
    	u8(jump_back::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) s(1)
		return false;
	case move::hash:
    	u8(move::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 7, args))
            return true;
		b(0) b(1) i(2) i(3) i(4) i(5) b(6)
		return false;
	case hash("add_effect"):{
    	u8(0x12);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) str(1)
		return false;}
	case release_effect::hash:
    	u8(release_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case as_14::hash:
    	u8(as_14::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case wait_effect::hash:
    	u8(wait_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case finish_effect::hash:
    	u8(finish_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case cancel_effect::hash:
    	u8(cancel_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case show_effect::hash:
    	u8(show_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 14, args))
            return true;
		b(0) b(1) b(2) s(3) i(4) i(5) i(6) s(7) s(8) s(9) s(10) s(11) s(12) b(13) 
		return false;
	case hash("show_3d_effect"):{
    	u8(0x19);
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 14, args))
            return true;
		b(0) b(1) str(2) s(3) i(4) i(5) i(6) s(7) s(8) s(9) s(10) s(11) s(12) b(13)
	    return false;}
	case as_1a::hash:
    	u8(as_1a::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case select_chip::hash:
    	u8(select_chip::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case damage::hash:
    	u8(damage::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case damage_anime::hash:
    	u8(damage_anime::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_1e::hash:
    	u8(as_1e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_1f::hash:
    	u8(as_1f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		s(0) s(1) b(2)
		return false;
	case as_20::hash:
    	u8(as_20::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 6, args))
            return true;
		b(0) b(1) b(2) b(3) i(4) i(5)
		return false;
	case as_21::hash:
    	u8(as_21::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1) i(2) i(3)
		return false;
	case begin_thread::hash:{
    	u8(begin_thread::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1)
		if(args[2][0] != '$')
        {
            printf("[ERROR] %s missing $ in label operand: %s", go_to::name, args[2]);
            return true;
        }
        std::string jmp_target = std::string((const char*)(args[2] + 1));
        auto it = ani->label_to_offset_map.find(jmp_target);
        if(it == ani->label_to_offset_map.end())
        {
            printf("[ERROR] %s undefined label: %s", go_to::name, jmp_target.data());
            return true;
        }
        u16(it->second); b(3)
		return false;}
	case wait_thread::hash:
    	u8(wait_thread::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case set_chip_mode_flag::hash:
    	u8(set_chip_mode_flag::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case clear_chip_mode_flag::hash:
    	u8(clear_chip_mode_flag::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case as_26::hash:
    	u8(as_26::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case as_27::hash:
    	u8(as_27::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case hash("char_say"):{
    	u8(0x28);
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
            return true;
		b(0) str(1) i(2)
	    return false;}
	case as_29::hash:
    	u8(as_29::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case hash("tip_text"):{
    	u8(0x2a);
		auto copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 2, args))
            return true;
		str(0) i(1)
		return false;}
	case as_2b::hash:
    	u8(as_2b::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case shadow_begin::hash:
    	u8(shadow_begin::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) s(1) s(2)
		return false;
	case shadow_end::hash:
    	u8(shadow_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case shake_char::hash:
    	u8(shake_char::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) i(1) i(2) i(3)
		return false;
	case suspend_thread::hash:
    	u8(suspend_thread::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case hash("char_say_random"):{
    	u8(0x30);
		string copy = *line;
		size_t cnt = 0;
		std::stringstream ss;
		ss << copy;
		while (ss >> std::ws) {
			std::string csvElement;
 
			if (ss.peek() == '"') {
				ss >> std::quoted(csvElement);
				std::string discard;
				std::getline(ss, discard, ',');
			}
			else {
				std::getline(ss, csvElement, ',');
			}
			cnt++;
		}
		if(!parse_assembly_instruction(*line, &name_check, cnt, args))
            return true;
		b(0)
		for(int j = 1; j < cnt; j++) str(j)
		u8(0); //null terminator
	    return false;}
	case as_31::hash:
    	u8(as_31::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case as_32::hash:
    	u8(as_32::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case as_33::hash:
    	u8(as_33::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case as_34::hash:
    	u8(as_34::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case keep_angle::hash:
    	u8(keep_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) i(1) i(2) i(3) i(4)
		return false;
	case as_36::hash:
    	u8(as_36::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		i(0) i(1) i(2) i(3)
		return false;
	case rotation_angle::hash:
    	u8(rotation_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		i(0) i(1) i(2) i(3)
		return false;
	case rotation_angle_v::hash:
    	u8(rotation_angle_v::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		i(0) i(1) i(2) i(3)
		return false;
	case set_angle::hash:
    	u8(set_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		i(0) i(1)
		return false;
	case tilt_angle::hash:
    	u8(tilt_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		i(0) i(1)
		return false;
	case rotation_angle_h::hash:
    	u8(rotation_angle_h::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		i(0) i(1)
		return false;
	case as_3c::hash:
    	u8(as_3c::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) i(1)
		return false;
	case shake_screen::hash:
    	u8(shake_screen::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		i(0) i(1) i(2) i(3)
		return false;
	case as_3e::hash:
    	u8(as_3e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		i(0) i(1)
		return false;
	case as_3f::hash:
    	u8(as_3f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_40::hash:
    	u8(as_40::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case lock_angle::hash:
    	u8(lock_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
        	return true;
		b(0)
		return false;
	case as_42::hash:
    	u8(as_42::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) i(1) b(2)
		return false;
	case set_bk_color::hash:
    	u8(set_bk_color::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) i(1) i(2)
		return false;
	case zoom_angle::hash:
    	u8(zoom_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) i(1) i(2)
		return false;
	case as_45::hash:
    	u8(as_45::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case as_46::hash:
    	u8(as_46::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) i(1) i(2)
		return false;
	case as_47::hash:
    	u8(as_47::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_48::hash:
    	u8(as_48::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case set_control::hash:
    	u8(set_control::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case as_4a::hash:
    	u8(as_4a::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case random::hash:{
    	u8(random::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1) i(2)
		if(args[3][0] != '$')
        {
            printf("[ERROR] %s missing $ in label operand: %s", random::name, args[3]);
            return true;
        }
        std::string jmp_target = std::string((const char*)(args[3] + 1));
        auto it = ani->label_to_offset_map.find(jmp_target);
        if(it == ani->label_to_offset_map.end())
        {
            printf("[ERROR] \"%s\": undefined label: %s", random::name, jmp_target.c_str());
            return true;
        }
        u16(it->second);
		return false;}
	case loop_target_beg::hash:{
    	u8(loop_target_beg::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		if(args[0][0] != '$')
        {
            printf("[ERROR] %s missing $ in label operand: %s", loop_target_beg::name, args[0]);
            return true;
        }
        std::string jmp_target = std::string((const char*)(args[0] + 1));
        auto it = ani->label_to_offset_map.find(jmp_target);
        if(it == ani->label_to_offset_map.end())
        {
            printf("[ERROR] %s undefined label: %s", loop_target_beg::name, jmp_target.c_str());
            return true;
        }
        u16(it->second);
		return false;}
	case reset_loop_target::hash:
    	u8(reset_loop_target::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case loop_target_end::hash:
    	u8(loop_target_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_4f::hash:
    	u8(as_4f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case call::hash:{
    	u8(call::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		if(args[0][0] != '$')
        {
            printf("[ERROR] %s missing $ in label operand: %s", call::name, args[0]);
            return true;
        }
        std::string jmp_target = std::string((const char*)(args[0] + 1));
        auto it = ani->label_to_offset_map.find(jmp_target);
        if(it == ani->label_to_offset_map.end())
        {
            printf("[ERROR] %s undefined label: %s", call::name, jmp_target.c_str());
            return true;
        }
        u16(it->second);
		return false;}
	case ret::hash:
    	u8(ret::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_52::hash:
    	u8(as_52::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_53::hash:
    	u8(as_53::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_54::hash:
    	u8(as_54::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case magic_cast_begin::hash:
    	u8(magic_cast_begin::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case magic_cast_end::hash:
    	u8(magic_cast_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_57::hash:
    	u8(as_57::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case beat_back::hash:
    	u8(beat_back::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_59::hash:
    	u8(as_59::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case as_5a::hash:
    	u8(as_5a::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_5b::hash:
    	u8(as_5b::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case show::hash:
    	u8(show::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case hide::hash:
    	u8(hide::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case as_5e::hash:
    	u8(as_5e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_5f::hash:
    	u8(as_5f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case as_60::hash:
    	u8(as_60::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case set_battle_speed::hash:
    	u8(set_battle_speed::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_62::hash:
    	u8(as_62::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) b(2) b(3) s(4)
		return false;
	case as_63::hash:
    	u8(as_63::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case sound_effect::hash:
    	u8(sound_effect::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case sound_effect_ex::hash:
    	u8(sound_effect_ex::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) b(1)
		return false;
	case as_66::hash:
    	u8(as_66::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case hash("scraft_cut_in"):{
    	u8(0x67);
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 1, args))
            return true;
		str(0)
		return false;}
	case nop_68::hash:
    	u8(nop_68::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case release_texture::hash:
    	u8(release_texture::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case load_schip::hash:
    	u8(load_schip::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) i(1) i(2)
		return false;
	case reset_scraft_chip::hash:
    	u8(reset_scraft_chip::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case die::hash:
    	u8(die::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_6d::hash:
    	u8(as_6d::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_6e::hash:
    	u8(as_6e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_6f::hash:
    	u8(as_6f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(0)
		return false;
	case as_70::hash:
    	u8(as_70::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1) s(2) s(3)
		return false;
	case as_71::hash:
    	u8(as_71::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_72::hash:
    	u8(as_72::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_73::hash:
    	u8(as_73::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_74::hash:
    	u8(as_74::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_75::hash:
    	u8(as_75::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case as_76::hash:
    	u8(as_76::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_77::hash:
    	u8(as_77::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case set_eff_state::hash:
    	u8(set_eff_state::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case nop_79::hash:
    	u8(nop_79::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case craft_end::hash:
    	u8(craft_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case set_craft_end_flag::hash:
    	u8(set_craft_end_flag::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case as_7c::hash:
    	u8(as_7c::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case as_7d::hash:
    	u8(as_7d::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case as_7e::hash:
    	u8(as_7e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case blur_screen::hash:
    	u8(blur_screen::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		i(0) i(1) i(2) b(3) i(4)
		return false;
	case as_80::hash:
    	u8(as_80::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_81::hash:
    	u8(as_81::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) s(2)
		return false;
	case as_82::hash:
    	u8(as_82::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case sort_target::hash:
    	u8(sort_target::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case rotate_char::hash:
    	u8(rotate_char::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 6, args))
            return true;
		b(0) s(1) s(2) s(3) i(4) b(5)
		return false;
	case as_85::hash:
    	u8(as_85::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_86::hash:
    	u8(as_86::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		s(0) s(1) s(2) b(3) i(4)
		return false;
	case as_87::hash:
    	u8(as_87::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) b(1)
		return false;
	case voice::hash:
    	u8(voice::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case save_cur_pos::hash:
    	u8(save_cur_pos::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case clone::hash:
    	u8(clone::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case use_item_begin::hash:
    	u8(use_item_begin::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case use_item_end::hash:
    	u8(use_item_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case zoom::hash:
    	u8(zoom::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) i(1) i(2) i(3) i(4)
		return false;
	case hash("load_x_file"): //2 byte opcode
    	u8(0x8e);
		u8(0x01);
		{
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) str(1)
		return false;}
	case as_8e0d::hash: //2 byte opcode
    	u8(as_8e0d::opcode);
		u8(0x0d);
		if(!parse_assembly_instruction(*line, &name_check, 6, args))
            return true;
		b(0) i(1) i(2) i(3) i(4) i(5)
		return false;
	case as_8e::hash:
    	u8(as_8e::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 6, args))
            return true;
		b(0) b(1) i(2) i(3) i(4) i(5)
		return false;
	case as_8f::hash:
    	u8(as_8f::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_90::hash:
    	u8(as_90::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_91::hash:
    	u8(as_91::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_92::hash:
    	u8(as_92::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 7, args))
            return true;
		b(0) b(1) i(2) i(3) i(4) s(5) i(6)
		return false;
	case hash("as_93"):{
    	u8(0x93);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) str(2)
		return false;}
	case hash("as_94"):{
    	u8(0x94);
		string copy = *line;
		if(!parse_assembly_instruction(copy, &name_check, 3, args))
            return true;
		b(0) str(1) i(2)
		return false;}
	case as_95::hash:
    	u8(as_95::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case hash("set_angle_target"):{
    	u8(0x96);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) str(1) s(2)
		return false;}
	case move_angle::hash:
    	u8(move_angle::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		i(0) s(1) s(2)
		return false;
	case as_98::hash:
    	u8(as_98::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
            return true;
		b(0) b(1) i(2) i(3)
		return false;
	case as_99::hash:
    	u8(as_99::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_9a::hash:
    	u8(as_9a::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_9b::hash:
    	u8(as_9b::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case reset_chip_status::hash:
    	u8(reset_chip_status::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_9d::hash:
    	u8(as_9d::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case set_timer::hash:
    	u8(set_timer::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case set_battle_mode::hash:
    	u8(set_battle_mode::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case hash("as_a0"):{
    	u8(0xa0);
		if(!parse_assembly_instruction(*line, &name_check, 4, args))
			return true;
		b(0) i(1) s(2) str(3)
		return false;}
	case as_a1::hash:
    	u8(as_a1::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) i(1)
		return false;
	case as_a2::hash:
    	u8(as_a2::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		b(0)
		return false;
	case as_a3::hash:
    	u8(as_a3::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case as_a402::hash: // 2 byte opcode
    	u8(as_a402::opcode);
		u8(0x02);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		s(0)
		return false;
	case as_a400::hash: // 2 byte opcode
    	u8(as_a400::opcode);
		u8(0x00);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_a401::hash: // 2 byte opcode
    	u8(as_a401::opcode);
		u8(0x01);
		if(!parse_assembly_instruction(*line, &name_check, 0, args))
            return true;
		return false;
	case as_a5::hash:
    	u8(as_a5::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) i(2) i(3) b(4)
		return false;
	case as_a6::hash:
    	u8(as_a6::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 7, args))
            return true;
		b(0) b(1) b(2) i(3) i(4) i(5) i(6)
		return false;
	case battle_effect_end::hash:
    	u8(battle_effect_end::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case damage_voice::hash:
    	u8(damage_voice::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) b(1)
		return false;
	case nop_a9::hash:
    	u8(nop_a9::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 1, args))
            return true;
		i(0)
		return false;
	case as_aa::hash:
    	u8(as_aa::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		i(0) i(1)
		return false;
	case as_ab::hash:
    	u8(as_ab::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) b(2)
		return false;
	case as_ab00::hash:
    	u8(as_ab00::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_ab01::hash:
    	u8(as_ab01::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_ac::hash:
    	u8(as_ac::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) i(2) i(3) b(4)
		return false;
	case as_ad::hash:
    	u8(as_ad::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 3, args))
            return true;
		b(0) b(1) i(2)
		return false;
	case as_ae::hash:
    	u8(as_ae::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		s(0) i(1)
		return false;
	case as_af::hash:
    	u8(as_af::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 5, args))
            return true;
		b(0) b(1) i(2) i(3) i(4)
		return false;
	case as_b0::hash:
    	u8(as_b0::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	case as_b1::hash:
    	u8(as_b1::opcode);
		if(!parse_assembly_instruction(*line, &name_check, 2, args))
            return true;
		b(0) s(1)
		return false;
	}
	#undef u8
	#undef u16
	#undef b
	#undef s
	#undef i
	#undef str
	return true; //silence compiler warning
}