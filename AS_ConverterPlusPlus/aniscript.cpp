#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <unordered_set>
#include "aniscript.h"
#include "parse_assembly_line.h"

bool aniscript::CompileFromText(text_context *const ctx, vector<char> *const binary)
{
	bool has_errors = false;
	char event_funct_count = 0;
	bool found_event_label[TOTAL_EVENT_LABELS] = {false};
	static const boost::regex label_pattern("[ \\t]*([\\w_]+)[ \\t]*:[ \\t]*");
	static const boost::regex section_pattern("[ \\t]*section[ \\t]+([\\w]+)[ \\t]*:[ \\t]*");
	static const boost::regex sigh_just_a_string("\"(.)*:");
	boost::smatch what;
	string label_match;
	// find total number of event labels, and check for any redefinitions
	// also find the section definitions
	for (std::list<string>::iterator line = ctx->lines.begin(); line != ctx->lines.end(); line++)
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
		else if(boost::regex_search(*line, what, sigh_just_a_string))
		{
			continue;
		}
		else
		{
			printf("[ERROR] %ls:\n\tmalformed label or section definition:\n\t%s\n", ctx->filename.c_str(), line->data());
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

	for(int i =0; i < MANDATORY_EVENT_LABELS; i++)
	{
		if (found_event_label[i]) continue;
		printf("[ERROR] %ls:\n\tmissing mandatory event_function labels:\n", ctx->filename.c_str());
		for (char j = 0; j < MANDATORY_EVENT_LABELS; j++)
		{
			if (!found_event_label[j])
			{
				printf("\t%s\n", label_names[j]);
			}
		}
		has_errors = true;
	}
	//check for missing "extra" events in-between "extra" events
	for(int i = TOTAL_EVENT_LABELS -1; i > MANDATORY_EVENT_LABELS; i--)
	{
		if(!found_event_label[i]) continue;
		for(int j = i; j > MANDATORY_EVENT_LABELS; j--)
		{
			if(!found_event_label[j])
			{
				printf("[ERROR] %ls:\n\tmissing event_function label:\n\t%s\n", ctx->filename.c_str(), label_names[j]);
				has_errors = true;
			}
		}
		break;
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