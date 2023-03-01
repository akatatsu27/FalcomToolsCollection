#include "directives.h"
#include "binary_context.h"
#include "text_context.h"
#pragma once

#define uint8 uint8_t

struct instruction
{
    size_t offset;
    uint8_t opcode;

	uint16 targetOffset = 0;
	char* labelName; //the label of the target offset

    static instruction first_pass_binary(binary_context *const ctx);
    static instruction first_pass_text(text_context *const ctx);
    void second_pass_binary(binary_context* const ctx, string* const text);
    void second_pass_text(text_context* const ctx, vector<char>* const binary);
};

