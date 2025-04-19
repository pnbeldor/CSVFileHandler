
if(BUILD_TESTING)
    add_executable(CSVFileHandlerTest RapidCSVParserTests.cpp CSVFileHandlerTests.cpp)

    target_link_libraries(CSVFileHandlerTest PUBLIC gtest gmock gtest_main)
    target_link_libraries(CSVFileHandlerTest PUBLIC CSVFileHandler)
    target_include_directories(CSVFileHandlerTest
                        PUBLIC "${PROJECT_BINARY_DIR}"
                        PUBLIC "${PROJECT_SOURCE_DIR}/lib/include"
                        )

    add_test(NAME CSVFileHandlerTest COMMAND CSVFileHandlerTest)
    include(GoogleTest)

    gtest_discover_tests(CSVFileHandlerTest)
endif()
