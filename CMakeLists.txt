cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Magma-Lib)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif(MSVC)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

add_subdirectory(src/)
