#pragma once

#include <string>
#include <vector>

struct SqliteHeader
{
    std::string head;
    int pageSize;
    int fileFormatWriteVersion;
    int fileFormatReadVersion;
    int bytesOfUnused;
    int maximumEmbeddedPayloadFraction;
    int minimumEmbeddedPayloadFraction;
    int leafPayloadFraction;
    int fileChangeCounter;
    int pageCount;
    int firstFreelistPage;
    int freelistPageCount;
//    40	4	The schema cookie.
    int schemaFormat; // Supported schema formats are 1, 2, 3, and 4.
    int defaultPageCacheSize;
    int numberOfLargestRootPage;
    int databaseTextEncoding; // A value of 1 means UTF-8. A value of 2 means UTF-16le. A value of 3 means UTF-16be.
    int userVersion;
    bool incrementalVacuumMode;
    int applicationId;
//    72	20	Reserved for expansion. Must be zero.
    int versionValidNumber;
    int sqliteVersionNumber;
};

class IGui
{
public:
    virtual ~IGui(){}
    virtual void DisplayHeader(const std::vector<std::string>& header) = 0;
};

class IDbReader
{
public:
    virtual ~IDbReader(){}
    virtual bool ReadFilePath(const std::string& filePath) = 0;
    virtual bool CheckHeader() = 0;
    virtual std::string GetHeaderString() = 0;
    virtual int GetPageSize() = 0;
    virtual int GetFormatWriteVersion() = 0;
    virtual int GetFormatReadVersion() = 0;
    virtual int GetUnsedBytes() = 0;
    virtual int GetMaximumEmbeddedPayloadFraction() = 0;
    virtual int GetMinimumEmbeddedPayloadFraction() = 0;
    virtual int GetLeafPayloadFraction() = 0;
    virtual int GetFileChangeCounter() = 0;
    virtual int GetPageCount() = 0;
    virtual int GetFirstFreelistPage() = 0;
    virtual int GetFreelistPageCount() = 0;
//    40	4	The schema cookie.
    virtual int GetSchemaFormat() = 0; // Supported schema formats are 1, 2, 3, and 4.
    virtual int GetDefaultPageCacheSize() = 0;
    virtual int GetNumberOfLargestRootPage() = 0;
    virtual int GetDatabaseTextEncoding() = 0; // A value of 1 means UTF-8. A value of 2 means UTF-16le. A value of 3 means UTF-16be.
    virtual int GetUserVersion() = 0;
    virtual bool GetIncrementalVacuumMode() = 0;
    virtual int GetApplicationId() = 0;
//    72	20	Reserved for expansion. Must be zero.
    virtual int GetVersionValidNumber() = 0;
    virtual int GetSqliteVersionNumber() = 0;
    virtual bool IsEmpty() = 0;
};
