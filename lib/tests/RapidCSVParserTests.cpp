 //--------------------------------------------------------------------------------
 //RapidCSVParserTests.cpp
 //
 //--------------------------------------------------------------------------------
 
#include "RapidCSVParser.h"

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Helper function to create a test CSV file
void CreateTestCSV(const std::string& filename) {
    std::ofstream outfile(filename);
    outfile << "Filename,Type,Date,Time,(Center) Coordinate,DPI\n";
    outfile << "IMG1.png,png,2023-01-01,12:00:00,40° 26' 46\" N 79° 56' 55\" W,100\n";
    outfile << "IMG2.jpg,jpg,2023-01-02,13:00:00,40.446 -79.9486,200\n";
    outfile << "IMG3.jpg,jpg,2023-01-03,14:00:00,40° 26' N 79° 56' W,300\n";
    outfile << "IMG4.tif,tiff,2023-01-04,15:00:00,40.5 -80.0,400\n";
    outfile << "IMG5.png,png,2023-01-05,12:00:00,40° 26' 46\" N 79° 56' 55\" W,800\n";
    outfile << "IMG6.tif,tiff,2023-01-06,13:00:00,50.446 -80.9486,200\n";
    outfile << "IMG7.jpg,jpg,2023-01-07,14:00:00,40° 26' N 79° 56' W,300\n";
    outfile << "IMG8.jpg,jpg,2023-01-08,15:00:00,40.5 -81.0,400\n";
    outfile.close();
}

class RapidCSVParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFilename = "test_csv.csv";
        CreateTestCSV(testFilename);
        parser = std::make_unique<RapidCSVParser>(testFilename);
    }

    void TearDown() override {
        fs::remove(testFilename);
    }

    std::string testFilename;
    std::unique_ptr<ICSVParser> parser;
};

TEST_F(RapidCSVParserTest, ConstructorAndBasicFunctions) {
    EXPECT_TRUE(parser->IsDocumentLoaded());
    EXPECT_TRUE(parser->GetRowCount() > 0);
    EXPECT_TRUE(parser->GetColumnCount() > 0);
    
    auto headers = parser->GetHeaders();
    EXPECT_FALSE(headers.empty());
    EXPECT_EQ(headers.size(), parser->GetColumnCount());
    
    EXPECT_GT(parser->GetRowCount(), 0);
}

TEST_F(RapidCSVParserTest, GetRowAndColumn) {
    auto row = parser->GetRow(0);
    EXPECT_FALSE(row.empty());
    EXPECT_EQ(row.size(), parser->GetColumnCount());
    
    auto column = parser->GetColumn("Filename");
    EXPECT_FALSE(column.empty());
    EXPECT_EQ(column.size(), parser->GetRowCount());
    
    EXPECT_EQ(parser->GetCell("Filename", 0), "IMG1.png");
}

TEST_F(RapidCSVParserTest, InvalidRowHandling) {
    size_t invalidRow = parser->GetRowCount() + 1;
    EXPECT_THROW(parser->GetRow(invalidRow), std::exception);
    EXPECT_THROW(parser->GetCell("Filename", invalidRow), std::exception);
}

TEST_F(RapidCSVParserTest, EmptyFileHandling) {
    std::string emptyFile = "empty.csv";
    std::ofstream outfile(emptyFile);
    outfile.close();
    
    RapidCSVParser emptyParser(emptyFile);
    EXPECT_EQ(emptyParser.GetRowCount(), 0);
    EXPECT_EQ(emptyParser.GetColumnCount(), 0);
    EXPECT_TRUE(emptyParser.IsDocumentLoaded());
    
    fs::remove(emptyFile);
}

TEST_F(RapidCSVParserTest, NonExistantFileHandling) {
    std::string emptyFile = "empty.csv";
    
    std::unique_ptr<RapidCSVParser> emptyParser;
    EXPECT_THROW(emptyParser = std::make_unique<RapidCSVParser>(emptyFile), std::exception);
}
