#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool validate_chip_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx)
{
	string copy = *line;
	size_t total_bytes = 0;

	char *command_name;
	char *args[20];
	size_t cnt = boost::count(copy, ',') + 1;
	if (!parse_assembly_instruction(copy, &command_name, cnt, args))
		return true;
	for(int i = 0; i < cnt; i++)
	{
		if(args[i][0] == '\"')
		{
			printf("[ERROR] %ls:\n\tstring operands aren't allowed in chip_entries\n", ctx->filename.c_str());
			return true;
		}
	}
	switch (instruction::hash(command_name))
	{
	case instruction::hash("DB"):
	case instruction::hash("db"):
		total_bytes += 1 * cnt;
		break;
	case instruction::hash("DW"):
	case instruction::hash("dw"):
		total_bytes += 2 * cnt;
		break;
	default:
		printf("[ERROR] %ls:\n\tUnrecognized assembly instruction in chip_entry: %s\n", ctx->filename.c_str(), command_name);
		return true;
	}
	pos += total_bytes;
	return false;
}
bool aniscript::validate_chips_section(text_context* const ctx, size_t& cur_offset)
{
	auto chip_section_copy = chip_section;
	bool has_errors = false;
	chip_section_copy++;
	size_t cur_offset_copy = cur_offset;
	while (chip_section_copy != model_3d_section && chip_section_copy != bones_3d_section && chip_section_copy != unk_bytes_section && chip_section_copy != text_section && chip_section_copy != ctx->lines.end())
	{
		if (chip_section_copy->find_first_not_of(" \t") == string::npos)
		{
			chip_section_copy++;
			continue;
		}
		if (chip_section_copy->find(':') != string::npos)
		{
			chip_section_copy++;
			continue;
		}
		has_errors = validate_chip_line(chip_section_copy, cur_offset, ctx);
		chip_section_copy++;
	}
	if((cur_offset - cur_offset_copy) % 8 != 0)
	{
		printf("[ERROR] %ls\n\tsection \"chip_entries\" is malformed. Each chip entry (ch + cp) must be total 8 bytes in length\n", ctx->filename.c_str());
		has_errors = true;
	}
	cur_offset += 4; //terminator "entry"
	return has_errors;
}