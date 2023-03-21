#include "../asbs.h"

void asbs::new_label(text_context *const ctx, string label_match, int& event_funct_count, bool* const found_item_labels)
{
	for (int i = 0; i < ASBS_TOTAL_FUNCTIONS; i++)
	{
		if (strcmp(label_match.data(), label_names[i]) != 0)
			continue;
		if (found_item_labels[i])
		{
			printf("[ERROR] %ls:\n\tredefinition of bs label %s\n", ctx->filename.c_str(), label_names[i]);
			throw(69);
		}
		found_item_labels[i] = true;
		event_funct_count++;
		return;
	}
	auto insertion_result = label_to_offset_map.insert({label_match, 0});
	if (!insertion_result.second)
	{
		printf("[ERROR] %ls:\n\tredefinition of label %s\n", ctx->filename.c_str(), label_match.data());
		throw(69);
	}
}

void asbs::new_section(text_context *const ctx, string label_name, std::list<string>::iterator line)
{
	switch (instruction::hash(label_name.data()))
	{
	case instruction::hash("text"):
		text_section = line;
		sections.text = true;
		break;
	default:
		printf("[ERROR] %ls:\n\tunrecognized section name: %s\n", ctx->filename.c_str(), line->data());
		throw(69);
	}
}