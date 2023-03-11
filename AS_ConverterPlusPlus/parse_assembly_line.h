#include <string>
#include <iostream>
#include <list>
#pragma once

bool parse_assembly_instruction(std::list<std::string>::iterator& string, char** name, size_t expected_args_count, char* args[20]);
bool parse_assembly_instruction(std::string& string, char** name, size_t expected_args_count, char* args[20]);