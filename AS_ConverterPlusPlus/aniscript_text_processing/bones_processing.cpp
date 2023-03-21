#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool validate_bones_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx)
{
	string copy = *line;
    bool has_errors = false;
    char *command_name;
	char *args[20];
	size_t cnt = boost::count(copy, ',') + 1;
	if (!parse_assembly_instruction(copy, &command_name, cnt, args))
		return true;

    switch (instruction::hash(command_name))
	{
	case instruction::hash("DB"):
	case instruction::hash("db"):
		break;
	default:
		printf("[ERROR] %ls:\n\tinvalid assembly instruction in section \"bones_3d\": %s\n", ctx->filename.c_str(), command_name);
		return true;
	}
	for(int i = 0; i < cnt; i++)
	{
		if(args[i][0] != '\"')
		{
			printf("[ERROR] %ls:\n\tnon-string operands aren't allowed in section \"bones_3d\", except once at the beginning: %s\n", ctx->filename.c_str(), args[i]);
			has_errors = true;
		}
        pos += strlen(args[i]) -2 + 1; //"string" + \0
	}

    return has_errors;
}
bool validate_unk00(std::list<string>::iterator &line, size_t &pos, text_context *ctx)
{
	string copy = *line;
    bool has_errors = false;
    char *command_name;
	char *args[20];
	size_t cnt = 1;
	if (!parse_assembly_instruction(copy, &command_name, cnt, args))
		return true;

    switch (instruction::hash(command_name))
	{
	case instruction::hash("DB"):
	case instruction::hash("db"):
		break;
	default:
		printf("[ERROR] %ls:\n\tinvalid assembly instruction in section \"bones_3d\": %s\n", ctx->filename.c_str(), command_name);
		return true;
	}
	pos += 1;

    return has_errors;
}

bool aniscript::validate_bones_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
    bool parsed_unk00 = false;
	auto bones_3d_section_copy = bones_3d_section;
	bones_3d_section_copy++;
	while (bones_3d_section_copy != chip_section && bones_3d_section_copy != model_3d_section
		&& bones_3d_section_copy != sprite_offsets_section && bones_3d_section_copy != text_section && bones_3d_section_copy != ctx->lines.end())
	{
		if (bones_3d_section_copy->find_first_not_of(" \t") == string::npos)
		{
			bones_3d_section_copy++;
			continue;
		}
        if(parsed_unk00)
		    has_errors |= validate_bones_line(bones_3d_section_copy, cur_offset, ctx);
        else
        {
            has_errors |= validate_unk00(bones_3d_section_copy, cur_offset, ctx);
            parsed_unk00 = true;
        }
		bones_3d_section_copy++;
	}
    if(parsed_unk00) cur_offset += 1; //terminator null byte. if bones_3d section is empty, there is no null byte.
    return has_errors;
}

bool aniscript::write_binary_bones_line(std::list<string>::iterator &line)
{
	string copy = *line;
    bool has_errors = false;
    char *command_name;
	char *args[20];
	size_t cnt = boost::count(copy, ',') + 1;
	if (!parse_assembly_instruction(copy, &command_name, cnt, args))
		return true;
	for(int i = 0; i < cnt; i++)
	{
		size_t index = 1; //skip '\"'
		do
		{
			u8(args[i][index]);
			index++;
		} while (args[i][index] != '\"');
		u8(0); //terminate the string
	}

    return has_errors;
}
bool aniscript::write_binary_bones_unk00(std::list<string>::iterator &line)
{
	string copy = *line;
    char *command_name;
	char *args[20];
	size_t cnt = 1;
	if (!parse_assembly_instruction(copy, &command_name, cnt, args))
		return true;
	auto num = num_from_str(args[0], 1);
	if(num == ULLONG_MAX)
	{
		return true;
	}
	u8((uint8)num);

    return false;
}

bool aniscript::write_binary_bones_section(text_context* const ctx)
{
    bool has_errors = false;
    bool parsed_unk00 = false;
	auto bones_3d_section_copy = bones_3d_section;
	bones_3d_section_copy++;
	while (bones_3d_section_copy != chip_section && bones_3d_section_copy != model_3d_section
		&& bones_3d_section_copy != sprite_offsets_section && bones_3d_section_copy != text_section && bones_3d_section_copy != ctx->lines.end())
	{
		if (bones_3d_section_copy->find_first_not_of(" \t") == string::npos)
		{
			bones_3d_section_copy++;
			continue;
		}
        if(parsed_unk00)
		    has_errors |= write_binary_bones_line(bones_3d_section_copy);
        else
        {
            has_errors |= write_binary_bones_unk00(bones_3d_section_copy);
            parsed_unk00 = true;
        }
		bones_3d_section_copy++;
	}
    if(parsed_unk00) u8(0); //terminator null byte. if bones_3d section is empty, there is no null byte.
    return has_errors;
}