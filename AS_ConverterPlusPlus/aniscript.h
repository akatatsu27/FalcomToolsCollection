#include "directives.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <regex>
#include "context.h"
#include "as_instruction.h"
#pragma once

#define byte unsigned char

using std::vector;
using std::string;
using std::ios;

struct chip
{
    uint16 index;
    uint16 datatable;
};
struct chip_entry
{
    chip ch;
    chip cp;
};
struct bones
{
    byte unk00;
    vector<string> bones_3d; // an array of strings terminated by a null-string

    void fromBuffer(context* const ctx)
    {
        unk00 = ctx->u8();
        ctx->cstring_array(&bones_3d);
    }
};
struct unkBytes
{
    byte unk00;
    byte unk01;
};


struct aniscript
{
    uint16 craft_offset_table_offset;
    uint16 craft_offset_table_offset_end;
    uint16 bones_3d_offset;
    vector<chip_entry> chip_entries; // pairs of _ch and _cp, terminated by 0xFFFF
    vector<string> model_3d; // an array of strings terminated by a null-string
    bones bones_3d; // only present when bones_3d_offset isn't NULL!!!
    vector<uint16> function_offset_table; // size = (craft_offset_table_offset_end - craft_offset_table_offset)/2
    unkBytes unk_bytes[8];
    vector<instruction> instructions;

    aniscript(){}

    bool ParseFromBinary(context* const ctx, string* text)
    {
        craft_offset_table_offset = ctx->u16();
        craft_offset_table_offset_end = ctx->u16();
        bones_3d_offset = ctx->u16();
        chip_entry entry;
        do
        {
            entry.ch.index = ctx->u16();
            entry.ch.datatable = ctx->u16();
            if(entry.ch.index == 0xFFFF) break;
            entry.cp.index = ctx->u16();
            entry.cp.datatable = ctx->u16();
            chip_entries.push_back(entry);
        }while(1);
        text->append("%define self 0xFF\n%macro subchip_update 3\nselect_sub_chip $1, $2\nsleep $3\nupdate\n%endmacro\n");
        text->append("section chip_entries:\n");
        char buffer[100];
        for(byte i = 0; i < chip_entries.size(); i++)
        {
            chip_entry chp = chip_entries[i];
            snprintf(&buffer[0], 100, "chip%d:\nDW 0x%04X, 0x%04X\nDW 0x%04X, 0x%04X\n", i, chp.ch.index, chp.ch.datatable, chp.cp.index, chp.cp.datatable);
            text->append(&buffer[0]);
        }
        text->append("section model_3d:\n");
        ctx->cstring_array(&model_3d);
        for(byte i = 0; i < model_3d.size(); i++)
        {
            text->append("DB \"");
            text->append(model_3d[i]);
            text->append("\", 0x00\n");
        }
        text->append("DB 0x00\n"); // array terminator
        text->append("section bones_3d:\n");
        if(bones_3d_offset != 0)
        {
            bones_3d.fromBuffer(ctx);
            text->append("DB ");
            snprintf(&buffer[0], 10, "%#04X\n", bones_3d.unk00);
            text->append(&buffer[0]);
            for(char i = 0; i < bones_3d.bones_3d.size(); i++)
            {
                text->append("DB \"");
                text->append(bones_3d.bones_3d[i]);
                text->append("\", 0x00\n");
            }
            text->append("DB 0x00\n"); // array terminator
        }
        do
        {
            function_offset_table.push_back(ctx->u16());
        } while (ctx->position() != craft_offset_table_offset_end);
        text->append("section unk_bytes:\n");
        for(byte i = 0; i < 8; i++)
        {
            unk_bytes[i].unk00 = ctx->u8();
            unk_bytes[i].unk01 = ctx->u8();
            snprintf(&buffer[0], 100, "DB 0x%02X, 0x%02X\n", unk_bytes[i].unk00, unk_bytes[i].unk01);
            text->append(&buffer[0]);
        }
        text->append("section .text:\n");
        do
        {
            //combat events trigger specific functions identified by the index
            //in the function_offset_table
            //search for the current offset in the function_offset table,
            //and if it exists place a label
            for(char i = 0; i < function_offset_table.size(); i++)
            {
                if(function_offset_table[i] == ctx->position())
                {
                    snprintf(&buffer[0], 10, "FUN%02X:\n", i);
                    text->append(&buffer[0]);
                }
            }
            instruction instr = instruction::parse(ctx, text);
            if(instr.opcode == 0xFF) break;
        } while (ctx->position() < ctx->size());
        char* foo = text->data();
        static std::regex longform("select_sub_chip ([a-zA-Z0-9_]+), ([a-zA-Z0-9_]+)\nsleep ([a-zA-Z0-9_]+)\nupdate");
        text->assign((string)std::regex_replace(*text, longform, "subchip_update $1, $2, $3"));
        return true;
    }
};

