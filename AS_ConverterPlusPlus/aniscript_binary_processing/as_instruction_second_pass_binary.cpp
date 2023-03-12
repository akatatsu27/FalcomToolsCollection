#include "../as_instruction.h"

void instruction::second_pass_binary(binary_context* const ctx, string* const text)
{
    char buffer[500];
#define b ctx->u8()
#define s ctx->u16()
#define i ctx->u32()
#define str ctx->cstring()
#define txt text->append(&buffer[0]);

    ctx->u8(); // skip the opcode... and assume it's correct
    text->append("\t");
    switch (opcode)
    {
    case 0x00: // no operands
        sprintf(&buffer[0], "%s\n", end_event::name);
        txt return;
    case 0x01:{
        s;
        sprintf(&buffer[0], "%s $%s\n", go_to::name, labelName);
        txt return;}
    case 0x02:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", select_sub_chip::name, target_name, op2);
        txt return;}
    case 0x03:{
        uint8 target = b; uint16 op2 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%04X\n", deg::name, target_name, op2);
        txt return;}
    case 0x04:{
        uint8 target = b; uint8 op2 = b; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X, 0x%04X\n", as_04::name, target_name, op2, op3);
        txt return;}
    case 0x05:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X\n", as_05::name, op1, op2, op3);
        txt return;}
    case 0x06:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", sleep::name, op1);
        txt return;}
    case 0x07: // no operands
        sprintf(&buffer[0], "%s\n", update::name);
        txt return;
    case 0x08:{
        uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        const char* const target1_name = instruction::get_target_name(target1);
        const char* const target2_name = instruction::get_target_name(target2);
        sprintf(&buffer[0], "%s %s, %s, 0x%08X, 0x%08X, 0x%08X\n", teleport::name, target1_name, target2_name, op3, op4, op5);
        txt return;}
    case 0x09:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", as_09::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x0A:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%02X, 0x%08X\n", as_0a::name, op1, op2, op3, op4);
        txt return;}
    case 0x0B:{
        uint8 target = b; uint8 op2 = b; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X, 0x%04X\n", turn::name, target_name, op2, op3);
        txt return;}
    case 0x0C:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s; uint16 op4 = s; uint8 op5 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%04X, 0x%04X, 0x%02X\n", as_0c::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x0D:{
        uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint16 op6 = s; uint16 op7 = s;
        const char* const target1_name = instruction::get_target_name(target1);
        const char* const target2_name = instruction::get_target_name(target2);
        sprintf(&buffer[0], "%s %s, %s, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X\n", jump::name, target1_name, target2_name, op3, op4, op5, op6, op7);
        txt return;}
    case 0x0E:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint16 op5 = s; uint16 op6 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X\n", drop_down::name, op1, op2, op3, op4, op5, op6);
        txt return;}
    case 0x0F:{
        uint16 op1 = s; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%04X, 0x%04X\n", jump_to_target::name, op1, op2);
        txt return;}
    case 0x10:{
        uint16 op1 = s; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%04X, 0x%04X\n", jump_back::name, op1, op2);
        txt return;}
    case 0x11:{
        uint8 target = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i; uint8 op7 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%02X\n", move::name, target_name, op2, op3, op4, op5, op6, op7);
        txt return;}
    case 0x12:{
        uint16 op1 = s; char *op2 = str;
        snprintf(&buffer[0], sizeof(buffer), "add_effect 0x%04X, \"%s\"\n", op1, op2);
        txt return;}
    case 0x13:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", release_effect::name, op1);
        txt return;}
    case 0x14:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", as_14::name, op1);
        txt return;}
    case 0x15:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", wait_effect::name, target_name, op2);
        txt return;}
    case 0x16:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", finish_effect::name, target_name, op2);
        txt return;}
    case 0x17:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", cancel_effect::name, target_name, op2);
        txt return;}
    case 0x18:{
        uint8 op1 = b; uint8 op2 = b; uint8 target = b; uint16 op4 = s; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        uint16 op8 = s; uint16 op9 = s; uint16 op10 = s; uint32 op11 = s; uint16 op12 = s; uint32 op13 = s; uint8 op14 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, %s, 0x%04X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%02X\n",
            show_effect::name, op1, op2, target_name, op4, op5, op6, op7, op8, op9, op10, op11, op12, op13, op14);
        txt return;}
    case 0x19:{
        uint8 op1 = b; uint8 op2 = b; char *op3 = str; uint16 op4 = s; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        uint16 op8 = s; uint16 op9 = s; uint16 op10 = s; uint32 op11 = s; uint16 op12 = s; uint32 op13 = s; uint8 op14 = b;
        snprintf(&buffer[0], 500, "show_3d_effect 0x%02X, 0x%02X, \"%s\", 0x%04X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%02X\n",
         op1, op2, op3, op4, op5, op6, op7, op8, op9, op10, op11, op12, op13, op14);
        txt return;}
    case 0x1A:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", as_1a::name, op1, op2);
        txt return;}
    case 0x1B:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", select_chip::name, target_name, op2);
        txt return;}
    case 0x1C:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", damage::name, op1);
        txt return;}
    case 0x1D:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X\n", damage_anime::name, op1, op2, op3);
        txt return;}
    case 0x1E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_1e::name, op1);
        txt return;}
    case 0x1F:{
        uint16 op1 = s; uint16 op2 = s; uint8 op3 = b;
        sprintf(&buffer[0], "%s 0x%04X, 0x%04X, 0x%02X\n", as_1f::name, op1, op2, op3);
        txt return;}
    case 0x20:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint8 op4 = b; uint32 op5 = i; uint32 op6 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", as_20::name, op1, op2, op3, op4, op5, op6);
        txt return;}
    case 0x21:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", as_21::name, op1, op2, op3, op4);
        txt return;}
    case 0x22:{
        uint8 target = b; uint8 op2 = b; s; uint8 op4 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X, $%s, 0x%02X\n", begin_thread::name, target_name, op2, labelName , op4);
        txt return;}
    case 0x23:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", wait_thread::name, target_name, op2);
        txt return;}
    case 0x24:{
        uint8 op1 = b; uint8 target = b; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s 0x%02X, %s, 0x%04X\n", set_chip_mode_flag::name, op1, target_name, op3);
        txt return;}
    case 0x25:{
        uint8 op1 = b; uint8 target = b; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s 0x%02X, %s, 0x%04X\n", clear_chip_mode_flag::name, op1, target_name, op3);
        txt return;}
    case 0x26:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%04X\n", as_26::name, op1, op2, op3);
        txt return;}
    case 0x27:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%04X\n", as_27::name, op1, op2, op3);
        txt return;}
    case 0x28:{
        uint8 op1 = b; char *op2 = str; uint32 op3 = i;
        snprintf(&buffer[0], 500, "char_say 0x%02X, \"%s\", 0x%08X\n", op1, op2, op3);
        txt return;}
    case 0x29:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_29::name, op1);
        txt return;}
    case 0x2A:{
        char *op1 = str; uint32 op2 = i;
        snprintf(&buffer[0], 500, "tip_text \"%s\", 0x%08X\n", op1, op2);
        txt return;}
    case 0x2B: // no operands
        sprintf(&buffer[0], "%s\n", as_2b::name);
        txt return;
    case 0x2C:{
        uint8 target = b; uint16 op2 = s; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%04X, 0x%04X\n", shadow_begin::name, target_name, op2, op3);
        txt return;}
    case 0x2D:{
        uint8 target = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s\n", shadow_end::name, target_name);
        txt return;}
    case 0x2E:{
        uint8 target = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%08X, 0x%08X, 0x%08X\n", shake_char::name, target_name, op2, op3, op4);
        txt return;}
    case 0x2F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", suspend_thread::name, op1, op2);
        txt return;}
    case 0x30:{
        uint8 op1 = b;
        sprintf(&buffer[0], "char_say_random 0x%02X", op1);
        txt 
        do
        {
            char *string = str;
            if (*string == 0)
            {
                text->append("\n");
                return;
            }
            snprintf(&buffer[0], sizeof(buffer), ", \"%s\"", string);
            txt
        }
        while (1);
        return;}
    case 0x31:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_31::name, op1, op2);
        txt return;}
    case 0x32:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_32::name, op1, op2);
        txt return;}
    case 0x33:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_33::name, op1, op2);
        txt return;}
    case 0x34: // no operands
        sprintf(&buffer[0], "%s\n", as_34::name);
        txt return;
    case 0x35:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", keep_angle::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x36:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", as_36::name, op1, op2, op3, op4);
        txt return;}
    case 0x37:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", rotation_angle::name, op1, op2, op3, op4);
        txt return;}
    case 0x38:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", rotation_angle_v::name, op1, op2, op3, op4);
        txt return;}
    case 0x39:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X\n", set_angle::name, op1, op2);
        txt return;}
    case 0x3A:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X\n", tilt_angle::name, op1, op2);
        txt return;}
    case 0x3B:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X\n", rotation_angle_h::name, op1, op2);
        txt return;}
    case 0x3C:{
        uint16 op1 = s; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%04X, 0x%08X\n", as_3c::name, op1, op2);
        txt return;}
    case 0x3D:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", shake_screen::name, op1, op2, op3, op4);
        txt return;}
    case 0x3E:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X\n", as_3e::name, op1, op2);
        txt return;}
    case 0x3F:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_3f::name, op1);
        txt return;}
    case 0x40:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_40::name, op1);
        txt return;}
    case 0x41:{
        uint8 target = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s\n", lock_angle::name, target_name);
        txt return;}
    case 0x42:{
        uint8 op1 = b; uint32 op2 = i; uint8 op3 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%02X\n", as_42::name, op1, op2, op3);
        txt return;}
    case 0x43:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X\n", set_bk_color::name, op1, op2, op3);
        txt return;}
    case 0x44:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X\n", zoom_angle::name, op1, op2, op3);
        txt return;}
    case 0x45:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_45::name, op1, op2);
        txt return;}
    case 0x46:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X\n", as_46::name, op1, op2, op3);
        txt return;}
    case 0x47:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_47::name, op1);
        txt return;}
    case 0x48:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_48::name, op1, op2);
        txt return;}
    case 0x49:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", set_control::name, op1, op2);
        txt return;}
    case 0x4A:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_4a::name, op1);
        txt return;}
    case 0x4B:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, $%s\n", random::name, op1, op2, op3, labelName);
        txt return;}
    case 0x4C:{
        s;
        sprintf(&buffer[0], "%s $%s\n", loop_target_beg::name, labelName);
        txt return;}
    case 0x4D: // no operands
        sprintf(&buffer[0], "%s\n", reset_loop_target::name);
        txt return;
    case 0x4E: // no operands
        sprintf(&buffer[0], "%s\n", loop_target_end::name);
        txt return;
    case 0x4F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_4f::name, op1, op2);
        txt return;}
    case 0x50:{
        s;
        sprintf(&buffer[0], "%s $%s\n", call::name, labelName);
        txt return;}
    case 0x51: // no operands
        sprintf(&buffer[0], "%s\n", ret::name);
        txt return;
    case 0x52:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_52::name, op1);
        txt return;}
    case 0x53:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_53::name, op1);
        txt return;}
    case 0x54:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_54::name, op1);
        txt return;}
    case 0x55:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", magic_cast_begin::name, op1);
        txt return;}
    case 0x56: // no operands
        sprintf(&buffer[0], "%s\n", magic_cast_end::name);
        txt return;
    case 0x57:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_57::name, op1, op2);
        txt return;}
    case 0x58:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", beat_back::name, op1);
        txt return;}
    case 0x59:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", as_59::name, op1, op2);
        txt return;}
    case 0x5A:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X\n", as_5a::name, op1, op2, op3);
        txt return;}
    case 0x5B:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_5b::name, op1);
        txt return;}
    case 0x5C:{
        uint8 target = b; uint32 op2 = i;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%08X\n", show::name, target_name, op2);
        txt return;}
    case 0x5D:{
        uint8 target = b; uint32 op2 = i;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%08X\n", hide::name, target_name, op2);
        txt return;}
    case 0x5E:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_5e::name, op1);
        txt return;}
    case 0x5F:{
        uint8 target = b; uint8 op2 = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s, 0x%02X\n", as_5f::name, target_name, op2);
        txt return;}
    case 0x60:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_60::name, op1);
        txt return;}
    case 0x61:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", set_battle_speed::name, op1);
        txt return;}
    case 0x62:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint8 op4 = b; uint16 op5 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%04X\n", as_62::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x63:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_63::name, op1, op2);
        txt return;}
    case 0x64:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", sound_effect::name, op1);
        txt return;}
    case 0x65:{
        uint16 op1 = s; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%04X, 0x%02X\n", sound_effect_ex::name, op1, op2);
        txt return;}
    case 0x66:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", as_66::name, op1);
        txt return;}
    case 0x67:{
        char *op1 = str;
        snprintf(&buffer[0], sizeof(buffer), "scraft_cut_in \"%s\"\n", op1);
        txt return;}
    case 0x68: // no operands
        sprintf(&buffer[0], "%s\n", nop_68::name);
        txt return;
    case 0x69: // no operands
        sprintf(&buffer[0], "%s\n", release_texture::name);
        txt return;
    case 0x6A:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X\n", load_schip::name, op1, op2, op3);
        txt return;}
    case 0x6B: // no operands
        sprintf(&buffer[0], "%s\n", reset_scraft_chip::name);
        txt return;
    case 0x6C: // no operands
        sprintf(&buffer[0], "%s\n", die::name);
        txt return;
    case 0x6D:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_6d::name, op1);
        txt return;}
    case 0x6E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_6e::name, op1);
        txt return;}
    case 0x6F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_6f::name, op1, op2);
        txt return;}
    case 0x70:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s; uint16 op4 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%04X, 0x%04X\n", as_70::name, op1, op2, op3, op4);
        txt return;}
    case 0x71:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_71::name, op1);
        txt return;}
    case 0x72:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_72::name, op1);
        txt return;}
    case 0x73:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_73::name, op1);
        txt return;}
    case 0x74:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_74::name, op1);
        txt return;}
    case 0x75:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", as_75::name, op1);
        txt return;}
    case 0x76:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_76::name, op1);
        txt return;}
    case 0x77:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_77::name, op1);
        txt return;}
    case 0x78:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", set_eff_state::name, op1);
        txt return;}
    case 0x79:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", nop_79::name, op1);
        txt return;}
    case 0x7A:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", craft_end::name, op1);
        txt return;}
    case 0x7B:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", set_craft_end_flag::name, op1);
        txt return;}
    case 0x7C:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_7c::name, op1, op2);
        txt return;}
    case 0x7D:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_7d::name, op1, op2);
        txt return;}
    case 0x7E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_7e::name, op1);
        txt return;}
    case 0x7F:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint8 op4 = b; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X, 0x%08X, 0x%02X, 0x%08X\n", blur_screen::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x80:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_80::name, op1);
        txt return;}
    case 0x81:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%04X\n", as_81::name, op1, op2, op3);
        txt return;}
    case 0x82: // no operands
        sprintf(&buffer[0], "%s\n", as_82::name);
        txt return;
    case 0x83:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", sort_target::name, op1);
        txt return;}
    case 0x84:{
        uint8 op1 = b; uint16 op2 = s; uint16 op3 = s; uint16 op4 = s; uint32 op5 = i; uint8 op6 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X, 0x%04X, 0x%04X, 0x%08X, 0x%02X\n", rotate_char::name, op1, op2, op3, op4, op5, op6);
        txt return;}
    case 0x85:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X\n", as_85::name, op1, op2, op3);
        txt return;}
    case 0x86:{
        uint16 op1 = s; uint16 op2 = s; uint16 op3 = s; uint8 op4 = b; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%04X, 0x%04X, 0x%04X, 0x%02X, 0x%08X\n", as_86::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x87:{
        uint16 op1 = s; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%04X, 0x%02X\n", as_87::name, op1, op2);
        txt return;}
    case 0x88:{
        uint16 op1 = s;
        sprintf(&buffer[0], "%s 0x%04X\n", voice::name, op1);
        txt return;}
    case 0x89:{
        uint8 target = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s\n", save_cur_pos::name, target_name);
        txt return;}
    case 0x8A:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", clone::name, op1, op2);
        txt return;}
    case 0x8B: // no operands
        sprintf(&buffer[0], "%s\n", use_item_begin::name);
        txt return;
    case 0x8C: // no operands
        sprintf(&buffer[0], "%s\n", use_item_end::name);
        txt return;
    case 0x8D:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", zoom::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0x8E:{
        uint8 op1 = b;
        if (op1 == 0x01)
        {
            op1 = b; char *op2 = str;
            snprintf(&buffer[0], 500, "load_x_file 0x%02X, \"%s\"\n", op1, op2);
            txt
        }
        else if (op1 == 0x0D)
        {
            op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i;
            sprintf(&buffer[0], "as_8e0d 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5, op6);
            txt
        }
        else
        {
            uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i;
            sprintf(&buffer[0], "as_8e 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5, op6);
            txt
        }
        return;}
    case 0x8F:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_8f::name, op1);
        txt return;}
    case 0x90:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_90::name, op1);
        txt return;}
    case 0x91:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_91::name, op1);
        txt return;}
    case 0x92:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint16 op6 = s; uint32 op7 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%08X\n", as_92::name, op1, op2, op3, op4, op5, op6, op7);
        txt return;}
    case 0x93:{
        uint8 op1 = b; uint8 op2 = b; char *op3 = str;
        snprintf(&buffer[0], sizeof(buffer), "as_93 0x%02X, 0x%02X, \"%s\"\n", op1, op2, op3);
        txt return;}
    case 0x94:{
        uint8 op1 = b; char *op2 = str; uint32 op3 = i;
        snprintf(&buffer[0], sizeof(buffer), "as_94 0x%02X, \"%s\", 0x%08X\n", op1, op2, op3);
        txt return;}
    case 0x95: // no operands
        sprintf(&buffer[0], "%s\n", as_95::name);
        txt return;
    case 0x96:{
        uint8 target = b; char *op2 = str; uint16 op3 = s;
        const char* const target_name = instruction::get_target_name(target);
        snprintf(&buffer[0], sizeof(buffer), "set_angle_target %s, \"%s\", 0x%04X\n", target_name,  op2, op3);
        txt return;}
    case 0x97:{
        uint32 op1 = i; uint16 op2 = s; uint16 op3 = s;
        sprintf(&buffer[0], "%s 0x%08X, 0x%04X, 0x%04X\n", move_angle::name, op1, op2, op3);
        txt return;}
    case 0x98:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", as_98::name, op1, op2, op3, op4);
        txt return;}
    case 0x99:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_99::name, op1);
        txt return;}
    case 0x9A:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", as_9a::name, op1);
        txt return;}
    case 0x9B:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_9b::name, op1);
        txt return;}
    case 0x9C:{
        uint8 target = b;
        const char* const target_name = instruction::get_target_name(target);
        sprintf(&buffer[0], "%s %s\n", reset_chip_status::name, target_name);
        txt return;}
    case 0x9D:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_9d::name, op1);
        txt return;}
    case 0x9E:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", set_timer::name, op1);
        txt return;}
    case 0x9F:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", set_battle_mode::name, op1, op2);
        txt return;}
    case 0xA0:{
        uint8 op1 = b; uint32 op2 = i; uint16 op3 = s; char *op4 = str;
        snprintf(&buffer[0], sizeof(buffer), "as_a0 0x%02X, 0x%08X, 0x%04X, \"%s\"\n", op1, op2, op3, op4);
        txt return;}
    case 0xA1:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%08X\n", as_a1::name, op1, op2);
        txt return;}
    case 0xA2:{
        uint8 op1 = b;
        sprintf(&buffer[0], "%s 0x%02X\n", as_a2::name, op1);
        txt return;}
    case 0xA3:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", as_a3::name, op1, op2);
        txt return;}
    case 0xA4:{
        uint8 op1 = b;
        if (op1 == 0x02)
        {
            uint16 op2 = s;
            sprintf(&buffer[0], "%s, 0x%04X\n", as_a402::name, op2);
        }
        else if (op1 == 0x00)
        {
            sprintf(&buffer[0], "%s\n", as_a400::name);
        }
        else if (op1 == 0x01)
        {
            sprintf(&buffer[0], "%s\n", as_a401::name);
        }
        else
        {
            opcode = 0xFF;
            printf("tried to read invalid opcode 0x%02X%02X at offset 0x%04X\n", opcode, op1, offset);
            throw(69);
        }
        txt return;}
    case 0xA5:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint8 op5 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%02X\n", as_a5::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0xA6:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", as_a6::name, op1, op2, op3, op4, op5, op6, op7);
        txt return;}
    case 0xA7:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", battle_effect_end::name, op1, op2);
        txt return;}
    case 0xA8:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X\n", damage_voice::name, op1, op2);
        txt return;}
    case 0xA9:{
        uint32 op1 = i;
        sprintf(&buffer[0], "%s 0x%08X\n", nop_a9::name, op1);
        txt return;}
    case 0xAA:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%08X, 0x%08X\n", as_aa::name, op1, op2);
        txt return;}
    case 0xAB:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%02X\n", as_ab::name, op1, op2, op3);
        txt return;}
    case 0xAC:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint8 op5 = b;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%02X\n", as_ac::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0xAD:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X\n", as_ad::name, op1, op2, op3);
        txt return;}
    case 0xAE:{
        uint16 op1 = s; uint32 op2 = i;
        sprintf(&buffer[0], "%s 0x%04X, 0x%08X\n", as_ae::name, op1, op2);
        txt return;}
    case 0xAF:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "%s 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", as_af::name, op1, op2, op3, op4, op5);
        txt return;}
    case 0xB0:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", as_b0::name, op1, op2);
        txt return;}
    case 0xB1:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "%s 0x%02X, 0x%04X\n", as_b1::name, op1, op2);
        txt return;}
    default:
        printf(text->data());
        printf("second_pass: %ls tried to read invalid opcode %X at offset 0x%X\n", ctx->filename.c_str(), opcode, offset);
        opcode = 0xFF;
		return;
#undef b
    }
    return;
#undef s
#undef i
#undef str
#undef txt
}