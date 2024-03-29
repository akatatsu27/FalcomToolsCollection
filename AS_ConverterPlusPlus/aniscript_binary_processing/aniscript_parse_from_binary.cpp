#include "../aniscript.h"

bool aniscript::parse_from_binary(binary_context *const ctx, string *text)
{
	event_offset_table_offset = ctx->u16();
	event_offset_table_offset_end = ctx->u16();
	bones_3d_offset = ctx->u16();
	chip_entry entry;
	do
	{
		entry.ch.index = ctx->u16();
		entry.ch.datatable = ctx->u16();
		if (entry.ch.index == 0xFFFF)
			break;
		entry.cp.index = ctx->u16();
		entry.cp.datatable = ctx->u16();
		chip_entries.push_back(entry);
	} while (1);
	ctx->cstring_array(&model_3d);
	if (bones_3d_offset != 0)
	{
		bones_3d.fromBuffer(ctx);
	}
	do
	{
		function_offset_table.push_back(ctx->u16());
	} while (ctx->position != event_offset_table_offset_end);
	for (uint8 i = 0; i < 8; i++)
	{
		sprite_offsets[i].horizontal_offset = ctx->u8();
		sprite_offsets[i].vertical_offset = ctx->u8();
	}
	size_t beginOfInstructions = ctx->position;	
	do
	{
		instruction instr;
		try
		{
			instr = instruction::first_pass_binary(ctx, this);
		}
		catch (...)
		{
			return false;
		}

		if (instr.opcode == 0xFF)
			return false;
		instructions[(uint16)instr.offset] = instr;
		// combat events trigger specific functions identified by the index
		// in the function_offset_table
		// search for the current offset in the function_offset table,
		// and if it exists place a label
		for (int i = 0; i < function_offset_table.size(); i++)
		{
			if (function_offset_table[i] == instr.offset)
			{
				std::array<char, 100> arr;
				memcpy(arr.data(), label_names[i], sizeof(label_names[i]));
				labels[(uint16)instr.offset].push_back(arr);
			}
		}

	} while (ctx->position < ctx->size());
	if (ctx->position > ctx->size())
		return false;
	std::array<char, 100> buffer;
	// Get an iterator pointing to the first element in the map
	std::map<uint16, instruction>::iterator it = instructions.begin();
	// Iterate through the map
	while (it != instructions.end())
	{
		uint16 toff = it->second.targetOffset;
		if (toff == 0)
		{
			++it;
			continue;
		}
		else if (toff == 0x0C) //0x0C is an invalid offset that is present in "AS04230 ._DT" and "AS04240 ._DT"
		{
			auto beninging = instructions.begin();
			it->second.labelName = labels[beninging->first][0].data(); //simply point them to the first instruction, which is end_event.
			++it;
			continue;
		}

		if (auto labelV = labels.find(toff); labelV == labels.end()) // labels don't exist. create a new label vector
		{
			snprintf(buffer.data(), buffer.size(), "loc%04X", toff);
			labels[toff].push_back(buffer);
		}
		it->second.labelName = labels[toff][0].data();
		++it;
	}
	//Writing
	for(uint8 target : targets_in_file)
	{
		const char* const target_name = instruction::get_target_name(target);
		snprintf(buffer.data(), buffer.size(), "%%define %s 0x%02X\n", target_name, target);
		text->append(&buffer[0]);
	}
	text->append("%macro subchip_update 3\nselect_sub_chip $1, $2\nsleep $3\nupdate\n%endmacro\n");
	text->append("section chip_entries:\n");
	for (int i = 0; i < chip_entries.size(); i++)
	{
		chip_entry chp = chip_entries[i];
		snprintf(buffer.data(), buffer.size(), "chip%d:\nDW 0x%04X, 0x%04X\nDW 0x%04X, 0x%04X\n", i, chp.ch.index, chp.ch.datatable, chp.cp.index, chp.cp.datatable);
		text->append(&buffer[0]);
	}
	text->append("section model_3d:\n");
	for (int i = 0; i < model_3d.size(); i++)
	{
		text->append("DB \"");
		text->append(model_3d[i]);
		text->append("\"\n");
	}
	text->append("section bones_3d:\n");
	if (bones_3d_offset != 0)
	{
		text->append("DB ");
		snprintf(buffer.data(), buffer.size(), "0x%02X\n", bones_3d.unk00);
		text->append(buffer.data());
		for (char i = 0; i < bones_3d.bones_3d.size(); i++)
		{
			text->append("DB \"");
			text->append(bones_3d.bones_3d[i]);
			text->append("\"\n");
		}
	}
	text->append("section sprite_offsets:\n");
	for (uint8 i = 0; i < 8; i++)
	{
		snprintf(buffer.data(), buffer.size(), "DB 0x%02X, 0x%02X\n", sprite_offsets[i].horizontal_offset, sprite_offsets[i].vertical_offset);
		text->append(buffer.data());
	}
	text->append("section text:\n");
	ctx->position = beginOfInstructions;
	it = instructions.begin();
	while (it != instructions.end())
	{
		uint16 instrOffset = it->first;
		if (auto labelV = labels.find(instrOffset); labelV != labels.end()) // labels exist. place them in the text file before the instruction.
		{
			for (int j = 0; j < labelV->second.size(); j++)
			{
				text->append(labelV->second[j].data());
				text->append(":\n");
			}
		}
		it->second.second_pass_binary(ctx, text);
		++it;
	}
	static boost::regex longform("\tselect_sub_chip ([a-zA-Z0-9_]+), ([a-zA-Z0-9_]+)\n\tsleep ([a-zA-Z0-9_]+)\n\tupdate");
	text->assign((string)boost::regex_replace(*text, longform, "\tsubchip_update $1, $2, $3"));
	return true;
}