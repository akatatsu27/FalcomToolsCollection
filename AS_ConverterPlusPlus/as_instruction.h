#include "directives.h"
#include "binary_context.h"
#include "text_context.h"
#pragma once

struct instruction
{
    size_t offset;
    uint8 opcode;

	uint16 targetOffset = 0;
	char* labelName; //the label of the target offset
    std::list<std::string>::iterator line; //the line in the text_context
    uint32 name_hash; //the hash of the instruction's name. used in switch statements

    static instruction first_pass_binary(binary_context *const ctx);
    static instruction first_pass_text(std::list<string>::iterator &line, char* name, int len, size_t &cur_offset);
    void second_pass_binary(binary_context* const ctx, string* const text);
    void second_pass_text(text_context* const ctx, vector<char>* const binary);

    static constexpr uint32 hash(const char *const str)
    {
	    uint32 hash = 0;
	    for (int i = 0; i < str[i] != 0; i++)
	    {
		    hash = ((str[i] + 131 * hash));
	    }
	    return hash;
    }

    static uint32 hash(const char *const str, size_t len)
    {
	    uint32 hash = 0;
    	for (int i = 0; i < len && str[i] != 0; i++)
	    {
		    hash = ((str[i] + 131 * hash));
	    }
    	return hash;
    }

    static const char* const get_target_name(uint8_t num)
    {
        switch(num)
        {
        case 0x00:
            return "target_00";
        case 0x01:
            return "target_01";
        case 0x02:
            return "target_02";
        case 0x03:
            return "target_03";
        case 0x04:
            return "target_04";
        case 0x05:
            return "target_05";
        case 0x06:
            return "target_06";
        case 0x07:
            return "target_07";
        case 0x08:
            return "target_08";
        case 0x09:
            return "target_09";
        case 0x0A:
            return "target_0A";
        case 0x0B:
            return "target_0B";
        case 0x0C:
            return "target_0C";
        case 0x0D:
            return "target_0D";
        case 0x0E:
            return "target_0E";
        case 0x0F:
            return "target_0F";
        case 0x10:
            return "target_10";
        case 0x11:
            return "target_11";
        case 0x12:
            return "target_12";
        case 0x14:
            return "target_14";
        case 0x32:
            return "target_32";
        case 0x38:
            return "target_38";
        case 0x64:
            return "target_64";
        case 0x6A:
            return "target_6A";
        case 0x96:
            return "target_96";
        case 0x9C:
            return "target_9C";
        case 0xC8:
            return "target_C8";
        case 0xCE:
            return "target_CE";
        case 0xE7:
            return "target_E7";
        case 0xEA:
            return "target_EA";
        case 0xEE:
            return "target_EE";
        case 0xF0:
            return "target_F0";
        case 0xF2:
            return "target_F2";
        case 0xF3:
            return "target_F3";
        case 0xF7:
            return "target_F7";
        case 0xF8:
            return "target_F8";
        case 0xFB:
            return "target_FB";
        case 0xFC:
            return "target_FC";
        case 0xFD:
            return "target_FD";
        case 0xFE:
            return "target_FE";
        case 0xFF:
            return "self";
        default:
            printf("unrecognized instruction target value: 0x%02X\n", num);
            return "err";
        }
        
    }
};

