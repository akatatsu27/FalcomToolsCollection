#include <filesystem>
#include <fstream>
#include "aniscript.h"
#include "asmag.h"
#include "asitem.h"
#include "asbs.h"

bool ParseBinaryAniscript(const std::filesystem::path *fPath, base_aniscript* const as)
{
	namespace fs = std::filesystem;
	binary_context ctx(fPath);
	if (!ctx.isGood())
		return false;
	std::string text;
	try
	{
		if (!as->parse_from_binary(&ctx, &text))
			return false;
	}
	catch (...)
	{
		//printf(text.data());
		printf("%ls:\n\ttried to read outside the bounds of the file\n", fPath->c_str());
	}

    std::filesystem::path folder("out");
	std::filesystem::create_directory(folder);
	std::filesystem::path filename = ctx.filename;
	fs::path extension(".as");
	filename.replace_extension(extension);
	folder /= filename;
	std::ofstream txtfile;
	txtfile.open(folder, ios::out | ios::trunc);
	if (!txtfile.is_open())
	{
		printf("unable to write in this directory");
		return false;
	}
	txtfile.write(text.data(), text.size());
	txtfile.close();
	printf("Successfully parsed file ");
	wprintf(fPath->c_str());
	printf("\n");
	return true;
}
bool ParseText(const std::filesystem::path *fPath, base_aniscript* as)
{
	namespace fs = std::filesystem;
	text_context ctx(fPath);
	if (!ctx.isGood())
		return false;
	if (!as->compile_from_text(&ctx))
		return false;
    std::filesystem::path folder("outbin");
	std::filesystem::create_directory(folder);
	std::filesystem::path filename = ctx.filename;
	fs::path extension("._DT");
	filename.replace_extension(extension);
	folder /= filename;
	std::ofstream binfile;
	binfile.open(folder, ios::out | ios::binary | ios::trunc);
	if (!binfile.is_open())
	{
		printf("unable to write to this directory");
		return false;
	}
	binfile.write(as->assembled_binary, as->assembled_binary_size);
	binfile.close();
	printf("Successfully assembled file ");
	wprintf(fPath->c_str());
	printf("\n");
	return true;
}

void decide(const std::filesystem::path *fPath)
{
	//is the file ".as" or "._DT"?
	if(!fPath->has_extension())
	{
		return;
	}
	else if(fPath->extension() == "._DT")
	{
		if(fPath->filename() == L"ASMAG000._DT")
		{
			asmag mag;
			bool succ = ParseBinaryAniscript(fPath, &mag);
		}
		else if(fPath->filename() == L"ASITEM  ._DT")
		{
			asitem item;
			bool succ = ParseBinaryAniscript(fPath, &item);
		}
		else if (!wcsncmp(fPath->filename().c_str(), L"AS", 2))
		{
			aniscript as;
        	bool succ = ParseBinaryAniscript(fPath, &as);
		}
		else if (!wcsncmp(fPath->filename().c_str(), L"BS", 2))
		{
			asbs bs;
			bool succ = ParseBinaryAniscript(fPath, &bs);
		}
	}
	else if(fPath->extension() == ".as")
	{
		if(fPath->filename() == L"ASMAG000.as")
		{
			asmag mag;
			bool succ = ParseText(fPath, &mag);
		}
		else if(fPath->filename() == L"ASITEM  .as")
		{
			asitem item;
			bool succ = ParseText(fPath, &item);
		}
		else if (!wcsncmp(fPath->filename().c_str(), L"AS", 2))
		{
			aniscript as;
        	bool succ = ParseText(fPath, &as);
		}
		else if (!wcsncmp(fPath->filename().c_str(), L"BS", 2))
		{
			asbs bs;
			bool succ = ParseText(fPath, &bs);
		}
	}
	return;
}

int main(int argc, char** argv)
{    
    if(argc <= 1)
    {
        printf("AS_Converter version 1.42069\nUsage:\n\tAS_converter.exe [Directory/File]");
        return 0;
    }
    std::filesystem::path path(argv[1]);
    std::error_code ec;
    if(std::filesystem::is_directory(path, ec))
    {
        for(auto & entry : std::filesystem::directory_iterator(path))
    	{
			decide(&entry.path());
    	}
    }
    else if (std::filesystem::is_regular_file(path, ec))
    {
		decide(&path);
    }
    else
    {
        //if(ec)
        //{
        //    
        //}
        //else
        //    printf("wut");
		printf("[ERROR] Input was not a file or directory");
    }

    return EXIT_SUCCESS;
}
