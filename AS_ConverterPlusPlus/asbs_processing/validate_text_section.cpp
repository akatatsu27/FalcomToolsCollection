#include "../asbs.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool asbs::validate_text_section(text_context* const ctx, size_t& cur_offset)
{
    bool has_errors = false;
	auto text_section_copy = text_section;
	text_section_copy++;
	boost::smatch what;
	static const boost::regex label_pattern("[ \\t]*([\\w_]+)[ \\t]*:[ \\t]*");
	while (text_section_copy != ctx->lines.end())
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
			for(int i = 0; i < ASBS_TOTAL_FUNCTIONS; i++)
			{
				if(strcmp(label.data(), label_names[i]) == 0)
				{
					function_label_offsets[i] = cur_offset;
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