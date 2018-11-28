#pragma once

#include <gmock/gmock.h>

#include "Interfaces.h"

class GuiMock: public IGui
{
public:
    ~GuiMock(){}
    MOCK_METHOD1(DisplayHeader, void(const std::vector<std::string>& header));
};

class DbReaderMock: public IDbReader
{
public:
    ~DbReaderMock(){}
    MOCK_METHOD1(ReadFilePath, bool(const std::string& filePath));
    MOCK_METHOD0(CheckHeader, bool());
    MOCK_METHOD0(GetHeaderString, std::string());
    MOCK_METHOD0(GetPageSize, int());
    MOCK_METHOD0(GetFormatWriteVersion, int());
    MOCK_METHOD0(GetFormatReadVersion, int());
    MOCK_METHOD0(GetUnsedBytes, int());
    MOCK_METHOD0(GetMaximumEmbeddedPayloadFraction, int());
    MOCK_METHOD0(GetMinimumEmbeddedPayloadFraction, int());
    MOCK_METHOD0(GetLeafPayloadFraction, int());
    MOCK_METHOD0(GetFileChangeCounter, int());
    MOCK_METHOD0(GetPageCount, int());
    MOCK_METHOD0(GetFirstFreelistPage, int());
    MOCK_METHOD0(GetFreelistPageCount, int());
//    40	4	The schema cookie.
    MOCK_METHOD0(GetSchemaFormat, int());
    MOCK_METHOD0(GetDefaultPageCacheSize, int());
    MOCK_METHOD0(GetNumberOfLargestRootPage, int());
    MOCK_METHOD0(GetDatabaseTextEncoding, int());
    MOCK_METHOD0(GetUserVersion, int());
    MOCK_METHOD0(GetIncrementalVacuumMode, bool());
    MOCK_METHOD0(GetApplicationId, int());
//    72	20	Reserved for expansion. Must be zero.
    MOCK_METHOD0(GetVersionValidNumber, int());
    MOCK_METHOD0(GetSqliteVersionNumber, int());
    MOCK_METHOD0(IsEmpty, bool());
};
