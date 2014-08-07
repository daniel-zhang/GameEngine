#ifndef OPERATING_SYSTEM_INTERFACE_H
#define OPERATING_SYSTEM_INTERFACE_H

#include "typedefs.h"
#include <string>
#include <vector>

/*
A wrapper around OS to unify related OS operations.
    file operation
    dir navigation
    date time
*/

class UTCTime;
class DateTime 
{
public:
    DateTime();
    DateTime& operator=(const DateTime& rhs);
    DateTime(const DateTime& rhs);

    void asString(std::wstring& outString);
    void asUTC(UTCTime& outTime);

    uint16 year;
    uint16 month;
    uint16 day;
    uint16 day_of_week;
    uint16 hour;
    uint16 minute;
    uint16 second;
    uint16 millisecond;
};

class UTCTime
{
public:
    UTCTime();
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
    UTCTime mLastModifiedTime;
    bool mIsDir;
};

class OSInterface 
{
public:
    OSInterface();
    ~OSInterface();

    void mount(const FilePath& path);
    bool ls(std::vector<PhysicalFileInfo>& fileInfoCollection, const FilePath& path);
    void getCurrentTime(UTCTime* datetime);

protected:
    FilePath mMountPoint; 
};

#endif