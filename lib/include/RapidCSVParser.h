 //--------------------------------------------------------------------------------
 //RapidCSVParser.h
 //
 //--------------------------------------------------------------------------------
 
#ifndef RAPID_CSV_PARSER_H
#define RAPID_CSV_PARSER_H

#include "ICSVParser.h"

#include "rapidcsv.h"

class RapidCSVParser : public ICSVParser
{
public:
    RapidCSVParser(const std::string& filename);
    virtual ~RapidCSVParser();
    std::vector<std::string> GetHeaders() const override;
    std::vector<std::string> GetRow(size_t rowIndex) const override;
    std::vector<std::string>  GetColumn(const std::string& columnName) const override;
    std::string GetCell(const std::string& columnName_, size_t rowIndex) const override;
    size_t GetRowCount() const override;
    size_t GetColumnCount() const override;
    bool IsDocumentLoaded() override;

private:
    void Load(const std::string& filename);

private:
    rapidcsv::Document document;
    bool documentLoaded;

};

#endif // RAPID_CSV_PARSER_H
