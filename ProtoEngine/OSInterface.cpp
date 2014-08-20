#include "OSInterface.h"
#include <sstream>
#include <fstream>
#include <cassert>

DateTime::DateTime()
{
    ZeroMemory(this, sizeof(DateTime));
}

DateTime::DateTime( const DateTime& rhs )
{
    (*this) = rhs;
}

DateTime& DateTime::operator=( const DateTime& rhs )
{
    this->year        = rhs.year;
    this->month       = rhs.month;
    this->day         = rhs.day;
    this->day_of_week = rhs.day_of_week;
    this->hour        = rhs.hour;
    this->minute      = rhs.minute;
    this->second      = rhs.second;
    this->millisecond = rhs.millisecond;

    return (*this);
}

void DateTime::asString( std::wstring& outString ) const
{
    outString = static_cast<std::wostringstream*>( &(
        std::wostringstream() 
        << month << "/" << day << "/" << year << " " 
        << hour << ":" << minute << ":" << second << "." << millisecond
        ) )->str();
}

void DateTime::asBinaryTime( BinaryTime& outTime ) const
{
    SYSTEMTIME st;
    st.wYear        = year;
    st.wMonth       = month;
    st.wDay         = day;
    st.wDayOfWeek   = day_of_week;
    st.wHour        = hour;
    st.wMinute      = minute;
    st.wSecond      = second;
    st.wMilliseconds= millisecond;

    FILETIME ft;
    ::SystemTimeToFileTime(&st, &ft);

    LARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    outTime.mBinaryTime = li.QuadPart;
}

void DateTime::setFromWin32SysTime( const SYSTEMTIME& st )
{
    year        = st.wYear;
    month       = st.wMonth;
    day         = st.wDay;
    day_of_week = st.wDayOfWeek;
    hour        = st.wHour;
    minute      = st.wMinute;
    second      = st.wSecond;
    millisecond = st.wMilliseconds;
}

void DateTime::asWin32SysTime( SYSTEMTIME& st ) const
{
    st.wYear         = year;
    st.wMonth        = month;
    st.wDay          = day;
    st.wDayOfWeek    = day_of_week;
    st.wHour         = hour;
    st.wMinute       = minute;
    st.wSecond       = second;
    st.wMilliseconds = millisecond;
}

BinaryTime::BinaryTime()
{
    mBinaryTime = 0;
}

BinaryTime::BinaryTime( const DateTime& inDateTime )
{
    fromeDateTime(inDateTime);
}

void BinaryTime::asDateTime( DateTime& datetime)
{
    LARGE_INTEGER li;
    li.QuadPart = mBinaryTime;

    FILETIME ft;
    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;

    SYSTEMTIME st;
    ::FileTimeToSystemTime(&ft, &st);

    datetime.setFromWin32SysTime(st);
}

void BinaryTime::fromeDateTime( const DateTime& inDateTime )
{
    FILETIME ft;
    SYSTEMTIME st;
    inDateTime.asWin32SysTime(st);
    ::SystemTimeToFileTime(&st, &ft);

    LARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    mBinaryTime = li.QuadPart;
}


FilePath::FilePath() { } 
FilePath::FilePath( const FilePath& rhs ) { }

FilePath& FilePath::operator=( const FilePath& rhs )
{
    mPath.clear();
    for (uint32 i = 0; i < rhs.mPath.size(); ++i)
    {
        mPath[i] = rhs.mPath[i];
    }
    return *this;
}

// Split a string
void split(std::vector<std::wstring>& output, const std::wstring& input, const std::wstring& delimiter)
{
    uint32 start = 0, end = 0;
    while(end != std::wstring::npos)
    {
        end = input.find(delimiter, start);
        output.push_back( input.substr(start, (end == std::wstring::npos) ? std::wstring::npos : end - start ));
        start = ( end > (std::wstring::npos - delimiter.size()) ) ? std::wstring::npos : end + delimiter.size();
    }
}

void FilePath::fromString( const std::wstring pathString )
{
    mPath.clear();
    split(mPath, pathString, std::wstring(L"/"));
}

void FilePath::asString( std::wstring& outPathString )
{
    for (uint32 i = 0; i < mPath.size(); ++i)
    {
        outPathString += mPath[i] + L"/";
    }
}

FilePath FilePath::operator+( const FilePath& other ) const
{
    FilePath result;
    result += *this;
    result += other;

    return result;
}

FilePath& FilePath::operator+=( const FilePath& other )
{
    for (uint32 i = 0; i < other.mPath.size(); ++i)
    {
        mPath.push_back(other.mPath[i]);
    }
    return *this;
}

void OSInterface::getGlobalTime( DateTime& datetime ) 
{
    SYSTEMTIME st;
    ::GetSystemTime(&st);
    datetime.setFromWin32SysTime(st);
}

