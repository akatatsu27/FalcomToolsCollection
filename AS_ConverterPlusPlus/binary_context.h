#include "directives.h"
#pragma once

struct binary_context
{
private:
    bool _isGood;
    size_t _size;
    char* buffer;
    
public:

    size_t position;
    std::filesystem::path filename;
    inline bool isGood() { return _isGood; }
    inline size_t size() { return _size; }
    inline char u8() 
    { 
        if(position >= _size) throw (69);
        char byte = *(char*)(buffer + position);
        position += 1; 
        return byte;   
    }
    inline uint16 u16() 
    {
        if(position >= _size -1)
            throw (69);
        uint16 val = *(uint16*)(buffer + position);
        position += 2; 
        return val;   
    }
    inline uint32 u32() 
    {
        if(position >= _size -3) throw (69);
        uint32 val = *(uint32*)(buffer + position);
        position += 4; 
        return val;   
    }
    inline char* cstring()
    {
        char* str = (buffer + position);
        char curByte;
        do
        {
            curByte = u8();
        } while(curByte);
        return str;
    }
    inline void cstring_array(vector<char*>* arr)
    {
        do
        {
            char* str = cstring();
            if(*str == 0)
            {
                return;
            }
            arr->push_back(str);
        } while (1);
    }
    binary_context(const std::filesystem::path* const fPath)
    {
        uintmax_t fSize = std::filesystem::file_size(*fPath);
	    std::ifstream file(*fPath, std::ios::binary);
        filename = fPath->filename();
	    if(!file.good())
        { 
            _isGood = false;
            return;
        }
        position = 0;
        _size = fSize;
        buffer = new char[fSize];
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
    ~binary_context()
    {
        delete[] buffer;
    }
};