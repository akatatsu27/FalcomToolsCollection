#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <unordered_set>
#include "aniscript.h"
#include "parse_assembly_line.h"

bool aniscript::CompileFromText(text_context *const ctx, vector<char> *const binary)
{
	bool has_errors = false;
	char event_funct_count = 0;
	bool found_event_label[33] = {false};
	static const boost::regex label_pattern("[ \\t]*([_]*[\\w_]+[_]*)+[ \\t]*:[ \\t]*");
	static const boost::regex section_pattern("[ \\t]*section[ \\t]+([_]*[\\w]+[_]*)+[ \\t]*:[ \\t]*");
	boost::smatch what;
	string label_match;
	// find total number of event labels, and check for any redefinitions
	// also find the section definitions
	size_t line_num = 0;
	for (std::list<string>::iterator line = ctx->lines.begin(); line != ctx->lines.end(); line_num++, line++)
	{
		size_t label_end_pos = line->find(':');
		if (label_end_pos == string::npos)
			continue;
		if (boost::regex_match(*line, what, label_pattern))
		{
			label_match = what[1];
			new_label(ctx, label_match, event_funct_count, found_event_label);
		}
		else if (boost::regex_match(*line, what, section_pattern))
		{
			label_match = what[1];
			new_section(ctx, label_match, line);
		}
		else
		{
			printf("[ERROR] %ls:%d\n\tmalformed label or section definition\n", ctx->filename.c_str(), line_num);
			has_errors = true;
		}
	}

	if (!sections.chips)
	{
		printf("[ERROR] %ls:\n\tmissing \"chip_entries\" section\n", ctx->filename.c_str());
		has_errors = true;
	}
	if (!sections.model)
	{
		printf("[ERROR] %ls:\n\tmissing \"model_3d\" section\n", ctx->filename.c_str());
		has_errors = true;
	}
	if (!sections.bones)
	{
		printf("[ERROR] %ls:\n\tmissing \"bones_3d\" section\n", ctx->filename.c_str());
		has_errors = true;
	}
	if (!sections.unk_bytes)
	{
		printf("[ERROR] %ls:\n\tmissing \"unk_bytes\" section\n", ctx->filename.c_str());
		has_errors = true;
	}
	if (!sections.text)
	{
		printf("[ERROR] %ls:\n\tmissing \"text\" section\n", ctx->filename.c_str());
		has_errors = true;
	}

	if (event_funct_count < 30)
	{
		printf("[ERROR] %ls:\n\tmissing mandatory event_function labels:\n", ctx->filename.c_str());
		for (char i = 0; i < label_names_count; i++)
		{
			if (!found_event_label[i])
			{
				printf("\t%s\n", label_names[i]);
			}
		}
		has_errors = true;
	}
	if (has_errors)
	{
		return false;
	}
	// calculate offsets
	size_t cur_offset = 4; // skip craft_offset_table_offset and craft_offset_table_offset_end
	cur_offset += 2; // bones_3d_offset
	
	

	has_errors |= validate_chips_section(ctx, cur_offset);
	has_errors |= validate_model_section(ctx, cur_offset);
	has_errors |= validate_bones_section(ctx, cur_offset);
	cur_offset += 2 * event_funct_count;
	has_errors |= validate_unk_bytes_section(ctx, cur_offset);
	has_errors |= validate_text_section(ctx, cur_offset);
	return true;
}