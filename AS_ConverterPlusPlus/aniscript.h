#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "binary_context.h"
#include "text_context.h"
#include "as_instruction.h"
#include "base_aniscript.h"

#define TOTAL_EVENT_LABELS 34
#define MANDATORY_EVENT_LABELS 30 // at least 30 have to be present

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
struct sprite_offset
{
    uint8 horizontal_offset;
    uint8 vertical_offset;
};

union present_sections
{
    bool chips : 1;
    bool model : 1;
    bool bones : 1;
    bool sprite_offsets : 1;
    bool text : 1;

    void init()
    {
        chips = false;
        model = false;
        bones = false;
        sprite_offsets = false;
        text = false;
    }
};

struct aniscript : public base_aniscript
{
    uint16 event_offset_table_offset;
    uint16 event_offset_table_offset_end;
    uint16 bones_3d_offset;
    vector<chip_entry> chip_entries; // pairs of _ch and _cp, terminated by 0xFFFF
    vector<char*> model_3d; // an array of strings terminated by a null-string
    bones bones_3d; // only present when bones_3d_offset isn't NULL!!!
    vector<uint16> function_offset_table; // size = (craft_offset_table_offset_end - craft_offset_table_offset)/2
    sprite_offset sprite_offsets[8];
    //std::map<uint16, instruction> instructions;
    uint16 event_label_offsets[TOTAL_EVENT_LABELS];
    static constexpr char label_names[TOTAL_EVENT_LABELS][25] = {"OnMagicEffect", "OnStand", "OnMove", "OnUnderAttack", "OnDead", "OnNormalAttack", "OnMagicChant",
        "OnMagicCast", "OnWin", "FUN0A", "OnUseItem", "OnStun", "FUN0D", "FUN0E", "FUN0F", "FUN10", "FUN11", "FUN12", "FUN13", "FUN14",
        "FUN15", "FUN16", "FUN17", "FUN18", "FUN19", "FUN1A", "FUN1B", "FUN1C", "FUN1D", "FUN1E", "FUN1F", "FUN20", "FUN21", "FUN22"};
    std::list<string>::iterator chip_section;
    std::list<string>::iterator model_3d_section;
    std::list<string>::iterator bones_3d_section;
    std::list<string>::iterator sprite_offsets_section;
    std::list<string>::iterator text_section;
    present_sections sections;
    

    aniscript(){sections.init();}

    ~aniscript(){if(assembled_binary != nullptr) delete[] assembled_binary;}

    bool parse_from_binary(binary_context* const ctx, string* const text) override;
    bool compile_from_text(text_context* const ctx) override;
    void new_label(text_context* const ctx, string label_name, char& event_funct_count, bool* const found_event_label);
    void new_section(text_context* const ctx, string section_name, std::list<string>::iterator it);
    bool validate_chips_section(text_context* const ctx, size_t& cur_offset);
    bool validate_model_section(text_context* const ctx, size_t& cur_offset);
    bool validate_bones_section(text_context* const ctx, size_t& cur_offset);
    bool validate_sprite_offsets_section(text_context* const ctx, size_t& cur_offset);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);
    bool write_binary_chip_line(std::list<string>::iterator &line);
    bool write_binary_chips_section(text_context* const ctx);
    bool write_binary_model_line(std::list<string>::iterator &line);
    bool write_binary_model_section(text_context* const ctx);
    bool write_binary_bones_unk00(std::list<string>::iterator &line);
    bool write_binary_bones_line(std::list<string>::iterator &line);
    bool write_binary_bones_section(text_context* const ctx);
    bool write_binary_sprite_offsets_line(std::list<string>::iterator &line);
    bool write_binary_sprite_offsets_section(text_context* const ctx);
};