#include "directives.h"
#include "aniscript.h"

bool aniscript::ParseFromBinary(context *const ctx, string *text)
{
	craft_offset_table_offset = ctx->u16();
	craft_offset_table_offset_end = ctx->u16();
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
	text->append("%define self 0xFF\n%macro subchip_update 3\nselect_sub_chip $1, $2\nsleep $3\nupdate\n%endmacro\n");
	text->append("section chip_entries:\n");
	std::array<char, 100> buffer;
	for (byte i = 0; i < chip_entries.size(); i++)
	{
		chip_entry chp = chip_entries[i];
		snprintf(buffer.data(), buffer.size(), "chip%d:\nDW 0x%04X, 0x%04X\nDW 0x%04X, 0x%04X\n", i, chp.ch.index, chp.ch.datatable, chp.cp.index, chp.cp.datatable);
		text->append(&buffer[0]);
	}
	text->append("section model_3d:\n");
	ctx->cstring_array(&model_3d);
	for (byte i = 0; i < model_3d.size(); i++)
	{
		text->append("DB \"");
		text->append(model_3d[i]);
		text->append("\", 0x00\n");
	}
	text->append("DB 0x00\n"); // array terminator
	text->append("section bones_3d:\n");
	if (bones_3d_offset != 0)
	{
		bones_3d.fromBuffer(ctx);
		text->append("DB ");
		snprintf(buffer.data(), buffer.size(), "0x%04X\n", bones_3d.unk00);
		text->append(buffer.data());
		for (char i = 0; i < bones_3d.bones_3d.size(); i++)
		{
			text->append("DB \"");
			text->append(bones_3d.bones_3d[i]);
			text->append("\"\n");
		}
	}
	do
	{
		function_offset_table.push_back(ctx->u16());
	} while (ctx->position != craft_offset_table_offset_end);
	text->append("section unk_bytes:\n");
	for (byte i = 0; i < 8; i++)
	{
		unk_bytes[i].unk00 = ctx->u8();
		unk_bytes[i].unk01 = ctx->u8();
		snprintf(buffer.data(), buffer.size(), "DB 0x%02X, 0x%02X\n", unk_bytes[i].unk00, unk_bytes[i].unk01);
		text->append(buffer.data());
	}
	size_t beginOfInstructions = ctx->position;
	text->append("section .text:\n");
	do
	{
		instruction instr;
		try
		{
			instr = instruction::first_pass(ctx);
		}
		catch(...)
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
		for (char i = 0; i < function_offset_table.size(); i++)
		{
			if (function_offset_table[i] == instr.offset)
			{
				snprintf(&buffer[0], buffer.size(), "FUN%04X", i);
				labels[(uint16)instr.offset].push_back(buffer);
			}
		}
		
	} while (ctx->position < ctx->size());
	if(ctx->position > ctx->size()) return false;
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

		instruction func = instructions[toff];
		if (auto labelV = labels.find(toff); labelV == labels.end()) //labels don't exist. create a new label vector
		{
			snprintf(buffer.data(), buffer.size(), "loc%04X", toff);
			labels[toff].push_back(buffer);	
		}
		it->second.labelName = labels[toff][0].data();
		++it;
	}
	ctx->position = beginOfInstructions;
	it = instructions.begin();
	while(it != instructions.end())
	{
		uint16 instrOffset = it->first;
		if (auto labelV = labels.find(instrOffset); labelV != labels.end()) //labels exist. place them in the text file before the instruction.
		{
			for (int j = 0; j < labelV->second.size(); j++)
			{
				text->append(labelV->second[j].data());
				text->append(":\n");
			}
		}
		it->second.second_pass(ctx, text);
		++it;
	}
	static std::regex longform("\tselect_sub_chip ([a-zA-Z0-9_]+), ([a-zA-Z0-9_]+)\n\tsleep ([a-zA-Z0-9_]+)\n\tupdate");
	text->assign((string)std::regex_replace(*text, longform, "\tsubchip_update $1, $2, $3"));
	return true;
}