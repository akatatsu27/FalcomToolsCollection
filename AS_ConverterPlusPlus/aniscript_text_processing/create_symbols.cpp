#include "../aniscript.h"

void aniscript::new_label(text_context *const ctx, string label_match, char &event_funct_count, bool *const found_event_label)
{
	for (char i = 0; i < TOTAL_EVENT_LABELS; i++)
	{
		if (strcmp(label_match.data(), label_names[i]) != 0)
			continue;
		if (found_event_label[i])
		{
			printf("[ERROR] %ls:\n\tredefinition of event label %s\n", ctx->filename.c_str(), label_names[i]);
			throw(69);
		}
		found_event_label[i] = true;
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

void aniscript::new_section(text_context *const ctx, string label_name, std::list<string>::iterator line)
{
	switch (instruction::hash(label_name.data()))
	{
	case instruction::hash("chip_entries"):
		chip_section = line;
		sections.chips = true;
		break;
	case instruction::hash("model_3d"):
		model_3d_section = line;
		sections.model = true;
		break;
	case instruction::hash("bones_3d"):
		bones_3d_section = line;
		sections.bones = true;
		break;
	case instruction::hash("sprite_offsets"):
		sprite_offsets_section = line;
		sections.sprite_offsets = true;
		break;
	case instruction::hash("text"):
		text_section = line;
		sections.text = true;
		break;
	default:
		printf("[ERROR] %ls:\n\tunrecognized section name: %s\n", ctx->filename.c_str(), line->data());
		throw(69);
	}
}