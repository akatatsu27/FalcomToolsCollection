#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <unordered_set>
#include "../asbs.h"
#include "../parse_assembly_line.h"

bool asbs::compile_from_text(text_context *const ctx)
{
	printf("[INFO] %ls:\n\tassembling binary...\n", ctx->filename.c_str());
	bool has_errors = false;
	int item_funct_count = 0;
	bool found_bs_labels[ASBS_TOTAL_FUNCTIONS] = {false};
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
			new_label(ctx, label_match, item_funct_count, found_bs_labels);
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
			printf("[ERROR] %ls:\n\tmalformed label or section definition:\n\t%s\n", ctx->filename.c_str(), line->c_str());
			has_errors = true;
		}
	}

	if (!sections.text)
	{
		printf("[ERROR] %ls:\n\tmissing \"text\" section\n", ctx->filename.c_str());
		has_errors = true;
	}

	for(int i = 0; i < ASBS_TOTAL_FUNCTIONS; i++)
	{
		if (found_bs_labels[i]) continue;
		printf("[ERROR] %ls:\n\tmissing bs_function label(s):\n", ctx->filename.c_str());
		for (int j = 0; j < ASBS_TOTAL_FUNCTIONS; j++)
		{
			if (!found_bs_labels[j])
			{
				printf("\t%s\n", label_names[j]);
			}
		}
		has_errors = true;
		break;
	}

	if (has_errors)
		return false;

	// calculate offsets
	cur_offset = 2; // skip function_offset_table_offset
	function_offset_table_offset = cur_offset;
	cur_offset += 2 * item_funct_count;
	size_t text_section_start = cur_offset;
	has_errors |= validate_text_section(ctx, cur_offset);
	if(has_errors) return false;
	if(cur_offset > 0x4E20) //hardcoded limit. otherwise buffer overflow. you don't want that, right?
	{
		printf("[ERROR] %ls:\n\tresulting binary is too big! (0x%04X bytes) max is 0x4E20\n", ctx->filename.c_str(), cur_offset);
		return false;
	}

	//write
	assembled_binary_size = cur_offset;
	cur_offset = 0;
	assembled_binary = new char[assembled_binary_size];
	u16(function_offset_table_offset);
	for(int i = 0; i < ASBS_TOTAL_FUNCTIONS; i++)
	{
		u16(function_label_offsets[i]);
	}
	for(auto it : instructions)
	{
		has_errors |= it.second.second_pass_text(this);
	}
	return !has_errors;
}