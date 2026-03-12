cmake_minimum_required(VERSION 3.15)
project(flight_tracker VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SQLite3
find_package(SQLite3 REQUIRED)

add_executable(flight_tracker
    src/main.cpp
    src/database.cpp
    src/flight.cpp
    src/menu.cpp
)

target_include_directories(flight_tracker PRIVATE include)

target_link_libraries(flight_tracker PRIVATE SQLite::SQLite3)
