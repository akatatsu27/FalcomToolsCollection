#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <array>
#include <regex>
#include <fstream>
#include <filesystem>
#include <iostream>

#define uint16 uint16_t
#define uint32 uint32_t

using std::string;
using std::vector;

template<size_t N>
using FCArray = std::vector<std::array<char,N>>; // fixed size char array vector