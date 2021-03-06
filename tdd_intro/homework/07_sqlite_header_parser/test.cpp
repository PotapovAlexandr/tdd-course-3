/*
Implement application for display sqlite header structure of given file.
Application takes path to file and displays header structure
Sqlite header is described here https://www.sqlite.org/fileformat.html

Offset	Size	Description
0	16	The header string: "SQLite format 3\000"
16	2	The database page size in bytes. Must be a power of two between 512 and 32768 inclusive, or the value 1 representing a page size of 65536.
18	1	File format write version. 1 for legacy; 2 for WAL.
19	1	File format read version. 1 for legacy; 2 for WAL.
20	1	Bytes of unused "reserved" space at the end of each page. Usually 0.
21	1	Maximum embedded payload fraction. Must be 64.
22	1	Minimum embedded payload fraction. Must be 32.
23	1	Leaf payload fraction. Must be 32.
24	4	File change counter.
28	4	Size of the database file in pages. The "in-header database size".
32	4	Page number of the first freelist trunk page.
36	4	Total number of freelist pages.
40	4	The schema cookie.
44	4	The schema format number. Supported schema formats are 1, 2, 3, and 4.
48	4	Default page cache size.
52	4	The page number of the largest root b-tree page when in auto-vacuum or incremental-vacuum modes, or zero otherwise.
56	4	The database text encoding. A value of 1 means UTF-8. A value of 2 means UTF-16le. A value of 3 means UTF-16be.
60	4	The "user version" as read and set by the user_version pragma.
64	4	True (non-zero) for incremental-vacuum mode. False (zero) otherwise.
68	4	The "Application ID" set by PRAGMA application_id.
72	20	Reserved for expansion. Must be zero.
92	4	The version-valid-for number.
96	4	SQLITE_VERSION_NUMBER
*/

/*
 * Read file that not exist
 * Read file that have header less than 100
 * Read file with normal first 16 byte
 * ... read another parameters
 *
 *
 * */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Mocks.h"
using namespace testing;

const SqliteHeader g_testHeader{"SQLite format 3",
                        512,
                        1,
                        1,
                        0,
                        64,
                        32,
                        32,
                        1,
                        30,
                        20,
                        5,
                    //    40	4	The schema cookie.
                        1,
                        32,
                        0,
                        1,
                        2,
                        0,
                        33,
                    //    72	20	Reserved for expansion. Must be zero.
                        3,
                        22};


void DysplayHeaderStructure(IGui* gui, IDbReader* dbReader)
{
    if (gui == nullptr || dbReader->IsEmpty() || !dbReader->CheckHeader())
    {
        throw std::exception();
    }
    std::vector<std::string> messagesForOutput;
    messagesForOutput.push_back("Type - " + dbReader->GetHeaderString());
    messagesForOutput.push_back("DB page size - " + std::to_string(dbReader->GetPageSize()));
    messagesForOutput.push_back("File format write version - " + std::to_string(dbReader->GetFormatWriteVersion()));
    messagesForOutput.push_back("File format read version - " + std::to_string(dbReader->GetFormatReadVersion()));
    messagesForOutput.push_back("Unused bytes - " + std::to_string(dbReader->GetUnsedBytes()));
    messagesForOutput.push_back("Maximum embedded payload fraction - " + std::to_string(dbReader->GetMaximumEmbeddedPayloadFraction()));
    messagesForOutput.push_back("Minimum embedded payload fraction - " + std::to_string(dbReader->GetMinimumEmbeddedPayloadFraction()));
    messagesForOutput.push_back("Leaf payload fraction - " + std::to_string(dbReader->GetLeafPayloadFraction()));
    messagesForOutput.push_back("File change counter - " + std::to_string(dbReader->GetFileChangeCounter()));
    messagesForOutput.push_back("Size of the database file in pages - " + std::to_string(dbReader->GetPageCount()));
    messagesForOutput.push_back("Page number of the first freelist trunk page - " + std::to_string(dbReader->GetFirstFreelistPage()));
    messagesForOutput.push_back("Total number of freelist pages - " + std::to_string(dbReader->GetFreelistPageCount()));
    messagesForOutput.push_back("The schema format number - " + std::to_string(dbReader->GetSchemaFormat()));
    messagesForOutput.push_back("Default page cache size - " + std::to_string(dbReader->GetDefaultPageCacheSize()));
    messagesForOutput.push_back("The page number of the largest root b-tree page - " + std::to_string(dbReader->GetNumberOfLargestRootPage()));
    messagesForOutput.push_back("The database text encoding - " + std::to_string(dbReader->GetDatabaseTextEncoding()));
    messagesForOutput.push_back("The \"user version\" - " + std::to_string(dbReader->GetUserVersion()));
    messagesForOutput.push_back("The vacuum mode - " + std::to_string(dbReader->GetIncrementalVacuumMode()));
    messagesForOutput.push_back("The \"Application ID\" - " + std::to_string(dbReader->GetApplicationId()));
    messagesForOutput.push_back("The version-valid-for number - " + std::to_string(dbReader->GetVersionValidNumber()));
    messagesForOutput.push_back("The SQLITE_VERSION_NUMBER - " + std::to_string(dbReader->GetSqliteVersionNumber()));

    gui->DisplayHeader(messagesForOutput);
}

