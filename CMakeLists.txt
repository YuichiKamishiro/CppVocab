cmake_minimum_required(VERSION 3.0...3.5)
project(cppvocab)

set(CMAKE_CXX_STANDARD 23)

add_executable(cppvocab main.cpp)

target_include_directories(cppvocab PRIVATE ${CMAKE_SOURCE_DIR}/include/)