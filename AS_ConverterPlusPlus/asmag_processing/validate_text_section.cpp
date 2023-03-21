#include "../asmag.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool asmag::validate_text_line(std::list<string>::iterator &line, size_t &cur_offset, text_context *ctx)
{
	int index = 0;
	while(isblank(line->data()[index])) { index++; };
	char *command_name = (char*)(line->data() + index);
	int len = 1;
	while(command_name[len] != '\0' && isalnum(command_name[len]) || command_name[len] == '_') { len++; }
	size_t cur_offset_copy = cur_offset;
	instructions[cur_offset_copy] = instruction::first_pass_text(line, command_name, len, cur_offset);

    bool has_errors = false;
	if(cur_offset > 0xFFFF)
	{
		printf("[ERROR] %ls\n\tresulting binary is too big! Error happened on instruction: %.*s\n", ctx->filename.c_str(), command_name);
		has_errors = true;
	}
	has_errors |= instructions[cur_offset_copy].opcode == 0xFF;
	return has_errors;
}

bool asmag::validate_text_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
	auto text_section_copy = text_section;
	text_section_copy++;
	boost::smatch what;
	static const boost::regex label_pattern("[ \\t]*([\\w_]+)[ \\t]*:[ \\t]*");
	while (text_section_copy != super_special_section_section && text_section_copy != ctx->lines.end())
	{
		if (text_section_copy->find_first_not_of(" \t") == string::npos)
		{
			text_section_copy++;
			continue;
		}
		if(boost::regex_match(*text_section_copy, what, label_pattern))
		{
			size_t index = 0;
			while(isblank(text_section_copy->at(index))){ index++; }
			string label = what[1];
			label_to_offset_map[label] = cur_offset;
			//is it an event label?
			for(int i = 0; i < ASMAG_TOTAL_FUNCTIONS; i++)
			{
				if(strcmp(label.data(), label_names[i]) == 0)
				{
					magic_label_offsets[i] = cur_offset;
					break;
				}
			}
			text_section_copy++;
			continue;
		}
		has_errors |= validate_text_line(text_section_copy, cur_offset, ctx);
		text_section_copy++;
	}
    return has_errors;
}

bool asmag::validate_super_special_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
	auto super_special_section_section_copy = super_special_section_section;
	super_special_section_section_copy++;
	boost::smatch what;
	static const boost::regex label_pattern("[ \\t]*([\\w_]+)[ \\t]*:[ \\t]*");
	while (super_special_section_section_copy != text_section && super_special_section_section_copy != ctx->lines.end())
	{
		if (super_special_section_section_copy->find_first_not_of(" \t") == string::npos)
		{
			super_special_section_section_copy++;
			continue;
		}
		if(boost::regex_match(*super_special_section_section_copy, what, label_pattern))
		{
			size_t index = 0;
			while(isblank(super_special_section_section_copy->at(index))){ index++; }
			string label = what[1];
			label_to_offset_map[label] = cur_offset;
			//is it a magic_function label?
			for(int i = 0; i < ASMAG_TOTAL_FUNCTIONS; i++)
			{
				if(strcmp(label.data(), label_names[i]) == 0)
				{
					magic_label_offsets[i] = cur_offset;
					break;
				}
			}
			super_special_section_section_copy++;
			continue;
		}
		has_errors |= validate_text_line(super_special_section_section_copy, cur_offset, ctx);
		super_special_section_section_copy++;
	}
    return has_errors;
}