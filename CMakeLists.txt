cmake_minimum_required(VERSION 3.27)
project(ProgramsWithOR)
# Set the path to the OR-Tools library (change the path accordingly)
set(ORTOOLS_PATH "/Users/ondrejpaatz/source/or-tools")

# Include OR-Tools headers
include_directories(${ORTOOLS_PATH}/include)

# Link OR-Tools library
link_directories(${ORTOOLS_PATH}/lib)
link_libraries(libortools.dylib)  # Adjust the library name accordingly

set(CMAKE_CXX_STANDARD 20)

add_executable(ProgramsWithOR main.cpp
        classes.cpp
        classes.h)
add_executable(SplitRopro splitRopro.cpp
        classes.cpp
        classes.h)
