#include "as_instruction.h"
#include "context.h"

void instruction::second_pass(context* const ctx, string* const text)
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
        text->append("end_event\n");
        break;
    case 0x01:{
        s;
        sprintf(&buffer[0], "goto $%s\n", labelName);
        txt break;}
    case 0x02:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "select_sub_chip self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "select_sub_chip 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x03:{
        uint8 target = b; uint16 op2 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "deg self, 0x%04X\n", op2);
        else
            sprintf(&buffer[0], "deg 0x%02X, 0x%04X\n", target, op2);
        txt break;}
    case 0x04:{
        uint8 target = b; uint8 op2 = b; uint16 op3 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "as_04 self, 0x%02X, 0x%04X\n", op2, op3);
        else
            sprintf(&buffer[0], "as_04 0x%02X, 0x%02X, 0x%04X\n", target, op2, op3);
        txt break;}
    case 0x05:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "as_05 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x06:{
        uint32 op1 = i;
        sprintf(&buffer[0], "sleep 0x%08X\n", op1);
        txt break;}
    case 0x07: // no operands
        text->append("update\n");
        break;
    case 0x08:{
        uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        if (target1 == 0xFF)
            sprintf(&buffer[0], "teleport self, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", target2, op3, op4, op5);
        else
            sprintf(&buffer[0], "teleport 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", target1, target2, op3, op4, op5);
        txt break;}
    case 0x09:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "as_09 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x0A:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint32 op4 = i;
        sprintf(&buffer[0], "as_0a 0x%02X, 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x0B:{
        uint8 target = b; uint8 op2 = b; uint16 op3 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "turn self, 0x%02X, 0x%04X\n", op2, op3);
        else
            sprintf(&buffer[0], "turn 0x%02X, 0x%02X, 0x%04X\n", target, op2, op3);
        txt break;}
    case 0x0C:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s; uint16 op4 = s; uint8 op5 = b;
        sprintf(&buffer[0], "as_0c 0x%02X, 0x%02X, 0x%04X, 0x%04X, 0x%02X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x0D:{
        uint8 target1 = b; uint8 target2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint16 op6 = s; uint16 op7 = s;
        if (target1 == 0xFF && target2 == 0xFF)
            sprintf(&buffer[0], "jump self, self, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X\n", op3, op4, op5, op6, op7);
        else
            sprintf(&buffer[0], "jump 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X\n", target1, target2, op3, op4, op5, op6, op7);
        txt break;}
    case 0x0E:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint16 op5 = s; uint16 op6 = s;
        sprintf(&buffer[0], "drop_down 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X\n", op1, op2, op3, op4, op5, op6);
        txt break;}
    case 0x0F:{
        uint16 op1 = s; uint16 op2 = s;
        sprintf(&buffer[0], "jump_to_target 0x%04X, 0x%04X\n", op1, op2);
        txt break;}
    case 0x10:{
        uint16 op1 = s; uint16 op2 = s;
        sprintf(&buffer[0], "jump_back 0x%04X, 0x%04X\n", op1, op2);
        txt break;}
    case 0x11:{
        uint8 target = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i; uint8 op7 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "move self, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%02X\n", op2, op3, op4, op5, op6, op7);
        else
            sprintf(&buffer[0], "move 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%02X\n", target, op2, op3, op4, op5, op6, op7);
        txt break;}
    case 0x12:{
        uint16 op1 = s; char *op2 = str;
        snprintf(&buffer[0], sizeof(buffer), "add_effect 0x%04X, \"%s\"\n", op1, op2);
        txt break;}
    case 0x13:{
        uint16 op1 = s;
        sprintf(&buffer[0], "release_effect 0x%04X\n", op1);
        txt break;}
    case 0x14:{
        uint16 op1 = s;
        sprintf(&buffer[0], "as_14 0x%04X\n", op1);
        txt break;}
    case 0x15:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "wait_effect self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "wait_effect 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x16:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "finish_effect self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "finish_effect 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x17:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "cancel_effect self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "cancel_effect 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x18:{
        uint8 op1 = b; uint8 op2 = b; uint8 target = b; uint16 op4 = s; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        uint16 op8 = s; uint16 op9 = s; uint16 op10 = s; uint32 op11 = s; uint16 op12 = s; uint32 op13 = s; uint8 op14 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "show_effect 0x%02X, 0x%02X, self, 0x%04X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%02X\n",
             op1, op2, op4, op5, op6, op7, op8, op9, op10, op11, op12, op13, op14);
        else
            sprintf(&buffer[0], "show_effect 0x%02X, 0x%02X, 0x%02X, 0x%04X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%02X\n",
             op1, op2, target, op4, op5, op6, op7, op8, op9, op10, op11, op12, op13, op14);
        txt break;}
    case 0x19:{
        uint8 op1 = b; uint8 op2 = b; char *op3 = str; uint16 op4 = s; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        uint16 op8 = s; uint16 op9 = s; uint16 op10 = s; uint32 op11 = s; uint16 op12 = s; uint32 op13 = s; uint8 op14 = b;
        snprintf(&buffer[0], 500, "show_3d_effect 0x%02X, 0x%02X, \"%s\", 0x%04X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%02X\n",
         op1, op2, op3, op4, op5, op6, op7, op8, op9, op10, op11, op12, op13, op14);
        txt break;}
    case 0x1A:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "as_1a 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    case 0x1B:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "select_chip self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "select_chip 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x1C:{
        uint8 op1 = b;
        sprintf(&buffer[0], "damage 0x%02X\n", op1);
        txt break;}
    case 0x1D:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "damage_anime 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x1E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_1e 0x%08X\n", op1);
        txt break;}
    case 0x1F:{
        uint16 op1 = s; uint16 op2 = s; uint8 op3 = b;
        sprintf(&buffer[0], "as_1f 0x%04X, 0x%04X, 0x%02X\n", op1, op2, op3);
        txt break;}
    case 0x20:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint8 op4 = b; uint32 op5 = i; uint32 op6 = i;
        sprintf(&buffer[0], "as_20 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5, op6);
        txt break;}
    case 0x21:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "as_21 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x22:{
        uint8 target = b; uint8 op2 = b; s; uint8 op4 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "begin_thread self, 0x%02X, $%s, 0x%02X\n", op2, labelName , op4);
        else
            sprintf(&buffer[0], "begin_thread 0x%02X, 0x%02X, $%s, 0x%02X\n", target, op2, labelName , op4);
        txt break;}
    case 0x23:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "wait_thread self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "wait_thread 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x24:{
        uint8 op1 = b; uint8 target = b; uint16 op3 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "set_chip_mode_flag 0x%02X, self, 0x%04X\n", op1, op3);
        else
            sprintf(&buffer[0], "set_chip_mode_flag 0x%02X, 0x%02X, 0x%04X\n", op1, target, op3);
        txt break;}
    case 0x25:{
        uint8 op1 = b; uint8 target = b; uint16 op3 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "clear_chip_mode_flag 0x%02X, self, 0x%04X\n", op1, op3);
        else
            sprintf(&buffer[0], "clear_chip_mode_flag 0x%02X, 0x%02X, 0x%04X\n", op1, target, op3);
        txt break;}
    case 0x26:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "as_26 0x%02X, 0x%02X, 0x%04X\n", op1, op2, op3);
        txt break;}
    case 0x27:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "as_27 0x%02X, 0x%02X, 0x%04X\n", op1, op2, op3);
        txt break;}
    case 0x28:{
        uint8 op1 = b; char *op2 = str; uint32 op3 = i;
        snprintf(&buffer[0], 500, "char_say 0x%02X, \"%s\", 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x29:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_29 0x%02X\n", op1);
        txt break;}
    case 0x2A:{
        char *op1 = str; uint32 op2 = i;
        snprintf(&buffer[0], 500, "tip_text \"%s\", 0x%08X\n", op1, op2);
        txt break;}
    case 0x2B: // no operands
        text->append("as_2b\n");
        break;
    case 0x2C:{
        uint8 target = b; uint16 op2 = s; uint16 op3 = s;
        if (target == 0xFF)
            sprintf(&buffer[0], "shadow_begin self, 0x%04X, 0x%04X\n", op2, op3);
        else
            sprintf(&buffer[0], "shadow_begin 0x%02X, 0x%04X, 0x%04X\n", target, op2, op3);
        txt break;}
    case 0x2D:{
        uint8 target = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "shadow_end self\n");
        else
            sprintf(&buffer[0], "shadow_end 0x%02X\n", target);
        txt break;}
    case 0x2E:{
        uint8 target = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        if (target == 0xFF)
            sprintf(&buffer[0], "shake_char self, 0x%08X, 0x%08X, 0x%08X\n", op2, op3, op4);
        else
            sprintf(&buffer[0], "shake_char 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", target, op2, op3, op4);
        txt break;}
    case 0x2F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "suspend_thread 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x30:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_30 0x%02X", op1);
        txt 
        do
        {
            char *string = str;
            if (*string == 0)
            {
                text->append("\n");
                break;
            }
            snprintf(&buffer[0], sizeof(buffer), ", \"%s\"", string);
            txt
        }
        while (1);
        break;}
    case 0x31:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_31 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x32:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_32 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x33:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_33 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x34: // no operands
        text->append("as_34\n");
        break;
    case 0x35:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "keep_angle 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x36:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "as_36 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x37:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "rotation_angle 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x38:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "rotation_angle_v 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x39:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "set_angle 0x%08X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x3A:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "tilt_angle 0x%08X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x3B:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "rotation_angle_h 0x%08X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x3C:{
        uint16 op1 = s; uint32 op2 = i;
        sprintf(&buffer[0], "as_3c 0x%04X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x3D:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "shake_screen 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x3E:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "as_3e 0x%08X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x3F:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_3f 0x%02X\n", op1);
        txt break;}
    case 0x40:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_40 0x%02X\n", op1);
        txt break;}
    case 0x41:{
        uint8 target = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "lock_angle self\n");
        else
            sprintf(&buffer[0], "lock_angle 0x%02X\n", target);
        txt break;}
    case 0x42:{
        uint8 op1 = b; uint32 op2 = i; uint8 op3 = b;
        sprintf(&buffer[0], "as_42 0x%02X, 0x%08X, 0x%02X\n", op1, op2, op3);
        txt break;}
    case 0x43:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "set_bk_color 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x44:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "zoom_angle 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x45:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_45 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x46:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "as_46 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x47:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_47 0x%02X\n", op1);
        txt break;}
    case 0x48:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_48 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x49:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "set_control 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    case 0x4A:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_4a 0x%02X\n", op1);
        txt break;}
    case 0x4B:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; s;
        sprintf(&buffer[0], "random 0x%02X, 0x%02X, 0x%08X, $%s\n", op1, op2, op3, labelName);
        txt break;}
    case 0x4C:{
        s;
        sprintf(&buffer[0], "loop_target_beg $%s\n", labelName);
        txt break;}
    case 0x4D: // no operands
        text->append("reset_loop_target\n");
        break;
    case 0x4E: // no operands
        text->append("loop_target_end\n");
        break;
    case 0x4F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_4f 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x50:{
        s;
        sprintf(&buffer[0], "call $%s\n", labelName);
        txt break;}
    case 0x51: // no operands
        text->append("ret\n");
        break;
    case 0x52:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_52 0x%02X\n", op1);
        txt break;}
    case 0x53:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_53 0x%02X\n", op1);
        txt break;}
    case 0x54:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_54 0x%02X\n", op1);
        txt break;}
    case 0x55:{
        uint16 op1 = s;
        sprintf(&buffer[0], "magic_cast_begin 0x%04X\n", op1);
        txt break;}
    case 0x56: // no operands
        text->append("magic_cast_end\n");
        break;
    case 0x57:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_57 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x58:{
        uint8 op1 = b;
        sprintf(&buffer[0], "beat_back 0x%02X\n", op1);
        txt break;}
    case 0x59:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "as_59 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    case 0x5A:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "as_5a 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x5B:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_5b 0x%08X\n", op1);
        txt break;}
    case 0x5C:{
        uint8 target = b; uint32 op2 = i;
        if (target == 0xFF)
            sprintf(&buffer[0], "show self, 0x%08X\n", op2);
        else
            sprintf(&buffer[0], "show 0x%02X, 0x%08X\n", target, op2);
        txt break;}
    case 0x5D:{
        uint8 target = b; uint32 op2 = i;
        if (target == 0xFF)
            sprintf(&buffer[0], "hide self, 0x%08X\n", op2);
        else
            sprintf(&buffer[0], "hide 0x%02X, 0x%08X\n", target, op2);
        txt break;}
    case 0x5E:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_5e 0x%02X\n", op1);
        txt break;}
    case 0x5F:{
        uint8 target = b; uint8 op2 = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "as_5f self, 0x%02X\n", op2);
        else
            sprintf(&buffer[0], "as_5f 0x%02X, 0x%02X\n", target, op2);
        txt break;}
    case 0x60:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_60 0x%02X\n", op1);
        txt break;}
    case 0x61:{
        uint32 op1 = i;
        sprintf(&buffer[0], "set_battle_speed 0x%08X\n", op1);
        txt break;}
    case 0x62:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint8 op4 = b; uint16 op5 = s;
        sprintf(&buffer[0], "as_62 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%04X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x63:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_63 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x64:{
        uint16 op1 = s;
        sprintf(&buffer[0], "sound_effect 0x%04X\n", op1);
        txt break;}
    case 0x65:{
        uint16 op1 = s; uint8 op2 = b;
        sprintf(&buffer[0], "sound_effect_ex 0x%04X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x66:{
        uint16 op1 = s;
        sprintf(&buffer[0], "as_66 0x%04X\n", op1);
        txt break;}
    case 0x67:{
        char *op1 = str;
        snprintf(&buffer[0], sizeof(buffer), "scraft_cut_in \"%s\"\n", op1);
        txt break;}
    case 0x68: // no operands
        text->append("nop_68\n");
        break;
    case 0x69: // no operands
        text->append("release_texture\n");
        break;
    case 0x6A:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i;
        sprintf(&buffer[0], "load_schip 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x6B: // no operands
        text->append("reset_scraft_chip\n");
        break;
    case 0x6C: // no operands
        text->append("die\n");
        break;
    case 0x6D:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_6d 0x%08X\n", op1);
        txt break;}
    case 0x6E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_6e 0x%08X\n", op1);
        txt break;}
    case 0x6F:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_6f 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x70:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s; uint16 op4 = s;
        sprintf(&buffer[0], "as_70 0x%02X, 0x%02X, 0x%04X, 0x%04X\n", op1, op2, op3, op4);
        txt break;}
    case 0x71:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_71 0x%02X\n", op1);
        txt break;}
    case 0x72:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_72 0x%02X\n", op1);
        txt break;}
    case 0x73:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_73 0x%02X\n", op1);
        txt break;}
    case 0x74:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_74 0x%02X\n", op1);
        txt break;}
    case 0x75:{
        uint16 op1 = s;
        sprintf(&buffer[0], "as_75 0x%04X\n", op1);
        txt break;}
    case 0x76:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_76 0x%02X\n", op1);
        txt break;}
    case 0x77:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_77 0x%02X\n", op1);
        txt break;}
    case 0x78:{
        uint8 op1 = b;
        sprintf(&buffer[0], "set_eff_state 0x%02X\n", op1);
        txt break;}
    case 0x79:{
        uint8 op1 = b;
        sprintf(&buffer[0], "nop_79 0x%02X\n", op1);
        txt break;}
    case 0x7A:{
        uint8 op1 = b;
        sprintf(&buffer[0], "craft_end 0x%02X\n", op1);
        txt break;}
    case 0x7B:{
        uint16 op1 = s;
        sprintf(&buffer[0], "set_craft_end_flag 0x%04X\n", op1);
        txt break;}
    case 0x7C:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_7c 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x7D:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_7d 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0x7E:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_7e 0x%08X\n", op1);
        txt break;}
    case 0x7F:{
        uint32 op1 = i; uint32 op2 = i; uint32 op3 = i; uint8 op4 = b; uint32 op5 = i;
        sprintf(&buffer[0], "blur_screen 0x%08X, 0x%08X, 0x%08X, 0x%02X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x80:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_80 0x%08X\n", op1);
        txt break;}
    case 0x81:{
        uint8 op1 = b; uint8 op2 = b; uint16 op3 = s;
        sprintf(&buffer[0], "as_81 0x%02X, 0x%02X, 0x%04X\n", op1, op2, op3);
        txt break;}
    case 0x82: // no operands
        text->append("as_82\n");
        break;
    case 0x83:{
        uint8 op1 = b;
        sprintf(&buffer[0], "sort_target 0x%02X\n", op1);
        txt break;}
    case 0x84:{
        uint8 op1 = b; uint16 op2 = s; uint16 op3 = s; uint16 op4 = s; uint32 op5 = i; uint8 op6 = b;
        sprintf(&buffer[0], "rotate_char 0x%02X, 0x%04X, 0x%04X, 0x%04X, 0x%08X, 0x%02X\n", op1, op2, op3, op4, op5, op6);
        txt break;}
    case 0x85:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "as_85 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x86:{
        uint16 op1 = s; uint16 op2 = s; uint16 op3 = s; uint8 op4 = b; uint32 op5 = i;
        sprintf(&buffer[0], "as_86 0x%04X, 0x%04X, 0x%04X, 0x%02X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0x87:{
        uint16 op1 = s; uint8 op2 = b;
        sprintf(&buffer[0], "as_87 0x%04X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x88:{
        uint16 op1 = s;
        sprintf(&buffer[0], "voice 0x%04X\n", op1);
        txt break;}
    case 0x89:{
        uint8 target = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "save_cur_pos self\n");
        else
            sprintf(&buffer[0], "save_cur_pos 0x%02X\n", target);
        txt break;}
    case 0x8A:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "clone 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0x8B: // no operands
        text->append("use_item_begin\n");
        break;
    case 0x8C: // no operands
        text->append("use_item_end\n");
        break;
    case 0x8D:{
        uint8 op1 = b; uint32 op2 = i; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "zoom 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
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
        break;}
    case 0x8F:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_8f 0x%02X\n", op1);
        txt break;}
    case 0x90:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_90 0x%02X\n", op1);
        txt break;}
    case 0x91:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_91 0x%02X\n", op1);
        txt break;}
    case 0x92:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i; uint16 op6 = s; uint32 op7 = i;
        sprintf(&buffer[0], "as_92 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%04X, 0x%08X\n", op1, op2, op3, op4, op5, op6, op7);
        txt break;}
    case 0x93:{
        uint8 op1 = b; uint8 op2 = b; char *op3 = str;
        snprintf(&buffer[0], 500, "as_93 0x%02X, 0x%02X, \"%s\"\n", op1, op2, op3);
        txt break;}
    case 0x94:{
        uint8 op1 = b; char *op2 = str; uint32 op3 = i;
        snprintf(&buffer[0], 500, "as_94 0x%02X, \"%s\", 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0x95: // no operands
        text->append("as_95\n");
        break;
    case 0x96:{
        uint8 target = b; char *op2 = str; uint16 op3 = s;
        if (target == 0xFF)
            snprintf(&buffer[0], 500, "set_angle_target self, \"%s\", 0x%04X\n", op2, op3);
        else
            snprintf(&buffer[0], 500, "set_angle_target 0x%02X, \"%s\", 0x%04X\n", target, op2, op3);
        txt break;}
    case 0x97:{
        uint32 op1 = i; uint16 op2 = s; uint16 op3 = s;
        sprintf(&buffer[0], "move_angle 0x%08X, 0x%04X, 0x%04X\n", op1, op2, op3);
        txt break;}
    case 0x98:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i;
        sprintf(&buffer[0], "as_98 0x%02X, 0x%02X, 0x%08X, 0x%08X\n", op1, op2, op3, op4);
        txt break;}
    case 0x99:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_99 0x%02X\n", op1);
        txt break;}
    case 0x9A:{
        uint32 op1 = i;
        sprintf(&buffer[0], "as_9a 0x%08X\n", op1);
        txt break;}
    case 0x9B:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_9b 0x%02X\n", op1);
        txt break;}
    case 0x9C:{
        uint8 target = b;
        if (target == 0xFF)
            sprintf(&buffer[0], "reset_chip_status self\n");
        else
            sprintf(&buffer[0], "reset_chip_status 0x%02X\n", target);
        txt break;}
    case 0x9D:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_9d 0x%02X\n", op1);
        txt break;}
    case 0x9E:{
        uint8 op1 = b;
        sprintf(&buffer[0], "set_timer 0x%02X\n", op1);
        txt break;}
    case 0x9F:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "set_battle_mode 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0xA0:{
        uint8 op1 = b; uint32 op2 = i; uint16 op3 = s; char *op4 = str;
        snprintf(&buffer[0], 500, "as_a0 0x%02X, 0x%08X, 0x%04X, \"%s\"\n", op1, op2, op3, op4);
        txt break;}
    case 0xA1:{
        uint8 op1 = b; uint32 op2 = i;
        sprintf(&buffer[0], "as_b1 0x%02X, 0x%08X\n", op1, op2);
        txt break;}
    case 0xA2:{
        uint8 op1 = b;
        sprintf(&buffer[0], "as_a2 0x%02X\n", op1);
        txt break;}
    case 0xA3:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "as_a3 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0xA4:{
        uint8 op1 = b;
        if (op1 == 0x02)
        {
            uint16 op2 = s;
            sprintf(&buffer[0], "as_a402, 0x%04X", op2);
            text->append("as_a402\n");
        }
        else if (op1 == 0x00)
        {
            text->append("as_a400\n");
        }
        else if (op1 == 0x01)
        {
            text->append("as_a401\n");
        }
        else
        {
            opcode = 0xFF;
            printf("tried to read invalid opcode 0x%02X%02X at offset 0x%04X\n", opcode, op1, offset);
        }
        break;}
    case 0xA5:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint8 op5 = b;
        sprintf(&buffer[0], "as_a5 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%02X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0xA6:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b; uint32 op4 = i; uint32 op5 = i; uint32 op6 = i; uint32 op7 = i;
        sprintf(&buffer[0], "as_a6 0x%02X, 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5, op6, op7);
        txt break;}
    case 0xA7:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "battle_effect_end 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    case 0xA8:{
        uint8 op1 = b; uint8 op2 = b;
        sprintf(&buffer[0], "damage_voice 0x%02X, 0x%02X\n", op1, op2);
        txt break;}
    case 0xA9:{
        uint32 op1 = i;
        sprintf(&buffer[0], "nop_a9 0x%08X\n", op1);
        txt break;}
    case 0xAA:{
        uint32 op1 = i; uint32 op2 = i;
        sprintf(&buffer[0], "as_aa 0x%08X, 0x%08X\n", op1, op2);
        txt break;}
    case 0xAB:{
        uint8 op1 = b; uint8 op2 = b; uint8 op3 = b;
        sprintf(&buffer[0], "as_ad 0x%02X, 0x%02X, 0x%02X\n", op1, op2, op3);
        txt break;}
    case 0xAC:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint8 op5 = b;
        sprintf(&buffer[0], "as_ac 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%02X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0xAD:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i;
        sprintf(&buffer[0], "as_ad 0x%02X, 0x%02X, 0x%08X\n", op1, op2, op3);
        txt break;}
    case 0xAE:{
        uint16 op1 = s; uint32 op2 = i;
        sprintf(&buffer[0], "as_ae 0x%04X, 0x%08X\n", op1, op2);
        txt break;}
    case 0xAF:{
        uint8 op1 = b; uint8 op2 = b; uint32 op3 = i; uint32 op4 = i; uint32 op5 = i;
        sprintf(&buffer[0], "as_af 0x%02X, 0x%02X, 0x%08X, 0x%08X, 0x%08X\n", op1, op2, op3, op4, op5);
        txt break;}
    case 0xB0:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "as_b0 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    case 0xB1:{
        uint8 op1 = b; uint16 op2 = s;
        sprintf(&buffer[0], "as_b1 0x%02X, 0x%04X\n", op1, op2);
        txt break;}
    default:
        printf(text->data());
        printf("second_pass: %ls tried to read invalid opcode %X at offset 0x%X\n", ctx->filename.c_str(), opcode, offset);
        opcode = 0xFF;
    }
    return;
#undef b
#undef s
#undef i
#undef str
#undef txt
}