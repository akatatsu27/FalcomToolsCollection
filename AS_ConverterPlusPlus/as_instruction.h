#pragma once
#include "directives.h"
#include "binary_context.h"
#include "text_context.h"

struct base_aniscript;

struct instruction
{
    size_t offset;
    uint8 opcode = 0x00;

	uint16 targetOffset = 0;
	char* labelName; //the label of the target offset
    std::list<std::string>::iterator line; //the line in the text_context
    uint32 name_hash; //the hash of the instruction's name. used in switch statements

    static instruction first_pass_binary(binary_context *const ctx, base_aniscript* const ani);
    static instruction first_pass_text(std::list<string>::iterator &line, char* name, int len, size_t &cur_offset);
    void second_pass_binary(binary_context* const ctx, string* const text);
    bool second_pass_text(base_aniscript* const ani);

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

#define INSTRUCTION(x,y,z) struct y\
{\
    static constexpr size_t size = x;\
    static constexpr char name[25] = #y;\
    static constexpr uint8 opcode = z;\
    static constexpr uint32 hash = instruction::hash(#y);\
};

INSTRUCTION(0,end_event,0x00);
INSTRUCTION(2,go_to,0x01);
INSTRUCTION(1+1,select_sub_chip,0x02);
INSTRUCTION(1+2,deg,0x03);
INSTRUCTION(1+1+2,as_04,0x04);
INSTRUCTION(1+1+4,as_05,0x05);
INSTRUCTION(4,sleep,0x06);
INSTRUCTION(0,update,0x07);
INSTRUCTION(1+1+4+4+4,teleport,0x08);
INSTRUCTION(1+1+4+4+4,as_09,0x09);
INSTRUCTION(1+1+1+4,as_0a,0x0a);
INSTRUCTION(1+1+2,turn,0x0b);
INSTRUCTION(1+1+2+2+1,as_0c,0x0c);
INSTRUCTION(1+1+4+4+4+2+2,jump,0x0d);
INSTRUCTION(1 + 4 + 4 + 4 + 2 + 2,drop_down,0x0e);
INSTRUCTION(2 + 2,jump_to_target,0x0f);
INSTRUCTION(2 + 2,jump_back,0x10);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 4 + 1,move,0x11);
//INSTRUCTION(x,add_effect,0x12); variable length
INSTRUCTION(2,release_effect,0x13);
INSTRUCTION(2,as_14,0x14);
INSTRUCTION(1+1,wait_effect,0x15);
INSTRUCTION(1+1,finish_effect,0x16);
INSTRUCTION(1+1,cancel_effect,0x17);
INSTRUCTION(1+1+1+2+4+4+4+2+2+2+2+2+2+1,show_effect,0x18);
//INSTRUCTION(x,show_3d_effect,0x19); variable length
INSTRUCTION(1+2,as_1a,0x1a);
INSTRUCTION(1+1,select_chip,0x1b);
INSTRUCTION(1,damage,0x1c);
INSTRUCTION(1+1+4,damage_anime,0x1d);
INSTRUCTION(4,as_1e,0x1e);
INSTRUCTION(2+2+1,as_1f,0x1f);
INSTRUCTION(1 + 1 + 1 + 1 + 4 + 4,as_20,0x20);
INSTRUCTION(1 + 1 + 4 + 4,as_21,0x21);
INSTRUCTION(1 + 1 + 2 + 1,begin_thread,0x22);
INSTRUCTION(1+1,wait_thread,0x23);
INSTRUCTION(1 + 1 + 2,set_chip_mode_flag,0x24);
INSTRUCTION(1 + 1 + 2,clear_chip_mode_flag,0x25);
INSTRUCTION(1 + 1 + 2,as_26,0x26);
INSTRUCTION(1 + 1 + 2,as_27,0x27);
//INSTRUCTION(x, char_say,0x28); variable length
INSTRUCTION(1,as_29,0x29);
//INSTRUCTION(x,tip_text,0x2a); variable length
INSTRUCTION(0,as_2b,0x2b);
INSTRUCTION(1+2+2,shadow_begin,0x2c);
INSTRUCTION(1,shadow_end,0x2d);
INSTRUCTION(1+4+4+4,shake_char,0x2e);
INSTRUCTION(1+1,suspend_thread,0x2f);
//INSTRUCTION(x,char_say_random,0x30); variable length
INSTRUCTION(1+4,as_31,0x31);
INSTRUCTION(1+1,as_32,0x32);
INSTRUCTION(1+1,as_33,0x33);
INSTRUCTION(0,as_34,0x34);
INSTRUCTION(1 + 4 + 4 + 4 + 4,keep_angle,0x35);
INSTRUCTION(4+4+4+4,as_36,0x36);
INSTRUCTION(4+4+4+4,rotation_angle,0x37);
INSTRUCTION(4 + 4 + 4 + 4,rotation_angle_v,0x38);
INSTRUCTION(4+4,set_angle,0x39);
INSTRUCTION(4+4,tilt_angle,0x3a);
INSTRUCTION(4+4,rotation_angle_h,0x3b);
INSTRUCTION(2+4,as_3c,0x3c);
INSTRUCTION(4 + 4 + 4 + 4,shake_screen,0x3d);
INSTRUCTION(4+4,as_3e,0x3e);
INSTRUCTION(1,as_3f,0x3f);
INSTRUCTION(1,as_40,0x40);
INSTRUCTION(1,lock_angle,0x41);
INSTRUCTION(1+4+1,as_42,0x42);
INSTRUCTION(1+4+4,set_bk_color,0x43);
INSTRUCTION(1+4+4,zoom_angle,0x44);
INSTRUCTION(1+4,as_45,0x45);
INSTRUCTION(1+4+4,as_46,0x46);
INSTRUCTION(1,as_47,0x47);
INSTRUCTION(1+4,as_48,0x48);
INSTRUCTION(1+2,set_control,0x49);
INSTRUCTION(1,as_4a,0x4a);
INSTRUCTION(1+1+4+2,random,0x4b);
INSTRUCTION(2,loop_target_beg,0x4c);
INSTRUCTION(0,reset_loop_target,0x4d);
INSTRUCTION(0,loop_target_end,0x4e);
INSTRUCTION(1+1,as_4f,0x4f);
INSTRUCTION(2,call,0x50);
INSTRUCTION(0,ret,0x51);
INSTRUCTION(1,as_52,0x52);
INSTRUCTION(1,as_53,0x53);
INSTRUCTION(1,as_54,0x54);
INSTRUCTION(2,magic_cast_begin,0x55);
INSTRUCTION(0,magic_cast_end,0x56);
INSTRUCTION(1+1,as_57,0x57);
INSTRUCTION(1,beat_back,0x58);
INSTRUCTION(1+2,as_59,0x59);
INSTRUCTION(1+1+4,as_5a,0x5a);
INSTRUCTION(4,as_5b,0x5b);
INSTRUCTION(1+4,show,0x5c);
INSTRUCTION(1+4,hide,0x5d);
INSTRUCTION(1,as_5e,0x5e);
INSTRUCTION(1+1,as_5f,0x5f);
INSTRUCTION(1,as_60,0x60);
INSTRUCTION(4,set_battle_speed,0x61);
INSTRUCTION(1+1+1+1+2,as_62,0x62);
INSTRUCTION(1+4,as_63,0x63);
INSTRUCTION(2,sound_effect,0x64);
INSTRUCTION(2+1,sound_effect_ex,0x65);
INSTRUCTION(2,as_66,0x66);
//INSTRUCTION(x,scraft_cut_in,0x67); variable length
INSTRUCTION(0,nop_68,0x68);
INSTRUCTION(0,release_texture,0x69);
INSTRUCTION(1+4+4,load_schip,0x6a);
INSTRUCTION(0,reset_scraft_chip,0x6b);
INSTRUCTION(0,die,0x6c);
INSTRUCTION(4,as_6d,0x6d);
INSTRUCTION(4,as_6e,0x6e);
INSTRUCTION(1+1,as_6f,0x6f);
INSTRUCTION(1+1+2+2,as_70,0x70);
INSTRUCTION(1,as_71,0x71);
INSTRUCTION(1,as_72,0x72);
INSTRUCTION(1,as_73,0x73);
INSTRUCTION(1,as_74,0x74);
INSTRUCTION(2,as_75,0x75);
INSTRUCTION(1,as_76,0x76);
INSTRUCTION(1,as_77,0x77);
INSTRUCTION(1,set_eff_state,0x78);
INSTRUCTION(1,nop_79,0x79);
INSTRUCTION(1,craft_end,0x7a);
INSTRUCTION(2,set_craft_end_flag,0x7b);
INSTRUCTION(1+1,as_7c,0x7c);
INSTRUCTION(1+4,as_7d,0x7d);
INSTRUCTION(4,as_7e,0x7e);
INSTRUCTION(4 + 4 + 4 + 1 + 4,blur_screen,0x7f);
INSTRUCTION(4,as_80,0x80);
INSTRUCTION(1+1+2,as_81,0x81);
INSTRUCTION(0,as_82,0x82);
INSTRUCTION(1,sort_target,0x83);
INSTRUCTION(1 + 2 + 2 + 2 + 4 + 1,rotate_char,0x84);
INSTRUCTION(1+1+4,as_85,0x85);
INSTRUCTION(2+2+2+1+4,as_86,0x86);
INSTRUCTION(2+1,as_87,0x87);
INSTRUCTION(2,voice,0x88);
INSTRUCTION(1,save_cur_pos,0x89);
INSTRUCTION(1+1,clone,0x8a);
INSTRUCTION(0,use_item_begin,0x8b);
INSTRUCTION(0,use_item_end,0x8c);
INSTRUCTION(1 + 4 + 4 + 4 + 4,zoom,0x8d);
//INSTRUCTION(x,load_x_file,0x8e); variable length
INSTRUCTION((1)+1 + 4 + 4 + 4 + 4 + 4,as_8e0d,0x8e);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 4,as_8e,0x8e);
INSTRUCTION(1,as_8f,0x8f);
INSTRUCTION(1,as_90,0x90);
INSTRUCTION(1,as_91,0x91);
INSTRUCTION(1 + 1 + 4 + 4 + 4 + 2 + 4,as_92,0x92);
//INSTRUCTION(x,as_93,0x93); variable length
//INSTRUCTION(x,as_94,0x94); variable length
INSTRUCTION(0,as_95,0x95);
//INSTRUCTION(x,set_angle_target,0x96); variable length
INSTRUCTION(4 + 2 + 2,move_angle,0x97);
INSTRUCTION(1 + 1 + 4 + 4,as_98,0x98);
INSTRUCTION(1,as_99,0x99);
INSTRUCTION(4,as_9a,0x9a);
INSTRUCTION(1,as_9b,0x9b);
INSTRUCTION(1,reset_chip_status,0x9c);
INSTRUCTION(1,as_9d,0x9d);
INSTRUCTION(1,set_timer,0x9e);
INSTRUCTION(1+4,set_battle_mode,0x9f);
//INSTRUCTION(x,as_a0,0xa0); variable length
INSTRUCTION(1+4,as_a1,0xa1);
INSTRUCTION(1,as_a2,0xa2);
INSTRUCTION(1+1,as_a3,0xa3);
INSTRUCTION((1)+2,as_a402,0xa4);
INSTRUCTION((1),as_a400,0xa4);
INSTRUCTION((1),as_a401,0xa4);
INSTRUCTION(1 + 1 + 4 + 4 + 1,as_a5,0xa5);
INSTRUCTION(1 + 1 + 1 + 4 + 4 + 4 + 4,as_a6,0xa6);
INSTRUCTION(1+2,battle_effect_end,0xa7);
INSTRUCTION(1+1,damage_voice,0xa8);
INSTRUCTION(4,nop_a9,0xa9);
INSTRUCTION(4+4,as_aa,0xaa);
INSTRUCTION(1+1+1,as_ab,0xab);
INSTRUCTION((1)+1+1+4,as_ab00,0xab);
INSTRUCTION((1)+1+1+4,as_ab01,0xab);
INSTRUCTION(1+1+4+4+1,as_ac,0xac);
INSTRUCTION(1+1+4,as_ad,0xad);
INSTRUCTION(2+4,as_ae,0xae);
INSTRUCTION(1 + 1 + 4 + 4 + 4,as_af,0xaf);
INSTRUCTION(1+2,as_b0,0xb0);
INSTRUCTION(1+2,as_b1,0xb1);