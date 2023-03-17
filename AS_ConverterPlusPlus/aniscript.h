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
    uint16 event_label_offsets[TOTAL_EVENT_LABELS];
    static constexpr char label_names[TOTAL_EVENT_LABELS][25] = {"FUN01", "FUN02", "FUN03", "FUN04", "FUN05", "FUN06", "FUN07", "FUN08",
        "FUN09", "FUN0A", "FUN0B", "FUN0C", "FUN0D", "FUN0E", "FUN0F", "FUN10", "FUN11", "FUN12", "FUN13", "FUN14", "FUN15",
        "FUN16", "FUN17", "FUN18", "FUN19", "FUN1A", "FUN1B", "FUN1C", "FUN1D", "FUN1E", "FUN1F", "FUN20", "FUN21", "FUN22"};
    std::list<string>::iterator chip_section;
    std::list<string>::iterator model_3d_section;
    std::list<string>::iterator bones_3d_section;
    std::list<string>::iterator unk_bytes_section;
    std::list<string>::iterator text_section;
    present_sections sections;
    char* assembled_binary = nullptr;
    size_t assembled_binary_size;
    size_t cur_offset;

    aniscript(){sections.init();}

    ~aniscript(){if(assembled_binary != nullptr) delete[] assembled_binary;}

    bool ParseFromBinary(binary_context* const ctx, string* const text);
    bool CompileFromText(text_context* const ctx);
    void new_label(text_context* const ctx, string label_name, char& event_funct_count, bool* const found_event_label);
    void new_section(text_context* const ctx, string section_name, std::list<string>::iterator it);
    bool validate_chips_section(text_context* const ctx, size_t& cur_offset);
    bool validate_model_section(text_context* const ctx, size_t& cur_offset);
    bool validate_bones_section(text_context* const ctx, size_t& cur_offset);
    bool validate_unk_bytes_section(text_context* const ctx, size_t& cur_offset);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);
    bool validate_text_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx);
    bool write_binary_chip_line(std::list<string>::iterator &line);
    bool write_binary_chips_section(text_context* const ctx);
    bool write_binary_model_line(std::list<string>::iterator &line);
    bool write_binary_model_section(text_context* const ctx);
    bool write_binary_bones_unk00(std::list<string>::iterator &line);
    bool write_binary_bones_line(std::list<string>::iterator &line);
    bool write_binary_bones_section(text_context* const ctx);
    bool write_binary_unk_bytes_line(std::list<string>::iterator &line);
    bool write_binary_unk_bytes_section(text_context* const ctx);

    void u8(const uint8 val){*(uint8*)(assembled_binary + cur_offset) = val; cur_offset += 1;}
    void u16(const uint16 val){*(uint16*)(assembled_binary + cur_offset) = val; cur_offset += 2;}
    void u32(const uint32 val){*(uint32*)(assembled_binary + cur_offset) = val; cur_offset += 4;}
    inline void str(const char* const val)
    {
        size_t str_offset = 1; //skip '\"'
        do
        {
            *(char*)(assembled_binary + cur_offset) = val[str_offset];
            cur_offset += 1;
            str_offset += 1;
        } while (val[str_offset] != '\"');
    }

    static unsigned long long num_from_str(const char* const str, char length)
    {
        char* end = nullptr;
        auto num = std::strtoul(str, &end, 0);
        if (errno == ERANGE)
        {
            errno = 0;
            printf("range error: %s \n", str);
            return ULLONG_MAX;
        }
        if(end == nullptr || *end != '\0')
        {
            printf("Cannot convert %s to an integer\n", str);
            return ULLONG_MAX;
        }
        switch(length)
        {
        case 1:
            if(num > 0xFF)
            {
                printf("range error: 0x%X bigger than max size of byte (0xFF)\n", num);
                return ULLONG_MAX;
            }
            return num;
        case 2:
            if(num > 0xFFFF)
            {
                printf("range error: 0x%X bigger than max size of int16 (0xFFFF)\n", num);
                return ULLONG_MAX;
            }
            return num;
        case 4:
            if(num > 0xFFFFFFFF)
            {
                printf("range error: 0x%X bigger than max size of int32 (0xFFFFFFFF)\n", num);
                return ULLONG_MAX;
            }
            return num;
        default:
            printf("[DEBUG] Invalid size specifier %d", length);
            return ULLONG_MAX;
        }
    }
};