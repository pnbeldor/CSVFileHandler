 //--------------------------------------------------------------------------------
 // RapidCSVParser.cpp
 //
 //--------------------------------------------------------------------------------
 
#include "RapidCSVParser.h"

RapidCSVParser::RapidCSVParser(const std::string& filename) : documentLoaded(false)
{
    Load(filename);
}

RapidCSVParser::~RapidCSVParser()
{
}

std::vector<std::string> RapidCSVParser::GetHeaders() const
{
    return document.GetColumnNames();
}

std::vector<std::string> RapidCSVParser::GetRow(size_t rowIndex) const
{
   return document.GetRow<std::string>(rowIndex);
}

std::vector<std::string>  RapidCSVParser::GetColumn(const std::string& columnName) const
{
   return document.GetColumn<std::string>(columnName);
}

std::string RapidCSVParser::GetCell(const std::string& columnName_, size_t rowIndex) const
{
    return document.GetCell<std::string>(columnName_, rowIndex);
}

size_t RapidCSVParser::GetRowCount() const
{
   return document.GetRowCount();
}

size_t RapidCSVParser::GetColumnCount() const
{
   return document.GetColumnCount();
}

bool RapidCSVParser::IsDocumentLoaded()
{
    return documentLoaded;
}

void RapidCSVParser::Load(const std::string& filename)
{
    try {
        document.Load(filename);
        documentLoaded = true;
    }
    catch (const std::exception* e) {
        std::cerr << "Error loading the file:  " << std::endl;
        documentLoaded = false;
    }
}
