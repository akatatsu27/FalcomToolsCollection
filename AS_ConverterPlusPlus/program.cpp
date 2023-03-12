#include <filesystem>
#include <fstream>
#include "aniscript.h"

bool ParseBinary(const std::filesystem::path *fPath)
{
	namespace fs = std::filesystem;
	binary_context ctx(fPath);
	if (!ctx.isGood())
		return false;
	aniscript as;
	std::string text;
	try
	{
		if (!as.ParseFromBinary(&ctx, &text))
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
bool Preprocess(const std::filesystem::path *fPath)
{
	namespace fs = std::filesystem;
	text_context ctx(fPath);
	if (!ctx.isGood())
		return false;
	aniscript as;
	std::vector<char> binary;
	if (!as.CompileFromText(&ctx, &binary))
		return false;
	//}
	//catch (...)
	//{
	//	//printf(text.data());
	//	printf("%ls:\n\ttried to read outside the bounds of the file\n", fPath->c_str());
	//}
	string text;
	for(string i : ctx.lines)
	{
		text.append(i);
		text.push_back('\n');
	}
	fs::path folder("outbin");
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
        //if(entry.path().filename() == "") { continue; }
        bool succ = ParseBinary(&entry.path());
    }
	printf("completed processing binaries\n");
	for( const auto & entry : std::filesystem::directory_iterator( "out" ) )
    {
        //if(entry.path().filename() == "") { continue; }
        bool succ = Preprocess(&entry.path());
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
