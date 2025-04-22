 //--------------------------------------------------------------------------------
 //CSVFileHandler.cpp
 //
 //--------------------------------------------------------------------------------
 
#include "CSVFileHandler.h"
#include "ICSVParser.h"

#include <cstddef>
#include <iostream>
#include <cmath>
#include <regex>
#include <sstream>

//Constructor: Load the file

CSVFileHandler::CSVFileHandler(std::unique_ptr<ICSVParser> csvParser) : parser(std::move(csvParser))
{
}

//Destructor
CSVFileHandler::~CSVFileHandler() {}

//Display the content of the files -- Use for debugging
void CSVFileHandler::Print() const
{
    std::cout << "Display data file for csv photo file\n";
    std::vector<std::string> headers = parser->GetHeaders();

    for (std::string val : headers)
    {
        std::cout << val << "   ";
    }

    for (int i = 0; i < parser->GetRowCount(); i++)
    {
        for (int j = 0; j < parser->GetColumnCount(); j++)
        {
            std::cout << parser->GetCell(headers[j], i) << "      ";
        }

        std::cout << "\n";
    }
}

//Return a list of the indexes of the rows that match the criteria
std::vector<size_t> CSVFileHandler::FindRows(const SearchCriteria& searchCriteria)
{
    std::vector<size_t> matchingRows;

    if (!parser->IsDocumentLoaded() || searchCriteria.empty())
        return matchingRows;

    // Search through all rows
    for (size_t rowIdx = 0; rowIdx < parser->GetRowCount(); ++rowIdx)
    {
        bool match = true;

        // Verify that all columns in the search criteria exist
        for (const auto& criterion : searchCriteria) {
            const auto& col = criterion.first;
            const auto& op = criterion.second.first;
            const auto& val = criterion.second.second;

            if (!ValidateColumn(col)) {
                continue;
            }

            const std::string& cellValue = parser->GetCell(col, rowIdx);

            //Convert to numbers if possible for numeric comparison
            bool numericComparison = false;
            double cellNum = 0;
            double valNum = 0;
            try {
                cellNum = std::stod(cellValue);
                valNum = std::stod(val);
                numericComparison = true;
            } catch (...) {
                // Not numeric, will use string comparison
            }

            bool conditionMet = false;

            if (numericComparison) {
                switch (op) {
                    case ComparisonOp::EQUAL:
                        conditionMet = cellNum == valNum;
                        break;
                    case ComparisonOp::LESS:
                        conditionMet = cellNum < valNum;
                        break;
                    case ComparisonOp::GREATER:
                        conditionMet = cellNum > valNum;
                        break;
                    case ComparisonOp::LESS_EQUAL:
                        conditionMet = cellNum <= valNum;
                        break;
                    case ComparisonOp::GREATER_EQUAL:
                        conditionMet = cellNum >= valNum;
                        break;
                    default:
                        break;
                }
            }
            else {
                //String comparison (only EQUAL makes sense for strings)
                switch (op) {
                    case ComparisonOp::EQUAL:
                        conditionMet = cellValue == val;
                        break;
                    default:
                        // For other operators with strings, maybe lof a warning
                        conditionMet = false;
                        break;
                }
            }

            if (!conditionMet) {
                match = false;
                break;
            }
        }

        if (match) {
            matchingRows.push_back(rowIdx);
        }
    }

    return matchingRows;
}

std::vector<std::map<std::string, std::string>> CSVFileHandler::FindRowsAsMap(const SearchCriteria& criteria)
{
    std::vector<std::map<std::string, std::string>> result;

    auto matchingRowIndices = FindRows(criteria);

    for (size_t rowIndex : matchingRowIndices)
    {
        std::map<std::string, std::string> rowData;
        for (const auto& colName : parser->GetHeaders())
        {
            rowData[colName] = parser->GetCell(colName, rowIndex);
        }
        result.push_back(rowData);
    }

    return result;
}

