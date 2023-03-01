#include "directives.h"
#include <map>
#include <unordered_map>
#include "binary_context.h"
#include "text_context.h"
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
    vector<char*> bones_3d; // an array of strings terminated by a null-string

    void fromBuffer(binary_context* const ctx)
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
    vector<char*> model_3d; // an array of strings terminated by a null-string
    bones bones_3d; // only present when bones_3d_offset isn't NULL!!!
    vector<uint16> function_offset_table; // size = (craft_offset_table_offset_end - craft_offset_table_offset)/2
    unkBytes unk_bytes[8];
    std::map<uint16, instruction> instructions;
    std::unordered_map<uint16, FCArray<100>> labels;
    std::unordered_map<char*, uint16> label_to_offset_map;

    aniscript(){}

    bool ParseFromBinary(binary_context* const ctx, string* const text);
    bool CompileFromText(text_context* const ctx, std::vector<char>* const binary);
};

