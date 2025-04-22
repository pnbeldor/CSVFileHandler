#include "CSVFileHandler.h"

#include "RapidCSVParser.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
#include <string>
#include <utility>


void printHelp()
{
  std::cout << "Usage: ./CSVSearch <filename> [options]\n"
            << "Options:\n"
            << "  --search <column1>=<value1> [<column2>=<value2> ...]  Search "
               "by exact matches\n"
            << "  --polygon   <\"lat1,lon1\"> <\"lat2, lon2\"> ...  Search "
               "within geographic polygon\n"
            << "  --help                                                 Show "
               "this help message\n";
}

SearchCriteria parseSearchArguments(int argc, char* argv[])
{
    SearchCriteria criteria;
    int i = 0; // Starting index
    
    while (++i < argc) {
        std::string arg = argv[i];
        
        // Find the operator position
        size_t op_pos = std::string::npos;
        ComparisonOp op = ComparisonOp::EQUAL;
        
        // Check for multi-character operators first
        if (arg.find(">=") != std::string::npos) {
            op_pos = arg.find(">=");
            op = ComparisonOp::GREATER_EQUAL;
        }
        else if (arg.find("<=") != std::string::npos) {
            op_pos = arg.find("<=");
            op = ComparisonOp::LESS_EQUAL;
        }
        // Then check single-character operators
        else if (arg.find('>') != std::string::npos) {
            op_pos = arg.find('>');
            op = ComparisonOp::GREATER;
        }
        else if (arg.find('<') != std::string::npos) {
            op_pos = arg.find('<');
            op = ComparisonOp::LESS;
        }
        else if (arg.find('=') != std::string::npos) {
            op_pos = arg.find('=');
            op = ComparisonOp::EQUAL;
        }
        
        if (op_pos != std::string::npos) {
            std::string col = arg.substr(0, op_pos);
            std::string val = arg.substr(op_pos + (op == ComparisonOp::GREATER_EQUAL || op == ComparisonOp::LESS_EQUAL ? 2 : 1));
            
            criteria.emplace_back(col, std::make_pair(op, val));
        }
    }
    
    return criteria;
}

std::vector<GeoPoint> ParsePolygonArguments(CSVFileHandler& handler, int argc, char*argv[])
{
    std::vector<GeoPoint> polygon;
    GeoPoint point;
    int idx = 2;

    while (++idx < argc) {
        GeoPoint p;
        handler.ParseCoordinate(std::string(argv[idx]), p);
        polygon.push_back(p);
    }

    auto results = handler.FindFilesInPolygonAsMap(polygon);

    for (const auto &result : results) {
        std::cout << result.at("Filename") << std::endl;
    }

    return polygon;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || std::string(argv[1]) == "--help") {
        printHelp();
        return 0;
    }

    // load the file to the buffer
    //std::unique_ptr<ICSVParser> parser = std::make_unique<RapidCSVParser>(argv[1]);
    CSVFileHandler handler(std::make_unique<RapidCSVParser>(argv[1]));

    std::string arg = argv[2];
 
    if (arg == "--search")
    {
        auto searCriteria = parseSearchArguments(argc, argv);

        auto results = handler.FindRowsAsMap(searCriteria);

        for (const auto &result : results) {
            std::cout << result.at("Filename") << std::endl;
        }
    }
    else if(arg == "--polygon")
    {
        auto polygon = ParsePolygonArguments(handler, argc, argv);

        auto results = handler.FindFilesInPolygonAsMap(polygon);

        for (const auto &result : results) {
        std::cout << result.at("Filename") << std::endl;
        }
    }
    else
    {
        std::cerr << "Invalid arguments...\n";
        printHelp();
    }

    return 0;
}
