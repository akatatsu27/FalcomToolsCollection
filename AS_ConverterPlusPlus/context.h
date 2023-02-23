#include "directives.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#pragma once

struct context
{
private:
    bool _isGood;
    size_t _position;
    size_t _size;
    char* bufferStart;
    char* buffer;
    
public:

    std::filesystem::path filename;
    inline bool isGood() { return _isGood; }
    inline size_t position(){ return _position; }
    inline size_t size() { return _size; }
    inline char u8() 
    { 
        char byte = *(char*)buffer;
        buffer += 1;
        _position += 1; 
        return byte;   
    }
    inline uint16 u16() 
    {
        uint16 val = *(uint16*)buffer;
        buffer += 2;
        _position += 2; 
        return val;   
    }
    inline uint32 u32() 
    {
        uint32 val = *(uint32*)buffer;
        buffer += 4;
        _position += 4; 
        return val;   
    }
    inline string cstring()
    {
        string str(buffer);
        size_t size = str.size();
        buffer += size + 1;
        _position += size + 1;
        return str;
    }
    inline void cstring_array(vector<string>* arr)
    {
        do
        {
            string str = cstring();
            if(str == "\0")
            {
                return;
            }
            arr->push_back(str);
        } while (1);
    }
    context(const std::filesystem::path* fPath)
    {
        uintmax_t fSize = std::filesystem::file_size(*fPath);
	    std::ifstream file(*fPath, std::ios::binary);
        filename = fPath->filename();
	    if(!file.good())
        { 
            _isGood = false;
            return;
        }
        _position = 0;
        _size = fSize;
        bufferStart = new char[fSize];
        buffer = bufferStart;
	    try
	    {
		    file.read(buffer, fSize);
		    file.close();
            _isGood = true;
	    }
	    catch(const std::exception& e)
	    {
		    std::cerr << e.what() << '\n';
            _isGood = false;
	    }
    }
    ~context()
    {
        delete[] bufferStart;
    }
};