#include "directives.h"
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
	//aniscript as;
	//std::string text;
	//try
	//{
	//	if (!as.ParseFromBinary(&ctx, &text))
	//		return false;
	//}
	//catch (...)
	//{
	//	//printf(text.data());
	//	printf("%ls:\n\ttried to read outside the bounds of the file\n", fPath->c_str());
	//}
	string text;
	for(string i : ctx._lines)
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
    std::filesystem::path folder("out");
	std::filesystem::create_directory(folder);
    const char* fpath = "AS32400 ._DT";
    //bool succ = ParseBinary("AS32400 ._DT");
    
    //for( const auto & entry : std::filesystem::directory_iterator( "DT30" ) )
    //{
    //    //if(entry.path().filename() == "") { continue; }
    //    bool succ = ParseBinary(&entry.path());
    //}
	for( const auto & entry : std::filesystem::directory_iterator( "out" ) )
    {
        //if(entry.path().filename() == "") { continue; }
        bool succ = Preprocess(&entry.path());
    }
    return EXIT_SUCCESS;
}
