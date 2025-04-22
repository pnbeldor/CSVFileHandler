
cmake_minimum_required(VERSION 3.20)
project(CSVFileManager)

set(CMAKE_CXX_STANDARD 17)

# Download RapidCSV
include(FetchContent)
FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv
    GIT_TAG        v8.85
)
FetchContent_MakeAvailable(rapidcsv)

 include(FetchContent)
 FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)

FetchContent_MakeAvailable(googletest)

 include(FetchContent)
 FetchContent_Declare(
    fast-cpp-csv-parser
    GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(fast-cpp-csv-parser)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(lib)
target_link_libraries(${PROJECT_NAME} PRIVATE rapidcsv)
target_link_libraries(${PROJECT_NAME} PUBLIC CSVFileHandler)
include(CTest)
enable_testing()
