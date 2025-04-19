
# CSV File Handler


the application use CMake

to build the application, the following tools are needed:
    * git
    * c++17
    * CMake 3.20 or Higher
    * RapidCSV library (automatically downloaded during build)


## Application structure
```
csv_search/
├── CMakeLists.txt
├── lib/
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── CSVFileHandler.h
│   │   ├── CSVParser.h
│   │   └── RapidCSVParser.h
│   ├── src/
│   │   ├── CSVFileHandler.cpp
│   │   └── RapidCSVParser.cpp
│   └── tests/
│       ├── CMakeLists.txt
│       └── CSVFileHandlerTests.cpp
├── main.cpp
└── README.md
```


to build the application:
    *mkdir build && cd build/
    * cmake -S ./.. -B . -G "Unix MakeFiles"
    * make -j

## Basic syntax
    ./CSVFileHandler <filename> [options]

## Get Help
    ./CSVFileHandler --help

## Basic Search
Search for rows where specific columns match given values
    ./CSVFileHandler imageFile.csv --search "Type=png"

## Multiple criteria will be compiled with AND logic

## Determine id a point is inside of a polygon
    ./CSVFileHandler path/to/file.csv --polygon <"lat1, long1"> <"lat2,long2"> .....

The program provide support both format for coordinates:
    Decimal degrees: "51.176094, -113.923462"
    DMS (Degree/Minutes/Seconds): "36° 00'N, 138° 00' E"

# Example
```
A polygon representation of Calgary:
/CSVFileHandler ImageFile.csv  --polygon \
"51.176094, -113.923462" \
"50.911922, -113.911103" \
"50.898898, -114.121216" \
"51.009630, -114.167908" \
"51.028635, -114.233826" \
"51.148535, -114.226960" \
"51.174372, -114.088257" \
"51.176094, -113.923462" \
```
## Performance Consideration
Consider optimize with the following performance considerations:
    * Pre-converting DMS to decimal during CSV loading
    * Adding a cache for parsed coordinates
    * Using parallel processing for the search
    * Better handling of the exceptions
    * Adding unit tests