void OSInterface::getLocalTime( DateTime& datetime ) 
{
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    datetime.setFromWin32SysTime(st);
}

bool OSInterface::fileToString( const std::string& path, std::string& output ) 
{
    std::ifstream fin(path.c_str(), std::ios::in);
    if (fin.is_open())
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        output = ss.str();
        fin.close();

        return true;
    }
    return false;
}

bool OSInterface::stringToFile( const std::string& path, const std::string& input)
{
    std::ofstream ofs(path.c_str(), std::ios::out);
    if (ofs.is_open())
    {
        ofs << input;
        ofs.close();
        return true;
    }
    return false;
}

bool OSInterface::fileToWstring( const std::wstring& path, std::wstring& output )
{
    std::wifstream wifs(path.c_str(), std::ios::in);
    if (wifs.is_open())
    {
        std::wstringstream wss;
        wss << wifs.rdbuf();
        output = wss.str();
        wifs.close();
        return true;
    }
    return false;
}

bool OSInterface::wstringToFile( const std::wstring& path, const std::wstring& input )
{
    std::wofstream wofs(path.c_str(), std::ios::out);
    if (wofs.is_open())
    {
        wofs << input;
        wofs.close();
        return true;
    }
    return false;
}

bool OSInterface::fileToMem( const std::string& path, MemBlock& output )
{
    std::ifstream ifs(path.c_str(), std::ios::binary);
    if (ifs.is_open())
    {
        ifs.seekg(0, ifs.end);
        output.mSize = static_cast<uint32>(ifs.tellg());
        ifs.seekg(0, ifs.beg);

        output.mData = new char[output.mSize];
        ifs.read(output.mData, output.mSize);

        ifs.close();
        return true;
    }
    return false;
}

bool OSInterface::fileToMem( const std::string& path, std::vector<char>& output )
{
    std::ifstream ifs(path.c_str(), std::ios::binary);
    if (ifs.is_open())
    {
        ifs.seekg(0, ifs.end);
        uint32 size = static_cast<uint32>(ifs.tellg());
        ifs.seekg(0, ifs.beg);

        output.resize(size, 0);
        ifs.read(&output[0], size);
        ifs.close();
        return true;
    }
    return false;
}

bool OSInterface::memToFile( const std::string& path, const MemBlock& input )
{
    std::ofstream ofs(path.c_str(), std::ios::binary);
    if (ofs.is_open() && input.mData != NULL && input.mSize != 0 )
    {
        ofs.write(input.mData, input.mSize);
        ofs.close();
        return true;
    }
    return false;
}

bool OSInterface::memToFile( const std::string& path, const std::vector<char>& input )
{
    std::ofstream ofs(path.c_str(), std::ios::binary);
    if (ofs.is_open() && !input.empty())
    {
        ofs.write(&input[0], input.size());
        ofs.close();
        return true;
    }
    return false;
}

class DumbBlock
{
public:
    DumbBlock(bool flag)
    {
        if (flag)
        {
            mInt = 0;
            mBool = true;
            mFloat = 0.0f;
        }
        else
        {
            mInt = 1234;
            mBool = false;
            mFloat = 1234.1234f;
        }
    }
    int mInt;
    bool mBool;
    float mFloat;
};

void osi_test()
{
    // get local time
    DateTime localTime;
    OSInterface::getLocalTime(localTime);
    std::wstring strLocal;
    localTime.asString(strLocal);

    // datetime -> binary time -> datetime
    BinaryTime srcTime(localTime);
    DateTime dstTime;
    srcTime.asDateTime(dstTime);
    std::wstring strDst;
    dstTime.asString(strDst);

    assert( strLocal.compare(strDst) == 0 );

    // get global time
    DateTime globalTime;
    OSInterface::getGlobalTime(globalTime);

    std::wstring strGlobal;
    globalTime.asString(strGlobal);

    // string to file
    std::string filename = "./test.txt";
    std::stringstream  ss;
    for (uint32 i = 0; i < 100; ++i)
    {
        ss <<  i << "_";
    }
    OSInterface::stringToFile(filename, ss.str());

    // file to string
    std::string content;
    OSInterface::fileToString(filename, content);

    filename = "test.bin";

    DumbBlock dumb(false);
    MemBlock mem(reinterpret_cast<char*>(&dumb), sizeof(DumbBlock));
    OSInterface::memToFile(filename, mem);

    MemBlock mem2;
    OSInterface::fileToMem(filename, mem2);
    DumbBlock* d = reinterpret_cast<DumbBlock*>(mem2.mData);
    d->mBool;
    d->mFloat;
    d->mInt;

    std::vector<char> mem3;
    OSInterface::fileToMem(filename, mem3);
    DumbBlock* dd = reinterpret_cast<DumbBlock*>(&mem3[0]);
    dd->mBool;
    dd->mFloat;
    dd->mInt;

}
