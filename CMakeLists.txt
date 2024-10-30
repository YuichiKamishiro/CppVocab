cmake_minimum_required(VERSION 3.0...3.5)
project(CppVocab)

set(CMAKE_CXX_STANDARD 23)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)
