cmake_minimum_required(VERSION 2.8)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(SOURCE_FILES src/main.cxx)
add_executable(similarity_search ${SOURCE_FILES})