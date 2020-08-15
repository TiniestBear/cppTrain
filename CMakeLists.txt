# CMakeList.txt : CMake project for c11, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("c11")

# Add source to this project's executable.
add_executable (c11 "c11.cpp" "c11.h")

# TODO: Add tests and install targets if needed.
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)