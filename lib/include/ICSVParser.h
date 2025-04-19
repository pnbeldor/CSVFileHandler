 //--------------------------------------------------------------------------------
 //ICSVParser.h
 //
 //--------------------------------------------------------------------------------
 
#ifndef I_CSV_PARSER_HPP
#define I_CSV_PARSER_HPP

#include <string>
#include <cstddef>
#include <vector>

class ICSVParser {
public:
    virtual ~ICSVParser() = default;

    virtual std::vector<std::string> GetHeaders() const = 0;
    virtual std::vector<std::string> GetRow(size_t rowIndex) const = 0;
    virtual std::vector<std::string>  GetColumn(const std::string& columnName) const = 0;
    virtual std::string GetCell(const std::string& columnName, size_t rowIndex) const = 0;
    virtual size_t GetRowCount() const = 0;
    virtual size_t GetColumnCount() const = 0;
    virtual bool IsDocumentLoaded() = 0;
};

#endif // I_CSV_PARSER_H
