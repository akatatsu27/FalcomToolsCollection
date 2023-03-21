#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "as_instruction.h"

template<size_t N>
using FCArray = std::vector<std::array<char,N>>; // fixed size char array vector

struct base_aniscript
{
    std::unordered_set<uint8> targets_in_file; // all files make use of 0xFF (self), but there's a lot more possible targets. we want to %define only when needed.
    std::unordered_map<uint16, FCArray<100>> labels;
    std::unordered_map<string, uint16> label_to_offset_map;

    std::map<uint16, instruction> instructions;
    char* assembled_binary = nullptr;
    size_t assembled_binary_size;
    size_t cur_offset;
    
    virtual bool parse_from_binary(binary_context* const ctx, string* const text) = 0;
    virtual bool compile_from_text(text_context* const ctx) = 0;

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