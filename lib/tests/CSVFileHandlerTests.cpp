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
class CSVFileHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        handler = new CSVFileHandler(std::make_unique<MockCSVParser>());
    }

    void TearDown() override {
        delete handler;
    }

    CSVFileHandler* handler;
};

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
