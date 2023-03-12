#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "binary_context.h"
#include "text_context.h"
#include "as_instruction.h"

#define TOTAL_EVENT_LABELS 34
#define MANDATORY_EVENT_LABELS 30 // at least 30 have to be present

template<size_t N>
using FCArray = std::vector<std::array<char,N>>; // fixed size char array vector

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
    uint8 unk00;
    vector<char*> bones_3d; // an array of strings terminated by a null-string

    void fromBuffer(binary_context* const ctx)
    {
        unk00 = ctx->u8();
        ctx->cstring_array(&bones_3d);
    }
};
struct unkBytes
{
    uint8 unk00;
    uint8 unk01;
};

union present_sections
{
    bool chips : 1;
    bool model : 1;
    bool bones : 1;
    bool unk_bytes : 1;
    bool text : 1;

    void init()
    {
        chips = false;
        model = false;
        bones = false;
        unk_bytes = false;
        text = false;
    }
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
    std::unordered_set<uint8> targets_in_file; // all files make use of 0xFF (self), but there's a lot more possible targets. we want to %define only when needed.
    std::unordered_map<uint16, FCArray<100>> labels;
    std::unordered_map<string, uint16> label_to_offset_map;
    static constexpr char label_names[TOTAL_EVENT_LABELS][25] = {"FUN01", "FUN02", "FUN03", "FUN04", "FUN05", "FUN06", "FUN07", "FUN08",
        "FUN09", "FUN0A", "FUN0B", "FUN0C", "FUN0D", "FUN0E", "FUN0F", "FUN10", "FUN11", "FUN12", "FUN13", "FUN14", "FUN15",
        "FUN16", "FUN17", "FUN18", "FUN19", "FUN1A", "FUN1B", "FUN1C", "FUN1D", "FUN1E", "FUN1F", "FUN20", "FUN21", "FUN22"};
    std::list<string>::iterator chip_section;
    std::list<string>::iterator model_3d_section;
    std::list<string>::iterator bones_3d_section;
    std::list<string>::iterator unk_bytes_section;
    std::list<string>::iterator text_section;
    present_sections sections;

    aniscript(){sections.init();}

    bool ParseFromBinary(binary_context* const ctx, string* const text);
    bool CompileFromText(text_context* const ctx, std::vector<char>* const binary);
    void new_label(text_context* const ctx, string label_name, char& event_funct_count, bool* const found_event_label);
    void new_section(text_context* const ctx, string section_name, std::list<string>::iterator it);
    bool validate_chips_section(text_context* const ctx, size_t& cur_offset);
    bool validate_model_section(text_context* const ctx, size_t& cur_offset);
    bool validate_bones_section(text_context* const ctx, size_t& cur_offset);
    bool validate_unk_bytes_section(text_context* const ctx, size_t& cur_offset);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);
    bool validate_text_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx);
};