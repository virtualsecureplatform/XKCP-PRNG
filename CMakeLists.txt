cmake_minimum_required(VERSION 3.16)
project(xkcp-prng C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-march=native -O3 -g -funroll-loops -Wall -Wextra -pedantic -Wno-sign-compare"
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -O3")

option(USE_AVX512 "Use AVX512 version " OFF)

option(USE_AVX2 "Use AVX2 version" ON)

option(ENABLE_TEST "Build tests" ON)

if(USE_AVX512)
    set(XKCP_TARGET "AVX512")
elseif(USE_AVX2)
    set(XKCP_TARGET "AVX2")
else()
    set(XKCP_TARGET "generic64")
endif()

set(XKCP_ROOT "${PROJECT_SOURCE_DIR}/thirdparties/XKCP")
include(thirdparties/XKCP.cmake)
add_subdirectory(thirdparties/BLAKE3/c)
add_subdirectory(test)