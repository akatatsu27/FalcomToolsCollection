#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "aniscript.h"

bool ParseBinary(const std::filesystem::path* fPath);

int main(int argc, char** argv)
{
    std::filesystem::path folder("out");
	std::filesystem::create_directory(folder);
    const char* fpath = "AS32400 ._DT";
    //bool succ = ParseBinary("AS32400 ._DT");
    
    for( const auto & entry : std::filesystem::directory_iterator( "DT30" ) )
    {
        //if(entry.path().filename() == "") { continue; }
        bool succ = ParseBinary(&entry.path());
    }
    return EXIT_SUCCESS;
}
