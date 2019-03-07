cmake_minimum_required (VERSION 2.8)
project (PANDARAY)

# set c++ version
set (CMAKE_CXX_STANDARD 14)

# header files
include_directories(src/*)

# renderer
# add_executable(pdray src/main/main.cpp)
file(GLOB SOURCES "src/*/*.cpp")
 
add_executable(pdray ${SOURCES})