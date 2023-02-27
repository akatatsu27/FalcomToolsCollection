#include "directives.h"
#include "context.h"
#pragma once

#define uint8 uint8_t

struct instruction
{
    size_t offset;
    uint8_t opcode;

	uint16 targetOffset = 0;
	char* labelName; //the label of the target offset

    static instruction first_pass(context *const ctx);
    void second_pass(context* const ctx, string* const text);
};

