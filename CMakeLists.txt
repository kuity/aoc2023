cmake_minimum_required(VERSION 3.10)

project(AoC2023 VERSION 1.0)

# Set the required version of C++ 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/Users/lingzhang.jiang/projects/tools/cpp/eigen)
file(GLOB LIB_SOURCES "lib/*.cpp")

foreach(qnum RANGE 1 25)
    add_executable("d${qnum}" ${LIB_SOURCES} "src/d${qnum}.cpp")
endforeach()

