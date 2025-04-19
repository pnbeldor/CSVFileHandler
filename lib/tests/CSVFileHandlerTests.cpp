 //--------------------------------------------------------------------------------
 //CSVFileHandlerTests.cpp
 //
 //--------------------------------------------------------------------------------
 
#include "CSVFileHandler.h"

#include "gmock/gmock.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "ICSVParser.h"
#include "MockCSVParser.h"

namespace fs = std::filesystem;

using ::testing::_;

/*
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
*/

class CSVFileHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // testFilename = "test_csv.csv";
        // CreateTestCSV(testFilename);
        parser = std::make_unique<MockCSVParser>();
        parserRawPtr  = (MockCSVParser*)(parser.get());
        handler = new CSVFileHandler(parser);
    }

    void TearDown() override {
        delete handler;
        parserRawPtr = nullptr;
        //fs::remove(testFilename);
    }

    //std::string testFilename;
    std::unique_ptr<ICSVParser> parser;
    MockCSVParser* parserRawPtr;
    CSVFileHandler* handler;
};

TEST_F(CSVFileHandlerTest, Constructor) {
    EXPECT_EQ(parser, nullptr);
    //std::unique_ptr<MockFile> mockFile(new MockFile);
    //std::unique_ptr<FileSystemInterface> mockFile(new MockFile);
}

TEST_F(CSVFileHandlerTest, FindRowsAsMap) {

    //MockCSVParser* parserPtr = new MockCSVParser();
    //MockCSVParser& parserRef = *(handler->parser);
    //EXPECT_CALL(parserRef, IsDocumentLoaded()).WillOnce(testing::Return (true));
    //SearchCriteria criteria = {std::make_pair("Filename", std::make_pair(ComparisonOp::EQUAL, "IMG2.jpg"))};
    //auto results = handler->FindRowsAsMap(criteria);
    
    // EXPECT_EQ(results.size(), 1);
    // EXPECT_EQ(results[0]["Filename"], "IMG2.jpg");
    // EXPECT_EQ(results[0]["DPI"], "200");
    //delete parserPtr;
    //parserPtr = nullptr;
}


TEST_F(CSVFileHandlerTest, ParseCoordinate) {
    GeoPoint point;
    std::string coordString = "51.176094, -113.923462";

    EXPECT_TRUE(handler->ParseCoordinate(coordString, point));
    EXPECT_EQ(point.latitude, 51.176094);
    EXPECT_EQ(point.longitude, -113.923462);
}

TEST_F(CSVFileHandlerTest, IsPointInPolygon) {
    std::vector<GeoPoint> polygon = {
        {40.0, -80.0},
        {40.0, -79.0},
        {41.0, -79.0},
        {41.0, -80.0}
    };
    
    GeoPoint insidePoint = {40.5, -79.5};
    EXPECT_TRUE(handler->IsPointInPolygon(insidePoint, polygon));
    
    GeoPoint outsidePoint = {39.5, -79.5};
    EXPECT_FALSE(handler->IsPointInPolygon(outsidePoint, polygon));
    
    GeoPoint edgePoint = {40.0, -79.5};
    EXPECT_TRUE(handler->IsPointInPolygon(edgePoint, polygon));
}

TEST_F(CSVFileHandlerTest, FindFilesInPolygon) {
    std::vector<GeoPoint> polygon = {
        {40.4, -80.0},
        {40.4, -79.9},
        {40.5, -79.9},
        {40.5, -80.0}
    };
    
    MockCSVParser parser;
    EXPECT_CALL(parser, IsDocumentLoaded()).Times(1).WillOnce(testing::Return (true));

    auto results = handler->FindFilesInPolygonAsMap(polygon, "(Center) Coordinate");
    
    EXPECT_EQ(results.size(), 6);
    
    // Check that the results contain the expected filenames
    std::set<std::string> foundFiles;
    for (const auto& row : results) {
        foundFiles.insert(row.at("Filename"));
    }
    
    EXPECT_TRUE(foundFiles.count("IMG2.jpg") > 0);
    EXPECT_TRUE(foundFiles.count("IMG4.tif") > 0);
    EXPECT_TRUE(foundFiles.count("IMG1.png") > 0);
    EXPECT_TRUE(foundFiles.count("IMG5.png") > 0);
    EXPECT_FALSE(foundFiles.count("IMG6.tif") > 0);
    EXPECT_TRUE(foundFiles.count("IMG7.jpg") > 0);
    EXPECT_FALSE(foundFiles.count("IMG8.jpg") > 0);
}
