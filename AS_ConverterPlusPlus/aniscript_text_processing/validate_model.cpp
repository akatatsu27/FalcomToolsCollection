#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool validate_model_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx)
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
		printf("[ERROR] %ls:\n\tinvalid assembly instruction in section \"model_3d\": %s\n", ctx->filename.c_str(), command_name);
		has_errors = true;
	}
	for(int i = 0; i < cnt; i++)
	{
		if(args[i][0] != '\"')
		{
			printf("[ERROR] %ls:\n\tnon-string operands aren't allowed in section \"model_3d\": %s\n", ctx->filename.c_str(), args[i]);
			has_errors = true;
		}
        pos += strlen(args[i]) + 1; //"string" + \0
	}

    return has_errors;
}

bool aniscript::validate_model_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
	auto model_3d_section_copy = model_3d_section;
	model_3d_section_copy++;
	while (model_3d_section_copy != chip_section && model_3d_section_copy != bones_3d_section && model_3d_section_copy != unk_bytes_section && model_3d_section_copy != text_section && model_3d_section_copy != ctx->lines.end())
	{
		if (model_3d_section_copy->find_first_not_of(" \t") == string::npos)
		{
			model_3d_section_copy++;
			continue;
		}
		has_errors |= validate_model_line(model_3d_section_copy, cur_offset, ctx);
		model_3d_section_copy++;
	}
    cur_offset += 1; //terminator null byte
    return has_errors;
}