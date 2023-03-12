#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool aniscript::validate_text_line(std::list<string>::iterator &line, size_t &cur_offset, text_context *ctx)
{
	int index = 0;
	while(isblank(line->data()[index])) { index++; };
	char *command_name = (char*)(line->data() + index);
	int len = 1;
	while(command_name[len] != '\0' && isalnum(command_name[len]) || command_name[len] == '_') { len++; }
	size_t cur_offset_copy = cur_offset;
	instructions[cur_offset_copy] = instruction::first_pass_text(line, command_name, len, cur_offset);

    return instructions[cur_offset_copy].opcode == 0xFF;
}

bool aniscript::validate_text_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
	auto text_section_copy = text_section;
	text_section_copy++;
	while (text_section_copy != chip_section && text_section_copy != bones_3d_section && text_section_copy != unk_bytes_section && text_section_copy != model_3d_section && text_section_copy != ctx->lines.end())
	{
		if (text_section_copy->find_first_not_of(" \t") == string::npos)
		{
			text_section_copy++;
			continue;
		}
		size_t label_end = text_section_copy->find(':');
		if(label_end != string::npos)
		{
			size_t index = 0;
			while(isblank(text_section_copy->at(index))){ index++; }
			string label = text_section_copy->substr(index, label_end - index);
			label_to_offset_map[label] = cur_offset;
			text_section_copy++;
			continue;
		}
		has_errors |= validate_text_line(text_section_copy, cur_offset, ctx);
		text_section_copy++;
	}
    return has_errors;
}