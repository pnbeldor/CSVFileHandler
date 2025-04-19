 //--------------------------------------------------------------------------------
 //CSVFileHandler.h
 //
 //--------------------------------------------------------------------------------
 
#ifndef CSV_FILE_HANDLER_HPP
#define CSV_FILE_HANDLER_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <memory>
#include <map>

#include "ICSVParser.h"

 // Geo-spatial functions
struct GeoPoint {
    double latitude;
    double longitude;
};

enum class ComparisonOp {
    EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL
};

using SearchCriterion = std::pair<std::string, std::pair<ComparisonOp, std::string>>;
using SearchCriteria = std::vector<SearchCriterion>;

class CSVFileHandler
{
public:
    CSVFileHandler(std::unique_ptr<ICSVParser>& csvParser);
    virtual ~CSVFileHandler();

    // Functions prototypes
    std::vector<std::map<std::string, std::string>> FindRowsAsMap(const SearchCriteria& criteria);
    std::vector<std::map<std::string, std::string>> FindFilesInPolygonAsMap(
                const std::vector<GeoPoint>& polygon,
                const std::string& coordColumn = "(Center) Coordinate");

    bool IsPointInPolygon(const GeoPoint& point, const std::vector<GeoPoint>& polygon);
    bool ParseCoordinate(const std::string& coordString, GeoPoint& point);

    void Print() const;

private:
    void Load(const std::string& filename_);
    bool ValidateColumn(const std::string& columnName);
    bool ParseFullDMS(const std::string& coordString, GeoPoint& point);
    bool ParseDMSWithoutSecond(const std::string& coordString, GeoPoint& point);
    std::vector<size_t> FindRows(const SearchCriteria& searchCriteria);
    std::vector<size_t> FindFilesInPolygon(const std::vector<GeoPoint>& polygon,
        const std::string& coordColumn);
    bool ParseDecimal(const std::string& coordString, GeoPoint& point);
    double DMSToDecimal(const std::string& degrees, const std::string& minutes, 
                       const std::string& seconds, const std::string& direction);
    double DMSToDecimal(const std::string& degrees, const std::string& minutes,
                       const std::string& direction);

public:
    std::unique_ptr<ICSVParser> parser;
};

#endif // CSV_FILE_HANDLER_HPP
