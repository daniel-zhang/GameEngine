#include "OperatingSystemInterface.h"
#include <sstream>
#include <windows.h>

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

void DateTime::asString( std::wstring& outString )
{
    outString = static_cast<std::wostringstream*>( &(
        std::wostringstream() 
        << month << "/" << day << "/" << year << " " 
        << hour << ":" << minute << ":" << second << "." << (1.f * millisecond) / 1000.f
        ) )->str();
}

void DateTime::asUTC( UTCTime& outTime )
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
    SystemTimeToFileTime(&st, &ft);

    LARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    outTime.mBinaryTime = li.QuadPart;
}

UTCTime::UTCTime()
{
    mBinaryTime = 0;
}

void UTCTime::asDateTime( DateTime& datetime)
{
    LARGE_INTEGER li;
    li.QuadPart = mBinaryTime;

    FILETIME ft;
    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;

    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);

    datetime.year        = st.wYear;
    datetime.month       = st.wMonth;
    datetime.day         = st.wDay;
    datetime.day_of_week = st.wDayOfWeek;
    datetime.hour        = st.wHour;
    datetime.minute      = st.wMinute;
    datetime.second      = st.wSecond;
    datetime.millisecond = st.wMilliseconds;
}


FilePath::FilePath()
{
}

FilePath::FilePath( const FilePath& rhs )
{

}

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


OSInterface::OSInterface()
{

}

OSInterface::~OSInterface()
{

}

void OSInterface::mount( const FilePath& path )
{
    mMountPoint = path;
}

bool OSInterface::ls( std::vector<PhysicalFileInfo>& fileInfoCollection, const FilePath& path )
{
    FilePath fullPath = mMountPoint + path;
    std::wstring strPath;
    fullPath.asString(strPath);

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    if ((hFind = FindFirstFile(strPath.c_str(), &fdFile)) == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    do 
    {
        if(strPath.compare(L".") != 0 && strPath.compare(L"..") != 0)
        {
        }
    } while (FindNextFile(hFind, &fdFile));

    return true;
}