TEST(SqliteHeaderReader, NoGui)
{
    DbReaderMock dbReader;
    ASSERT_THROW(DysplayHeaderStructure(nullptr, &dbReader), std::exception);
}

TEST(SqliteHeaderReader, EmptyReader)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(true));
    ASSERT_THROW(DysplayHeaderStructure(&gui, &dbReader), std::exception);
}

TEST(SqliteHeaderReader, ReadFileLessThan100Byte)
{
    DbReaderMock dbReader;
    EXPECT_CALL(dbReader, ReadFilePath("somePath")).WillOnce(Return(true));
    dbReader.ReadFilePath("somePath");
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(false));
    ASSERT_THROW(DysplayHeaderStructure(&gui, &dbReader), std::exception);
}

TEST(SqliteHeaderReader, ReadHeadString)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetHeaderString()).WillOnce(Return(g_testHeader.head));

    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadDbPageSize)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetPageSize()).WillOnce(Return(g_testHeader.pageSize));

    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadFileWriteFormatVersion)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFormatWriteVersion()).WillOnce(Return(g_testHeader.fileFormatWriteVersion));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadFileReadFormatVersion)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFormatReadVersion()).WillOnce(Return(g_testHeader.fileFormatReadVersion));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadUnusedBytes)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFormatReadVersion()).WillOnce(Return(g_testHeader.bytesOfUnused));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadMaximumEmbeddedPayloadFraction)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetMaximumEmbeddedPayloadFraction()).WillOnce(Return(g_testHeader.maximumEmbeddedPayloadFraction));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadMinimumEmbeddedPayloadFraction)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetMinimumEmbeddedPayloadFraction()).WillOnce(Return(g_testHeader.minimumEmbeddedPayloadFraction));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadLeafPayloadFraction)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetLeafPayloadFraction()).WillOnce(Return(g_testHeader.leafPayloadFraction));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadFileChangeCounter)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFileChangeCounter()).WillOnce(Return(g_testHeader.fileChangeCounter));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadPageCount)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetPageCount()).WillOnce(Return(g_testHeader.pageCount));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadFirstFreelistPage)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFirstFreelistPage()).WillOnce(Return(g_testHeader.firstFreelistPage));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadFreelistPageCount)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetFreelistPageCount()).WillOnce(Return(g_testHeader.freelistPageCount));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadSchemaFormat)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetSchemaFormat()).WillOnce(Return(g_testHeader.schemaFormat));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadDefaultPageCacheSize)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetDefaultPageCacheSize()).WillOnce(Return(g_testHeader.defaultPageCacheSize));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadNumberOfLargestRootPage)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetNumberOfLargestRootPage()).WillOnce(Return(g_testHeader.numberOfLargestRootPage));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadDatabaseTextEncoding)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetDatabaseTextEncoding()).WillOnce(Return(g_testHeader.databaseTextEncoding));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadUserVersion)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetUserVersion()).WillOnce(Return(g_testHeader.userVersion));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadIncrementalVacuumMode)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetIncrementalVacuumMode()).WillOnce(Return(g_testHeader.incrementalVacuumMode));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadApplicationId)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetApplicationId()).WillOnce(Return(g_testHeader.applicationId));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadVersionValidNumber)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetVersionValidNumber()).WillOnce(Return(g_testHeader.versionValidNumber));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, ReadSqliteVersionNumber)
{
    DbReaderMock dbReader;
    GuiMock gui;

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetSqliteVersionNumber()).WillOnce(Return(g_testHeader.sqliteVersionNumber));
    EXPECT_CALL(gui, DisplayHeader(_)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}