// Validate a column names by verifying it exists in the column header name
bool CSVFileHandler::ValidateColumn(const std::string& columnName)
{
    if (!parser->IsDocumentLoaded())
        return false;

    auto headers = parser->GetHeaders();
    for (const auto& header : headers)
    {
        if (header == columnName)
            return true;
    }

    return false;
}

// Find all the files within the given polygon
std::vector<size_t> CSVFileHandler::FindFilesInPolygon(
    const std::vector<GeoPoint>& polygon,
    const std::string& coordColumn)
{
    std::vector<size_t> results;

    //Verify polygon size
    if (!parser->IsDocumentLoaded() || polygon.size() < 3)
    {
        return results;
    }

    if (!ValidateColumn(coordColumn))
    {
        return results;
    }

    for (size_t rowIdx = 0; rowIdx < parser->GetRowCount(); ++rowIdx)
    {
        try {
            std::string coordStr = parser->GetCell(coordColumn, rowIdx);
            GeoPoint point;
            if (ParseCoordinate(coordStr, point))
            {
                if (IsPointInPolygon(point, polygon))
                {
                    results.push_back(rowIdx);
                }
            }
        } catch (...) {
            // Skip rows with invalid coordinates
            continue;
        }
    }

    return results;
}

//Files all the files with center coordinates inside the given polygon
std::vector<std::map<std::string, std::string>> CSVFileHandler::FindFilesInPolygonAsMap(
            const std::vector<GeoPoint>& polygon, const std::string& coordColumn)
{
   std::vector<size_t> matchingIndices = FindFilesInPolygon(polygon, coordColumn);

    std::vector<std::map<std::string, std::string>> results;

    for (size_t rowIndex : matchingIndices)
    {
        std::map<std::string, std::string> rowData;
        for (const auto& colName : parser->GetHeaders())
        {
            rowData[colName] = parser->GetCell(colName, rowIndex);
        }
        results.push_back(rowData);
    }

    return results;
}

// Check if a point is in a polygon using Ray-casting algorithm
bool CSVFileHandler::IsPointInPolygon(const GeoPoint& point, const std::vector<GeoPoint>& polygon)
{
    bool inside = false;
    size_t n = polygon.size();

    for (size_t i = 0, j = n - 1; i < n; j = i++)
    {
        const GeoPoint& p1 = polygon[i];
        const GeoPoint& p2 = polygon[j];

        // Check if point is on an edge
        if ((p1.longitude == point.longitude && p1.latitude == point.latitude) ||
            (p2.longitude == point.longitude && p2.latitude == point.latitude))
        {
            return true;
        }

        // Check if point is between p1 and p2 vertically
        if ((p1.latitude > point.latitude) != (p2.latitude > point.latitude))
        {
            // Calculate intersection
            double intersectLon = (p2.longitude - p1.longitude) * (point.latitude - p1.latitude) /
                                (p2.latitude - p1.latitude) + p1.longitude;

            // Check if point is on the edge
            if (std::abs(point.longitude - intersectLon) < 1e-9)
            {
                return true;
            }

            // Check if point is to the left of the intersection
            if (point.longitude < intersectLon)
            {
                inside = !inside;
            }
        }
    }

    return inside;
}

// Parse the coordinates - Expect long and lat to be in the same format
bool CSVFileHandler::ParseCoordinate(const std::string& coordString, GeoPoint& point)
{
    // First try to parse as decimal format
    if (ParseDecimal(coordString, point))
    {
        return true;
    }

    // If decimal fails, try DMS format with seconds
    if (ParseFullDMS(coordString, point))
    {
        return true;
    }

    // If decimal fails, try DMS format without seconds
    if (ParseDMSWithoutSecond(coordString, point))
    {
        return true;
    }

    return false;
}

//Parse the coordinates in decimal format
bool CSVFileHandler::ParseDecimal(const std::string& coordString, GeoPoint& point)
    {
    // Example formats:
    // "53.55014, -113.46871"
    // "53.55014,-113.46871"
    // "53.55014 -113.46871"
    std::regex decimal_regex(R"((-?\d+\.?\d*)[,\s]+(-?\d+\.?\d*))");
    std::smatch matches;

    if (!std::regex_search(coordString, matches, decimal_regex) || matches.size() != 3)
    {
        return false;
    }

    try {
        point.latitude = std::stod(matches[1].str());
        point.longitude = std::stod(matches[2].str());
        return true;
    } catch (...) {
        return false;
    }
}

