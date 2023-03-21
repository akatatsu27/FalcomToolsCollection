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

	fs::path folder("out");
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
	fs::path folder("outbin");
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
	return true;
}
int main(int argc, char** argv)
{    
	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
	auto t1 = high_resolution_clock::now();
    std::filesystem::path folder("out");
	std::filesystem::create_directory(folder);
    
    for( const auto & entry : std::filesystem::directory_iterator( "DT30" ) )
    {

		if(entry.path().filename() == L"ASMAG000._DT")
		{
			asmag mag;
			bool succ = ParseBinaryAniscript(&entry.path(), &mag);
		}
		else if(entry.path().filename() == L"ASITEM  ._DT")
		{
			asitem item;
			bool succ = ParseBinaryAniscript(&entry.path(), &item);
		}
		else if (!wcsncmp(entry.path().filename().c_str(), L"AS", 2))
		{
			aniscript as;
        	bool succ = ParseBinaryAniscript(&entry.path(), &as);
		}
		else if (!wcsncmp(entry.path().filename().c_str(), L"BS", 2))
		{
			asbs bs;
			bool succ = ParseBinaryAniscript(&entry.path(), &bs);
		}
    }
	printf("completed processing binaries\n");
	for( const auto & entry : std::filesystem::directory_iterator( "out" ) )
    {
		if(entry.path().filename() == L"ASMAG000.as")
		{
			asmag mag;
			bool succ = ParseText(&entry.path(), &mag);
		}
		else if(entry.path().filename() == L"ASITEM  .as")
		{
			asitem item;
			bool succ = ParseText(&entry.path(), &item);
		}
		else if (!wcsncmp(entry.path().filename().c_str(), L"AS", 2))
		{
			aniscript as;
        	bool succ = ParseText(&entry.path(), &as);
		}
		else if (!wcsncmp(entry.path().filename().c_str(), L"BS", 2))
		{
			asbs bs;
			bool succ = ParseText(&entry.path(), &bs);
		}
    }
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";

    return EXIT_SUCCESS;
}
