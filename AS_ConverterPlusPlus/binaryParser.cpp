#include "directives.h"
#include "aniscript.h"

bool ParseBinary(const std::filesystem::path* fPath)
{
	namespace fs = std::filesystem;
	context ctx(fPath);
	if(!ctx.isGood()) return false;
	aniscript as;
	std::string text;
	if(!as.ParseFromBinary(&ctx, &text)) return false;
	fs::path folder("out");
	std::filesystem::path filename = ctx.filename;
	fs::path extension(".as");
	filename.replace_extension(extension);
	folder /= filename;
    std::ofstream txtfile;
    txtfile.open(folder, ios::out | ios::trunc);
	if (!txtfile.is_open()) { printf("unable to write in this directory"); return false; }
	txtfile.write(text.data(), text.size());
	txtfile.close();
	return true;
}