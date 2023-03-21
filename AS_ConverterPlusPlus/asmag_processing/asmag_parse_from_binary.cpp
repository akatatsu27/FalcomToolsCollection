#include "../aniscript.h"
#include "../as_instruction.h"
#include "../asmag.h"

bool asmag::parse_from_binary(binary_context *const ctx, string *text)
{
    function_offset_table_offset = ctx->u16();
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
    }while(ctx->position != function_offset_table_offset);
    for (int i = 0; i < ASMAG_TOTAL_FUNCTIONS; i++) // total number of functions not specified anywhere
    {
        function_offset_table.push_back(ctx->u16());
    }
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
    text->append("section super_special_section:\n");   
    ctx->position = beginOfInstructions;
	it = instructions.begin();
	while (it != instructions.end())
	{
		uint16 instrOffset = it->first;
        if(instrOffset == function_offset_table[0])
        {
            ctx->position = instrOffset;
            text->append("section text:\n");
        }
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
    return true;
}
