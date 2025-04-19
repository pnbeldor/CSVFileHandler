#ifndef MOCK_CSV_PARSER_HPP
#define MOCK_CSV_PARSER_HPP

#include "ICSVParser.h"

#include <gmock/gmock.h>

#include <cstddef>

class MockCSVParser : public ICSVParser
{
public:
    MOCK_METHOD(std::vector<std::string>, GetHeaders, (), (const, override));
    MOCK_METHOD(std::vector<std::string>, GetRow, (size_t rowIndex), (const, override));
    MOCK_METHOD(std::vector<std::string>, GetColumn, (const std::string& column), (const, override));
    MOCK_METHOD(std::string, GetCell, (const std::string&columnName, size_t rowIndex), (const, override));
    MOCK_METHOD(size_t, GetRowCount, (), (const, override));
    MOCK_METHOD(size_t, GetColumnCount, (), (const, override));
    MOCK_METHOD(bool, IsDocumentLoaded, (), (override));
};
#endif // MOCK_CSV_PARSER_HPP
