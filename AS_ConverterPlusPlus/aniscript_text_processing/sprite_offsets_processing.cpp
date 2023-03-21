#include "../aniscript.h"
#include "../parse_assembly_line.h"
#include <boost/range/algorithm/count.hpp>

bool validate_sprite_offsets_line(std::list<string>::iterator &line, size_t &pos, text_context *ctx)
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
			printf("[ERROR] %ls:\n\tstring operands aren't allowed in section \"sprite_offsets\"\n", ctx->filename.c_str());
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
		printf("[ERROR] %ls:\n\tUnrecognized assembly instruction in section \"sprite_offsets\": %s\n", ctx->filename.c_str(), command_name);
		return true;
	}
	pos += total_bytes;
	return false;
}
bool aniscript::validate_sprite_offsets_section(text_context* const ctx, size_t& cur_offset)
{
	auto sprite_offsets_section_copy = sprite_offsets_section;
	bool has_errors = false;
	sprite_offsets_section_copy++;
	size_t cur_offset_copy = cur_offset;
	while (sprite_offsets_section_copy != model_3d_section && sprite_offsets_section_copy != bones_3d_section
		&& sprite_offsets_section_copy != chip_section && sprite_offsets_section_copy != text_section && sprite_offsets_section_copy != ctx->lines.end())
	{
		if (sprite_offsets_section_copy->find_first_not_of(" \t") == string::npos)
		{
			sprite_offsets_section_copy++;
			continue;
		}
		if (sprite_offsets_section_copy->find(':') != string::npos)
		{
			sprite_offsets_section_copy++;
			continue;
		}
		has_errors |= validate_sprite_offsets_line(sprite_offsets_section_copy, cur_offset, ctx);
		sprite_offsets_section_copy++;
	}
	if((cur_offset - cur_offset_copy) != 16)
	{
		printf("[ERROR] %ls\n\tsection \"sprite_offsets\" must be total 16 bytes in length\n", ctx->filename.c_str());
		has_errors = true;
	}
	return has_errors;
}

bool aniscript::write_binary_sprite_offsets_line(std::list<string>::iterator &line)
{
	char *command_name;
	char *args[20];
	size_t cnt = boost::count(*line, ',') + 1;
	if (!parse_assembly_instruction(*line, &command_name, cnt, args))
		return true;
	switch (instruction::hash(command_name))
	{
	case instruction::hash("DB"):
	case instruction::hash("db"):
		for(int i = 0; i < cnt; i++)
		{
			auto num = num_from_str(args[i], 1);
			if(num == ULLONG_MAX)
			{
				return true;
			}
			u8((uint8)num);
		}
		break;
	case instruction::hash("DW"):
	case instruction::hash("dw"):
		for(int i = 0; i < cnt; i++)
		{
			auto num = num_from_str(args[i], 2);
			if(num == ULLONG_MAX)
			{
				return true;
			}
			u16((uint16)num);
		}
		break;
	}
	return false;
}
bool aniscript::write_binary_sprite_offsets_section(text_context* const ctx)
{
	auto sprite_offsets_section_copy = sprite_offsets_section;
	bool has_errors = false;
	sprite_offsets_section_copy++;
	size_t cur_offset_copy = cur_offset;
	while (sprite_offsets_section_copy != model_3d_section && sprite_offsets_section_copy != bones_3d_section
		&& sprite_offsets_section_copy != chip_section && sprite_offsets_section_copy != text_section && sprite_offsets_section_copy != ctx->lines.end())
	{
		if (sprite_offsets_section_copy->find_first_not_of(" \t") == string::npos)
		{
			sprite_offsets_section_copy++;
			continue;
		}
		if (sprite_offsets_section_copy->find(':') != string::npos)
		{
			sprite_offsets_section_copy++;
			continue;
		}
		has_errors |= write_binary_sprite_offsets_line(sprite_offsets_section_copy);
		sprite_offsets_section_copy++;
	}
	return has_errors;
}