// Full DMS is a string taht contains degree, minutes and seconds
bool CSVFileHandler::ParseFullDMS(const std::string& coordString_, GeoPoint& point)
{
    // Example formats:
    // "36° 00' 00"N, 138° 00' 00" E"
    // "36°00'00"N, 138°00'00"E"
    // "36°00'00" N 138°00' 00" E"

    // Try full DMS with seconds first
    std::regex dmsRegexWithSecond(R"((\d+)°\s*(\d+)'\s*(\d+)(?:\")?\s*([NnSs])[,\s]+(\d+)°\s*(\d+)'\s*(\d+)(?:\")?\s*([EeWw]))");
    std::smatch matches;

    if (std::regex_search(coordString_, matches, dmsRegexWithSecond) && matches.size() == 9)
    {
        // strod can throw out of range exception
        try {
            point.latitude = DMSToDecimal(matches[1].str(), matches[2].str(),
                                         matches[3].str(), matches[4].str());
            point.longitude = DMSToDecimal(matches[5].str(), matches[6].str(),
                                         matches[7].str(), matches[8].str());
            return true;
        } catch (...) {
            //If out of range exception, return false
            return false;
        }
    }

    return false;
}

// Parse DMS format without the seconds
bool CSVFileHandler::ParseDMSWithoutSecond(const std::string& coordString_, GeoPoint& point)
{
    // Example formats:
    // "36° 00'N, 138° 00' E"
    // "36°00'N, 138°00'E"
    // "36°00' N 138°00' E"

    // Try DMS without seconds
    std::regex dmsRegex(R"((\d+)°\s*(\d+)'\s*([NnSs])[,\s]+(\d+)°\s*(\d+)'\s*([EeWw]))");
    std::smatch matches;

    // Regular expression search should return 7 group including the whole string as the first group
    if (std::regex_search(coordString_, matches, dmsRegex) && matches.size() == 7)
    {
        // stod can throw out_of_range exception
        try {
            point.latitude = DMSToDecimal(matches[1].str(), matches[2].str(), matches[3].str());
            point.longitude = DMSToDecimal(matches[4].str(), matches[5].str(), matches[6].str());
            return true;
        } catch (...) {
            // If out of range exception, return false
            return false;
        }
    }

    return false;
}

// Convert coordinate to decimal - this function convert the full DMS
double CSVFileHandler::DMSToDecimal(const std::string& degrees, const std::string& minutes,
                              const std::string& seconds, const std::string& direction)
{
    double deg = std::stod(degrees);
    double min = std::stod(minutes);
    double sec = std::stod(seconds);

    double decimal = deg + (min / 60.0) + (sec / 3600.0);

    char dir = toupper(direction[0]);

    //If direction is S or W, the coordinate is negate
    if (dir == 'S' || dir == 'W')
    {
        decimal = -decimal;
    }

    return decimal;
}

// Convert DMS to decimal degrees
double CSVFileHandler::DMSToDecimal(const std::string& degrees, const std::string& minutes,
                              const std::string& direction)
{
    // Overload for DMS without seconds
    double deg = std::stod(degrees);
    double min = std::stod(minutes);

    double decimal = deg + (min / 60.0);

    char dir = toupper(direction[0]);

    //If direction is S or W, the coordinate is negate
    if (dir == 'S' || dir == 'W')
    {
        decimal = -decimal;
    }

    return decimal;
}

/*
calgary polygon coordinates
"51.176094, -113.923462" \
"50.911922, -113.911103" \
"50.898898, -114.121216" \
"51.009630, -114.167908" \
"51.028635, -114.233826" \
"51.148535, -114.226960" \
"51.174372, -114.088257" \
"51.176094, -113.923462" 
*/