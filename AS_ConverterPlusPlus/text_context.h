#include "directives.h"
#pragma once

struct text_context
{
private:
    bool _isGood;

    std::list<string>::iterator curLine;
#define blank (curPos < line->size() && (line->at(curPos) == ' ' || line->at(curPos) == '\t'))
#define notblank  (curPos < line->size() && (line->at(curPos) != ' ' && line->at(curPos) != '\t'))   
    void define_directive()
    { 
        for(std::list<string>::iterator line = _lines.begin(); line != _lines.end(); line++)
        {
            if(line->compare(0, 7, "%define", 0, 7) != 0)
            {
                line++;
                continue;
            }
            //line starts with "%define"
            string defined("(\\b)");
            string definedName;
            char replacedWith[512];
            size_t curPos = 7; // skip "%define"
            while(blank) { curPos++; }
            if(curPos == 7)
            {
                printf("%ls\n\t[ERROR] line %d: invalid directive (did you forget a space?)\n", filename.c_str(), line);
            }
            for(; notblank; curPos++)
            {
                if(curPos == line->size() - 1)
                {
                    printf("[ERROR] %ls:\n\t incomplete %%define directive\n", filename.c_str());
                    throw (69);
                }

                definedName.push_back(line->at(curPos));
            }
            defined.append(definedName);
            defined.append("(\\b)");
            while(blank) { curPos++; }
            size_t index = 0;
            for(; notblank; curPos++)
            {
                replacedWith[index++] = line->at(curPos);
            }
            replacedWith[index] = 0;
            std::regex replaced(defined);
            string undefLine("%undef([ \t]+)");
            undefLine.append(definedName);
            std::regex undefMatch(undefLine);
            for(std::list<string>::iterator i = line; i != _lines.end(); i++)
            {
                if(std::regex_match(*i, undefMatch))
                {
                    _lines.erase(i);
                    break;
                }
                if(i->find(definedName) == std::string::npos) continue;

	            i->assign((string)std::regex_replace(*i, replaced, replacedWith));
            }
            std::list<string>::iterator defineLine = line++;
            _lines.erase(defineLine);
        }
    }
    void macro_directive()
    {        
        for(std::list<string>::iterator line = _lines.begin(); line != _lines.end(); line++)
        {
            if(line->size() < 6 || line->compare(0, 6, "%macro", 0, 6) != 0)
            {
                continue;
            }
            //line starts with "%macro"
            std::list<string>::iterator macroLine = line;            
            std::string defined("\\b");
            char operandsNum[512];
            size_t curPos = 6; // skip "%macro"
            while(blank) { curPos++; }
            if(curPos == 6)
            {
                printf("[ERROR] %ls:\n\t invalid directive (did you forget a space?\n", filename.c_str());
            }
            size_t operands;
            size_t index = 0;
            for(; notblank; curPos++)
            {
                if(curPos == macroLine->size() - 1)
                {
                    operands = 0;
                    goto MAKEMACRO;
                }

                char curChar = macroLine->at(curPos);
                defined.push_back(macroLine->at(curPos));
            }
            while(blank) { curPos++; }        
            for(; notblank; curPos++)
            {
                char curChar = macroLine->at(curPos);
                operandsNum[index++] = macroLine->at(curPos);
            }
            operandsNum[index] = 0;
            operands = std::strtoul(operandsNum, NULL, 0);
            if(operands == 0)
            {
                printf("[ERROR] %ls:\n\t unable to parse number of directive operands from: %s\n", filename.c_str(), operandsNum);
                throw(69);
            }
            MAKEMACRO:
            std::string macroName = defined.substr(2);
            size_t macroLength = 1;
            for(int i = 0; i < operands; i++)
            {
                if(i == 0)
                    defined.append("([ \t]+[\\w0-9]+[ \t]*),");
                else if(i == operands - 1)
                    defined.append("([ \t]*[\\w0-9]+)");
                else
                    defined.append("([ \t]*[\\w0-9]+[ \t]*),");
            }
            std::string replacedWith;
            while(true)
            {
                line++;
                if(line == _lines.end())
                {
                    printf("[ERROR] %ls:\n\t missing %%endmacro directive\n", filename.c_str());
                    throw(69);
                }
                if(line->find(macroName) != string::npos)
                {
                    printf("[ERROR] %ls:\n\t detected infinite recursion in macro\n", filename.c_str());
                }
                macroLength++;
                if(line->compare(0, 9, "%endmacro", 0, 9) == 0)
                {
                    line++;
                    break;
                }
                replacedWith.append(*line);
                replacedWith.push_back('\n');
            }
            std::regex macro(defined);
            for(std::list<string>::iterator i = line; i != _lines.end(); i++)
            {
                if(i->find(macroName) == std::string::npos) continue;
                std::string str;
	            std::istringstream ss((string)std::regex_replace(*i, macro, replacedWith));
                size_t newLineNum = 0;
                for (; std::getline(ss, str); newLineNum++)
                {                    
                    _lines.insert(i, str);
                }
                std::list<string>::iterator toBeRemoved = i;
                while(newLineNum--) {i++;}
                _lines.erase(toBeRemoved);
            }
            
            while(macroLength--)
            {
                std::list<string>::iterator defineLine = macroLine++;
                _lines.erase(defineLine);
            }          
        }
    }

public:

    std::list<string> _lines;
    std::filesystem::path filename;
    inline bool isGood() { return _isGood; }
    inline std::list<string>::iterator line()
    {
        return curLine++;
    }
    text_context(const std::filesystem::path* const fPath)
    {
        uintmax_t fSize = std::filesystem::file_size(*fPath);
	    std::ifstream file(*fPath);
        if(!file.good())
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
                _lines.emplace_back(std::string(line));
            }
            file.close();
            _isGood = true;
	    }
	    catch(const std::exception& e)
	    {
		    std::cerr << e.what() << '\n';
            _isGood = false;
	    }
        define_directive();
        macro_directive();
    }


    ~text_context()
    {
        
    }
};