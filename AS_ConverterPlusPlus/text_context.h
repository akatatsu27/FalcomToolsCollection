#include <filesystem>
#include <list>
#include <sstream>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#pragma once

struct text_context
{
private:
	bool _isGood;

	std::string &replace_except_in_quotes(std::string &content, const std::string &original, const std::string &replacement)
	{
		for (size_t pos = 0U; pos < content.size(); /*incremented in body*/)
		{
			size_t nextOriginal = content.find(original, pos);
			if (nextOriginal == std::string::npos)
			{
				break; // All occurences replaced
			}
			size_t nextQuote = content.find('\"', pos);
			bool skipQuotes = (nextQuote != std::string::npos) && (nextQuote < nextOriginal);
			if (skipQuotes)
			{
				nextQuote = content.find('\"', nextQuote + 1);
				if (nextQuote == std::string::npos)
				{
					break; // Missing closing quote
				}
				pos = nextQuote + 1;
				continue;
			}

        	if(nextOriginal + original.size() < content.size())
        	{
				char next_char = content.at(nextOriginal + original.size());
	    		if(!isblank(next_char) && next_char != ',')
		    	{
        		    pos += original.size();
    		       	continue;
			    }
        	}
			// Actually replace original with replacement
			content.replace(nextOriginal, original.size(), replacement);
			pos += replacement.size() + 1;
		}
		return content;
	}

	std::list<string>::iterator curLine;
#define blank (curPos < line->size() && (line->at(curPos) == ' ' || line->at(curPos) == '\t'))
#define notblank (curPos < line->size() && (line->at(curPos) != ' ' && line->at(curPos) != '\t'))
	void define_directive(std::list<string>::iterator &line)
	{
		// line starts with "%define"
		if (line->find_first_of("+-*/\\#@!~`%^&()=[]{}|;:'<>,.", 7) != string::npos)
		{
			printf("[ERROR] %ls:\n\tillegal character in %%define directive\n", filename.c_str());
			throw(69);
		}
		string definedName;
		size_t curPos = 7; // skip "%define"
		while (blank)
		{
			curPos++;
		}
		if (curPos == 7)
		{
			printf("[ERROR] %ls:\n\tinvalid directive (did you forget a space?)\n", filename.c_str());
		}
		for (; notblank; curPos++)
		{
			if (curPos == line->size() - 1)
			{
				printf("[ERROR] %ls:\n\tincomplete %%define directive\n", filename.c_str());
				throw(69);
			}
			definedName.push_back(line->at(curPos));
		}
		while (blank)
		{
			curPos++;
		}
		string replaced_with;
		for (; notblank; curPos++)
		{
			replaced_with.push_back(line->at(curPos));
		}
		string undefLine("%undef([ \t]+)");
		undefLine.append(definedName);
		boost::regex undefMatch(undefLine);
		for (std::list<string>::iterator i = line; i != lines.end(); i++)
		{
			if (boost::regex_match(*i, undefMatch))
			{
				lines.erase(i);
				break;
			}
			if (i->find(definedName) == std::string::npos)
				continue;
			replace_except_in_quotes(*i, definedName, replaced_with);
		}
		std::list<string>::iterator defineLine = line++;
		lines.erase(defineLine);
	}
	void macro_directive(std::list<string>::iterator &line)
	{
		// line starts with "%macro"
		std::list<string>::iterator macroLine = line;
		std::string defined("\\b");
		char operandsNum[512];
		size_t curPos = 6; // skip "%macro"
		while (blank)
		{
			curPos++;
		}
		if (curPos == 6)
		{
			printf("[ERROR] %ls:\n\tinvalid directive (did you forget a space?\n", filename.c_str());
		}
		size_t operands;
		size_t index = 0;
		for (; notblank; curPos++)
		{
			if (curPos == macroLine->size() - 1)
			{
				operands = 0;
				goto MAKEMACRO;
			}
			char curChar = macroLine->at(curPos);
			defined.push_back(macroLine->at(curPos));
		}
		while (blank)
		{
			curPos++;
		}
		for (; notblank; curPos++)
		{
			char curChar = macroLine->at(curPos);
			operandsNum[index++] = macroLine->at(curPos);
		}
		operandsNum[index] = 0;
		operands = std::strtoul(operandsNum, NULL, 0);
		if (operands == 0)
		{
			printf("[ERROR] %ls:\n\tunable to parse number of directive operands from: %s\n", filename.c_str(), operandsNum);
			throw(69);
		}
	MAKEMACRO:
		std::string macroName = defined.substr(2);
		size_t macroLength = 1;
		for (int i = 0; i < operands; i++)
		{
			if (i == 0)
				defined.append("([ \t]+[\\w0-9]+[ \t]*),");
			else if (i == operands - 1)
				defined.append("([ \t]*[\\w0-9]+)");
			else
				defined.append("([ \t]*[\\w0-9]+[ \t]*),");
		}
		std::string replacedWith;
		while (true)
		{
			line++;
			if (line == lines.end())
			{
				printf("[ERROR] %ls:\n\tmissing %%endmacro directive\n", filename.c_str());
				throw(69);
			}
			if (line->find(macroName) != string::npos)
			{
				printf("[ERROR] %ls:\n\tdetected infinite recursion in macro\n", filename.c_str());
				throw(69);
			}
			macroLength++;
			if (line->compare(0, 9, "%endmacro", 0, 9) == 0)
			{
				line++;
				break;
			}
			replacedWith.append(*line);
			replacedWith.push_back('\n');
		}
		boost::regex macro(defined);
		std::list<string>::iterator it = line;
		while (it != lines.end())
		{
			if (it->find(macroName) == std::string::npos)
			{
				it++;
				continue;
			}
			std::string str;
			std::istringstream ss((string)boost::regex_replace(*it, macro, replacedWith));
			while (std::getline(ss, str))
			{
				lines.insert(it, str);
			}
			std::list<string>::iterator toBeRemoved = it++;
			lines.erase(toBeRemoved);
		}

		while (macroLength--)
		{
			std::list<string>::iterator defineLine = macroLine++;
			lines.erase(defineLine);
		}
	}
#undef blank
#undef notblank
	void process_directives()
	{
		for (std::list<string>::iterator line = lines.begin(); line != lines.end();)
		{
			if (line->compare(0, 7, "%define", 0, 7) == 0)
			{
				define_directive(line);
			}
			else if (line->compare(0, 6, "%macro", 0, 6) == 0)
			{
				macro_directive(line);
			}
			else
				line++;
		}
	}
	void remove_comments()
	{
		for (std::list<string>::iterator line = lines.begin(); line != lines.end(); line++)
		{
			size_t pos = line->find(';');
			if (pos != string::npos)
			{
				line->resize(pos);
			}
		}
	}

public:
	std::list<string> lines;
	std::filesystem::path filename;
	inline bool isGood() { return _isGood; }
	inline std::list<string>::iterator line()
	{
		return curLine++;
	}
	text_context(const std::filesystem::path *const fPath)
	{
		uintmax_t fSize = std::filesystem::file_size(*fPath);
		std::ifstream file(*fPath);
		if (!file.good())
		{
			_isGood = false;
			return;
		}
		filename = fPath->filename();
		try
		{
			std::string line;
			while (std::getline(file, line))
			{
				lines.emplace_back(std::string(line));
			}
			file.close();
			_isGood = true;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			_isGood = false;
		}
		remove_comments();
		process_directives();
	}

	~text_context()
	{
	}
};