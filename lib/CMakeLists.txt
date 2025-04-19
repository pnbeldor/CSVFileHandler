
add_library(CSVFileHandler STATIC)
target_sources(CSVFileHandler PRIVATE src/CSVFileHandler.cpp src/RapidCSVParser.cpp)

target_include_directories(CSVFileHandler PUBLIC "include")
target_link_libraries(CSVFileHandler PUBLIC rapidcsv)

enable_testing()
add_subdirectory(tests)