TEST(SqliteHeaderReader, SummaryTest)
{
    DbReaderMock dbReader;
    EXPECT_CALL(dbReader, ReadFilePath("somePath")).WillOnce(Return(true));
    dbReader.ReadFilePath("somePath");
    GuiMock gui;

    std::vector<std::string> expectedData{
                std::string("Type - SQLite format 3"),
                std::string("DB page size - 512"),
                std::string("File format write version - 1"),
                std::string("File format read version - 1"),
                std::string("Unused bytes - 0"),
                std::string("Maximum embedded payload fraction - 64"),
                std::string("Minimum embedded payload fraction - 32"),
                std::string("Leaf payload fraction - 32"),
                std::string("File change counter - 1"),
                std::string("Size of the database file in pages - 30"),
                std::string("Page number of the first freelist trunk page - 20"),
                std::string("Total number of freelist pages - 5"),
                std::string("The schema format number - 1"),
                std::string("Default page cache size - 32"),
                std::string("The page number of the largest root b-tree page - 0"),
                std::string("The database text encoding - 1"),
                std::string("The \"user version\" - 2"),
                std::string("The vacuum mode - 0"),
                std::string("The \"Application ID\" - 33"),
                std::string("The version-valid-for number - 3"),
                std::string("The SQLITE_VERSION_NUMBER - 22")};

    EXPECT_CALL(dbReader, IsEmpty()).WillOnce(Return(false));
    EXPECT_CALL(dbReader, CheckHeader()).WillOnce(Return(true));
    EXPECT_CALL(dbReader, GetHeaderString()).WillOnce(Return(g_testHeader.head));
    EXPECT_CALL(dbReader, GetPageSize()).WillOnce(Return(g_testHeader.pageSize));
    EXPECT_CALL(dbReader, GetFormatWriteVersion()).WillOnce(Return(g_testHeader.fileFormatWriteVersion));
    EXPECT_CALL(dbReader, GetFormatReadVersion()).WillOnce(Return(g_testHeader.fileFormatReadVersion));
    EXPECT_CALL(dbReader, GetFormatReadVersion()).WillOnce(Return(g_testHeader.bytesOfUnused));
    EXPECT_CALL(dbReader, GetMaximumEmbeddedPayloadFraction()).WillOnce(Return(g_testHeader.maximumEmbeddedPayloadFraction));
    EXPECT_CALL(dbReader, GetMinimumEmbeddedPayloadFraction()).WillOnce(Return(g_testHeader.minimumEmbeddedPayloadFraction));
    EXPECT_CALL(dbReader, GetLeafPayloadFraction()).WillOnce(Return(g_testHeader.leafPayloadFraction));
    EXPECT_CALL(dbReader, GetFileChangeCounter()).WillOnce(Return(g_testHeader.fileChangeCounter));
    EXPECT_CALL(dbReader, GetPageCount()).WillOnce(Return(g_testHeader.pageCount));
    EXPECT_CALL(dbReader, GetFirstFreelistPage()).WillOnce(Return(g_testHeader.firstFreelistPage));
    EXPECT_CALL(dbReader, GetFreelistPageCount()).WillOnce(Return(g_testHeader.freelistPageCount));
    EXPECT_CALL(dbReader, GetSchemaFormat()).WillOnce(Return(g_testHeader.schemaFormat));
    EXPECT_CALL(dbReader, GetDefaultPageCacheSize()).WillOnce(Return(g_testHeader.defaultPageCacheSize));
    EXPECT_CALL(dbReader, GetNumberOfLargestRootPage()).WillOnce(Return(g_testHeader.numberOfLargestRootPage));
    EXPECT_CALL(dbReader, GetDatabaseTextEncoding()).WillOnce(Return(g_testHeader.databaseTextEncoding));
    EXPECT_CALL(dbReader, GetUserVersion()).WillOnce(Return(g_testHeader.userVersion));
    EXPECT_CALL(dbReader, GetIncrementalVacuumMode()).WillOnce(Return(g_testHeader.incrementalVacuumMode));
    EXPECT_CALL(dbReader, GetApplicationId()).WillOnce(Return(g_testHeader.applicationId));
    EXPECT_CALL(dbReader, GetVersionValidNumber()).WillOnce(Return(g_testHeader.versionValidNumber));
    EXPECT_CALL(dbReader, GetSqliteVersionNumber()).WillOnce(Return(g_testHeader.sqliteVersionNumber));

    EXPECT_CALL(gui, DisplayHeader(expectedData)).Times(1);

    ASSERT_NO_THROW(DysplayHeaderStructure(&gui, &dbReader));
}
