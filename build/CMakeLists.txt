cmake_minimum_required(VERSION 3.10)
project(CHESS_V2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(CHESS_V2 ${SOURCES})