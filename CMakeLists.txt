cmake_minimum_required(VERSION 3.16)
project(xoodoo-prng C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-march=native -O3 -g -funroll-loops -Wall -Wextra -pedantic -Wno-sign-compare"
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -O3")

option(USE_SSE2 "Use SSE2 version of Xoodoo" ON)

option(ENABLE_TEST "Build tests" ON)

add_subdirectory(test)