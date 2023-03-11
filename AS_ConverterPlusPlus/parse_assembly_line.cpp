#include "parse_assembly_line.h"

inline bool is_legal(char c)
{
    return isalnum(c) || c == '_';
}

inline bool is_legal_start(char c)
{
    return isalnum(c) || c == '_' || c == '$' || c == '\"';
}

inline bool skip_whitespace(const char* const str, size_t& pos)
{
    if(str[pos] == '\0') return true;
    while (isblank(str[pos]))
    {
        pos++;
        if(str[pos] == '\0') return true;
    }
    return false;
}

inline bool read_legal(const char* const str, size_t& pos)
{
    if(str[pos] == '\0') return true;
    if(!is_legal_start(str[pos]))
    {
        printf("illegal start of instruction name or operand: %c\n", str[pos]);
        return false;
    }
    if(str[pos] == '\"')
    {
        pos++;
        while(str[pos] != '\"')
        {
            if(str[pos] == '\0')
            {
                printf("unterminated string operand\n");
                return false;
            }
            pos++;
        }
    }
    pos++;
    if(str[pos] == '\0') return true;
    while(is_legal(str[pos]))
    {
        pos++;
        if(str[pos] == '\0') return true;
    }
    return false;
}

inline bool seek_next_operand(const char* const str, size_t& pos, bool already_parsed_comma)
{
    CONTINUE:
    switch(str[pos])
    {
    case ',':
        if(skip_whitespace(str, ++pos))
        {
            printf("unexpected end of line\n");
            return false;
        }
        if(!is_legal_start(str[pos]))
        {
            printf("illegal start of operand: %c\n", str[pos]);
            return false;
        }
        return true;
    case ' ':
    case '\t':
        skip_whitespace(str, pos);
        if(already_parsed_comma)
        {
            if(!is_legal_start(str[pos]))
            {
                printf("illegal character %c\n", str[pos]);
                return false;
            }
            return true;
        }
        goto CONTINUE;
    case '\0':
        printf("unexpected end of line\n");
        return false;
    default:
        if(already_parsed_comma && is_legal_start(str[pos])) return true;
        printf("illegal character %c, or missing comma between operands\n", str[pos]);
        return false;
    }
}
bool parse_assembly_instruction(std::list<std::string>::iterator& string, char** name, size_t expected_args_count, char* args[20])
{
    std::string text = *string;
    return parse_assembly_instruction(text, name, expected_args_count, args);
}
bool parse_assembly_instruction(std::string& string, char** name, size_t expected_args_count, char* args[20])
{
    size_t pos = 0;
    size_t args_count = 0;
    char* const str = string.data();
    if(skip_whitespace(str, pos)) return false; //"  empty\t" string
    *name = (char*)(str + pos);
    if(read_legal(str, pos))
    {
        if(expected_args_count == 0) return true;
        else return false;
    }
    //check for invalid chars
    if(!isblank(str[pos]))
    {
        printf("illegal character %c\n", str[pos]);
        return false;
    }
    str[pos++] = '\0';
    if(skip_whitespace(str, pos))
    {
        if(expected_args_count == 0) return true;
        else return false;
    }
    if(!is_legal_start(str[pos]))
    {
        printf("illegal character %c\n", str[pos]);
        return false;
    }
    args[args_count++] = (char*)(str + pos); //operand 1
    if(!read_legal(str, pos))
    {
        if(!(isblank(str[pos]) || str[pos] == ','))
        {
            printf("illegal character %c\n", str[pos]);
            return false;
        }
        else str[pos++] = '\0';
    }
    bool parsed_comma = true;
    while(args_count != expected_args_count)
    {
        if(!seek_next_operand(str, pos, parsed_comma)) return false;
        args[args_count++] = (char*)(str + pos);
        read_legal(str, pos);
        if(str[pos] == ',') parsed_comma = true;
        else parsed_comma = false;
        str[pos++] = '\0';
    }
    if(pos < string.size() && !skip_whitespace(str, pos))
    {
        printf("unexpected characters after last operand\n");
        return false;
    }
    return true;
}