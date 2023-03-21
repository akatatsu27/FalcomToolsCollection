#include "base_aniscript.h"

#define ASBS_TOTAL_FUNCTIONS 0x03

union asbs_sections
{
    bool text : 1;

    void init()
    {
        text = false;
    }
};

struct asbs : public base_aniscript
{
    asbs(){sections.init();}
    ~asbs(){if(assembled_binary != nullptr) delete[] assembled_binary;}

    uint16 function_offset_table_offset;
    vector<uint16> function_offset_table;
    std::list<string>::iterator text_section;
    uint16 function_label_offsets[ASBS_TOTAL_FUNCTIONS];

    asbs_sections sections;

    bool parse_from_binary(binary_context *const ctx, string *text) override;
    bool compile_from_text(text_context *const ctx) override;
    void new_label(text_context *const ctx, string label_match, int& event_funct_count, bool *const found_event_label);
    void new_section(text_context *const ctx, string label_name, std::list<string>::iterator line);
    bool validate_text_section(text_context* const ctx, size_t& cur_offset);

    static constexpr char label_names[ASBS_TOTAL_FUNCTIONS][25] = {
        "OnBS00", "OnBS01", "OnBS02"};
};