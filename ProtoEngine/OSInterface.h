#ifndef OS_INTERFACE_H
#define OS_INTERFACE_H

#include "typedefs.h"
#include <string>
#include <vector>
#include <windows.h>

/*
A wrapper around OS to unify related OS operations.
    file operation
    dir navigation
    date time
*/

class BinaryTime;
class DateTime 
{
public:
    DateTime();
    DateTime& operator=(const DateTime& rhs);
    DateTime(const DateTime& rhs);

    void setFromWin32SysTime(const SYSTEMTIME& st);
    void asWin32SysTime(SYSTEMTIME& st) const;
    void asString(std::wstring& outString) const;
    void asBinaryTime(BinaryTime& outTime) const;

    uint16 year;
    uint16 month;
    uint16 day;
    uint16 day_of_week;
    uint16 hour;
    uint16 minute;
    uint16 second;
    uint16 millisecond;

};

class BinaryTime
{
public:
    BinaryTime();
    BinaryTime(const DateTime& inDateTime);
    void fromeDateTime(const DateTime& inDateTime);
    void asDateTime(DateTime& outDateTime);

    uint64 mBinaryTime;
};

class FilePath
{
public:
    FilePath();
    FilePath(const FilePath& rhs);
    FilePath& operator=(const FilePath& rhs);
    FilePath& operator+=(const FilePath& other);
    FilePath operator+(const FilePath& other) const;

    void fromString(const std::wstring inPathString);
    void asString(std::wstring& outPathString);

    std::vector<std::wstring> mPath;
};

class PhysicalFileInfo
{
public:
    std::wstring mFileName;
    std::wstring mExtension;
    uint64 mSize;
    BinaryTime mLastModifiedTime;
    bool mIsDir;
};

struct MemBlock
{
    MemBlock()
    {
        mData = NULL;
        mSize = 0;
    }
    MemBlock(char* data, uint32 size):mData(data), mSize(size) { } 
    ~MemBlock()
    {
        if (!mData) delete mData;
    }
    char* mData;
    uint32  mSize;
};

class OSInterface 
{
public:
    static void getGlobalTime(DateTime& datetime); 
    static void getLocalTime(DateTime& datetime);

    static bool fileToString(const std::string& path, std::string& output);
    static bool stringToFile(const std::string& path, const std::string& input);

    static bool fileToWstring(const std::wstring& path, std::wstring& output);
    static bool wstringToFile(const std::wstring& path, const std::wstring& input);

    static bool fileToMem(const std::string& path, MemBlock& output);
    static bool memToFile(const std::string& path, const MemBlock& input);

    static bool fileToMem(const std::string& path, std::vector<char>& output);
    static bool memToFile(const std::string& path, const std::vector<char>& input);
};

void osi_test();

#endif