cmake_minimum_required(VERSION 3.10.0)
project(TinySQL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TinySQL src/main.cpp src/table.h src/tokenizer.h src/table.cpp src/tokenizer.cpp)
