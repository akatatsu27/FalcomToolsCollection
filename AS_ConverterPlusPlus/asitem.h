#include "base_aniscript.h"

#define ASITEM_TOTAL_FUNCTIONS 0x78

union asitem_sections
{
    bool super_special_section : 1;
    bool text : 1;

    void init()
    {
        super_special_section = false;
        text = false;
    }
};

struct asitem : public base_aniscript
{
    asitem(){sections.init();}
    ~asitem(){if(assembled_binary != nullptr) delete[] assembled_binary;}

    uint16 function_offset_table_offset;
    vector<uint16> function_offset_table;
    std::list<string>::iterator super_special_section_section;
    std::list<string>::iterator text_section;
    uint16 function_label_offsets[ASITEM_TOTAL_FUNCTIONS];

    asitem_sections sections;


    bool parse_from_binary(binary_context *const ctx, string *text) override;
    bool compile_from_text(text_context *const ctx) override;
    void new_label(text_context *const ctx, string label_match, int& event_funct_count, bool *const found_event_label);
    void new_section(text_context *const ctx, string label_name, std::list<string>::iterator line);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);
    bool validate_super_special_section(text_context* const ctx, size_t& cur_offset);

    static constexpr char label_names[ASITEM_TOTAL_FUNCTIONS][25] = {
        "OnItem00", "OnItem01", "OnItem02", "OnItem03", "OnItem04", "OnItem05", "OnItem06", "OnItem07", "OnItem08", "OnItem09", "OnItem0A", "OnItem0B", "OnItem0C", "OnItem0D", "OnItem0E", "OnItem0F",
        "OnItem10", "OnItem11", "OnItem12", "OnItem13", "OnItem14", "OnItem15", "OnItem16", "OnItem17", "OnItem18", "OnItem19", "OnItem1A", "OnItem1B", "OnItem1C", "OnItem1D", "OnItem1E", "OnItem1F",
        "OnItem20", "OnItem21", "OnItem22", "OnItem23", "OnItem24", "OnItem25", "OnItem26", "OnItem27", "OnItem28", "OnItem29", "OnItem2A", "OnItem2B", "OnItem2C", "OnItem2D", "OnItem2E", "OnItem2F",
        "OnItem30", "OnItem31", "OnItem32", "OnItem33", "OnItem34", "OnItem35", "OnItem36", "OnItem37", "OnItem38", "OnItem39", "OnItem3A", "OnItem3B", "OnItem3C", "OnItem3D", "OnItem3E", "OnItem3F",
        "OnItem40", "OnItem41", "OnItem42", "OnItem43", "OnItem44", "OnItem45", "OnItem46", "OnItem47", "OnItem48", "OnItem49", "OnItem4A", "OnItem4B", "OnItem4C", "OnItem4D", "OnItem4E", "OnItem4F",
        "OnItem50", "OnItem51", "OnItem52", "OnItem53", "OnItem54", "OnItem55", "OnItem56", "OnItem57", "OnItem58", "OnItem59", "OnItem5A", "OnItem5B", "OnItem5C", "OnItem5D", "OnItem5E", "OnItem5F",
        "OnItem60", "OnItem61", "OnItem62", "OnItem63", "OnItem64", "OnItem65", "OnItem66", "OnItem67", "OnItem68", "OnItem69", "OnItem6A", "OnItem6B", "OnItem6C", "OnItem6D", "OnItem6E", "OnItem6F",
        "OnItem70", "OnItem71", "OnItem72", "OnItem73", "OnItem74", "OnItem75", "OnItem76", "OnItem77"};
};