#define INSTRUCTION(x,y) struct y\
{\
    static constexpr size_t size = x;\
    static constexpr char name[25] = #y;\
    static constexpr uint32 hash = instruction::hash(#y);\
};

INSTRUCTION(0,end_event);
INSTRUCTION(2,go_to);
INSTRUCTION(1+1,select_sub_chip);;
INSTRUCTION(1+2,deg);
INSTRUCTION(1+1+2,as_04);
INSTRUCTION(1+1+4,as_05);
INSTRUCTION(4,sleep);
INSTRUCTION(0,update);
INSTRUCTION(1+1+4+4+4,teleport);
INSTRUCTION(1+1+4+4+4,as_09);
INSTRUCTION(1+1+1+4,as_0a);
INSTRUCTION(1+1+2,turn);
INSTRUCTION(1+1+2+2+1,as_0c);
INSTRUCTION(1+1+4+4+4+2+2,jump);
INSTRUCTION(1 + 4 + 4 + 4 + 2 + 2,drop_down);
INSTRUCTION(2 + 2,jump_to_target);
INSTRUCTION(2 + 2,jump_back);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 4 + 1,move);
//INSTRUCTION(x,add_effect); variable length
INSTRUCTION(2,release_effect);
INSTRUCTION(2,as_14);
INSTRUCTION(1+1,wait_effect);
INSTRUCTION(1+1,finish_effect);
INSTRUCTION(1+1,cancel_effect);
INSTRUCTION(1 + 1 + 1 + 2 + 4 + 4 + 4 + 2 + 2 + 2 + 2 + 2 + 2 + 1,show_effect);
//INSTRUCTION(x,show_3d_effect); variable length
INSTRUCTION(1+2,as_1a);
INSTRUCTION(1+1,select_chip);
INSTRUCTION(1,damage);
INSTRUCTION(1+1+4,damage_anime);
INSTRUCTION(4,as_1e);
INSTRUCTION(2+2+1,as_1f);
INSTRUCTION(1 + 1 + 1 + 1 + 4 + 4,as_20);
INSTRUCTION(1 + 1 + 4 + 4,as_21);
INSTRUCTION(1 + 1 + 2 + 1,begin_thread);
INSTRUCTION(1+1,wait_thread);
INSTRUCTION(1 + 1 + 2,set_chip_mode_flag);
INSTRUCTION(1 + 1 + 2,clear_chip_mode_flag);
INSTRUCTION(1 + 1 + 2,as_26);
INSTRUCTION(1 + 1 + 2,as_27);
//INSTRUCTION(x, char_say); variable length
INSTRUCTION(1,as_29);
//INSTRUCTION(x,tip_text); variable length
INSTRUCTION(0,as_2b);
INSTRUCTION(1+2+2,shadow_begin);
INSTRUCTION(1,shadow_end);
INSTRUCTION(1+4+4+4,shake_char);
INSTRUCTION(1+1,suspend_thread);
//INSTRUCTION(x,as_30); variable length
INSTRUCTION(1+4,as_31);
INSTRUCTION(1+1,as_32);
INSTRUCTION(1+1,as_33);
INSTRUCTION(0,as_34);
INSTRUCTION(1 + 4 + 4 + 4 + 4,keep_angle);
INSTRUCTION(4+4+4+4,as_36);
INSTRUCTION(4+4+4+4,rotation_angle);
INSTRUCTION(4 + 4 + 4 + 4,rotation_angle_v);
INSTRUCTION(4+4,set_angle);
INSTRUCTION(4+4,tilt_angle);
INSTRUCTION(4+4,rotation_angle_h);
INSTRUCTION(2+4,as_3c);
INSTRUCTION(4 + 4 + 4 + 4,shake_screen);
INSTRUCTION(4+4,as_3e);
INSTRUCTION(1,as_3f);
INSTRUCTION(1,as_40);
INSTRUCTION(1,lock_angle);
INSTRUCTION(1+4+1,as_42);
INSTRUCTION(1+4+4,set_bk_color);
INSTRUCTION(1+4+4,zoom_angle);
INSTRUCTION(1+4,as_45);
INSTRUCTION(1+4+4,as_46);
INSTRUCTION(1,as_47);
INSTRUCTION(1+4,as_48);
INSTRUCTION(1+2,set_control);
INSTRUCTION(1,as_4a);
INSTRUCTION(1+1+4+2,random);
INSTRUCTION(2,loop_target_beg);
INSTRUCTION(0,reset_loop_target);
INSTRUCTION(0,loop_target_end);
INSTRUCTION(1+1,as_4f);
INSTRUCTION(2,call);
INSTRUCTION(0,ret);
INSTRUCTION(1,as_52);
INSTRUCTION(1,as_53);
INSTRUCTION(1,as_54);
INSTRUCTION(2,magic_cast_begin);
INSTRUCTION(0,magic_cast_end);
INSTRUCTION(1+1,as_57);
INSTRUCTION(1,beat_back);
INSTRUCTION(1+2,as_59);
INSTRUCTION(1+1+4,as_5a);
INSTRUCTION(4,as_5b);
INSTRUCTION(1+4,show);
INSTRUCTION(1+4,hide);
INSTRUCTION(1,as_5e);
INSTRUCTION(1+1,as_5f);
INSTRUCTION(1,as_60);
INSTRUCTION(4,set_battle_speed);
INSTRUCTION(1+1+1+1+2,as_62);
INSTRUCTION(1+4,as_63);
INSTRUCTION(2,sound_effect);
INSTRUCTION(2+1,sound_effect_ex);
INSTRUCTION(2,as_66);
//INSTRUCTION(x,scraft_cut_in); variable length
INSTRUCTION(0,nop_68);
INSTRUCTION(0,release_texture);
INSTRUCTION(1+4+4,load_schip);
INSTRUCTION(0,reset_scraft_chip);
INSTRUCTION(0,die);
INSTRUCTION(4,as_6d);
INSTRUCTION(4,as_6e);
INSTRUCTION(1+1,as_6f);
INSTRUCTION(1+1+2+2,as_70);
INSTRUCTION(1,as_71);
INSTRUCTION(1,as_72);
INSTRUCTION(1,as_73);
INSTRUCTION(1,as_74);
INSTRUCTION(2,as_75);
INSTRUCTION(1,as_76);
INSTRUCTION(1,as_77);
INSTRUCTION(1,set_eff_state);
INSTRUCTION(1,nop_79);
INSTRUCTION(1,craft_end);
INSTRUCTION(2,set_craft_end_flag);
INSTRUCTION(1+1,as_7c);
INSTRUCTION(1+4,as_7d);
INSTRUCTION(4,as_7e);
INSTRUCTION(4 + 4 + 4 + 1 + 4,blur_screen);
INSTRUCTION(4,as_80);
INSTRUCTION(1+1+2,as_81);
INSTRUCTION(0,as_82);
INSTRUCTION(1,sort_target);
INSTRUCTION(1 + 2 + 2 + 2 + 4 + 1,rotate_char);
INSTRUCTION(1+1+4,as_85);
INSTRUCTION(2+2+2+1+4,as_86);
INSTRUCTION(2+1,as_87);
INSTRUCTION(2,voice);
INSTRUCTION(1,save_cur_pos);
INSTRUCTION(1+1,clone);
INSTRUCTION(0,use_item_begin);
INSTRUCTION(0,use_item_end);
INSTRUCTION(1 + 4 + 4 + 4 + 4,zoom);
//INSTRUCTION(x,load_x_file); variable length
INSTRUCTION((1)+1 + 4 + 4 + 4 + 4 + 4,as_8e0d);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 4,as_8e);
INSTRUCTION(1,as_8f);
INSTRUCTION(1,as_90);
INSTRUCTION(1,as_91);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 2 + 4,as_92);
//INSTRUCTION(x,as_93); variable length
//INSTRUCTION(x,as_94); variable length
INSTRUCTION(0,as_95);
//INSTRUCTION(x,set_angle_target); variable length
INSTRUCTION(4 + 2 + 2,move_angle);
INSTRUCTION(1 + 1 + 4 + 4,as_98);
INSTRUCTION(1,as_99);
INSTRUCTION(4,as_9a);
INSTRUCTION(1,as_9b);
INSTRUCTION(1,reset_chip_status);
INSTRUCTION(1,as_9d);
INSTRUCTION(1,set_timer);
INSTRUCTION(1+4,set_battle_mode);
//INSTRUCTION(x,as_a0); variable length
INSTRUCTION(1+4,as_a1);
INSTRUCTION(1,as_a2);
INSTRUCTION(1+1,as_a3);
INSTRUCTION((1)+2,as_a402);
INSTRUCTION((1),as_a400);
INSTRUCTION((1),as_a401);
INSTRUCTION(1 + 1 + 4 + 4 + 1,as_a5);
INSTRUCTION(1 + 1 + 1 + 4 + 4 + 4 + 4,as_a6);
INSTRUCTION(1+2,battle_effect_end);
INSTRUCTION(1+1,damage_voice);
INSTRUCTION(4,nop_a9);
INSTRUCTION(4+4,as_aa);
INSTRUCTION(1+1+1,as_ab);
INSTRUCTION(1+1+4+4+1,as_ac);
INSTRUCTION(1+1+4,as_ad);
INSTRUCTION(2+4,as_ae);
INSTRUCTION(1 + 1 + 4 + 4 + 4,as_af);
INSTRUCTION(1+2,as_b0);
INSTRUCTION(1+2,as_b1);