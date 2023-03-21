#include "base_aniscript.h"

#define ASMAG_TOTAL_FUNCTIONS 0x84

union asmag_sections
{
    bool super_special_section : 1;
    bool text : 1;

    void init()
    {
        super_special_section = false;
        text = false;
    }
};

struct asmag : public base_aniscript
{
    uint16 function_offset_table_offset;
    vector<uint16> function_offset_table;
    std::list<string>::iterator super_special_section_section;
    std::list<string>::iterator text_section;
    uint16 magic_label_offsets[ASMAG_TOTAL_FUNCTIONS];

    asmag_sections sections;

    asmag(){sections.init();}
    ~asmag(){if(assembled_binary != nullptr) delete[] assembled_binary;}

    bool parse_from_binary(binary_context *const ctx, string *text) override;
    bool compile_from_text(text_context *const ctx) override;
    void new_label(text_context *const ctx, string label_match, int& event_funct_count, bool *const found_event_label);
    void new_section(text_context *const ctx, string label_name, std::list<string>::iterator line);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);
    bool validate_super_special_section(text_context* const ctx, size_t& cur_offset);

    static constexpr char label_names[ASMAG_TOTAL_FUNCTIONS][25] = {
        "OnMag00", "OnMag01", "OnMag02", "OnMag03", "OnMag04", "OnMag05", "OnMag06", "OnMag07", "OnMag08", "OnMag09", "OnMag0A", "OnMag0B", "OnMag0C", "OnMag0D", "OnMag0E", "OnMag0F",
        "OnMag10", "OnMag11", "OnMag12", "OnMag13", "OnMag14", "OnMag15", "OnMag16", "OnMag17", "OnMag18", "OnMag19", "OnMag1A", "OnMag1B", "OnMag1C", "OnMag1D", "OnMag1E", "OnMag1F",
        "OnMag20", "OnMag21", "OnMag22", "OnMag23", "OnMag24", "OnMag25", "OnMag26", "OnMag27", "OnMag28", "OnMag29", "OnMag2A", "OnMag2B", "OnMag2C", "OnMag2D", "OnMag2E", "OnMag2F",
        "OnMag30", "OnMag31", "OnMag32", "OnMag33", "OnMag34", "OnMag35", "OnMag36", "OnMag37", "OnMag38", "OnMag39", "OnMag3A", "OnMag3B", "OnMag3C", "OnMag3D", "OnMag3E", "OnMag3F",
        "OnMag40", "OnMag41", "OnMag42", "OnMag43", "OnMag44", "OnMag45", "OnMag46", "OnMag47", "OnMag48", "OnMag49", "OnMag4A", "OnMag4B", "OnMag4C", "OnMag4D", "OnMag4E", "OnMag4F",
        "OnMag50", "OnMag51", "OnMag52", "OnMag53", "OnMag54", "OnMag55", "OnMag56", "OnMag57", "OnMag58", "OnMag59", "OnMag5A", "OnMag5B", "OnMag5C", "OnMag5D", "OnMag5E", "OnMag5F",
        "OnMag60", "OnMag61", "OnMag62", "OnMag63", "OnMag64", "OnMag65", "OnMag66", "OnMag67", "OnMag68", "OnMag69", "OnMag6A", "OnMag6B", "OnMag6C", "OnMag6D", "OnMag6E", "OnMag6F",
        "OnMag70", "OnMag71", "OnMag72", "OnMag73", "OnMag74", "OnMag75", "OnMag76", "OnMag77", "OnMag78", "OnMag79", "OnMag7A", "OnMag7B", "OnMag7C", "OnMag7D", "OnMag7E", "OnMag7F",
        "OnMag80", "OnMag81", "OnMag82", "OnMag83"};
};