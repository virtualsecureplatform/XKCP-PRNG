
file(GLOB test_sources RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "*.cpp")

foreach(test_source ${test_sources})
    string( REPLACE ".cpp" "" test_name ${test_source} )
    # target_link_libraries(${test_name} INTERFACE xoodoo)
    if(USE_AVX512)
    add_executable(${test_name} ${test_source} ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/AVX512/Xoodoo-SIMD512.c randen/randen.cc randen/nanobenchmark.cc)
	target_include_directories(${test_name} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/common
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/AVX512
    )
	elseif(USE_SSE2)
    add_executable(${test_name} ${test_source} ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/SSE2/Xoodoo-SIMD128.c randen/randen.cc randen/nanobenchmark.cc)
	target_include_directories(${test_name} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/common
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/SSE2
	)
	else()
    add_executable(${test_name} ${test_source} ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/plain/Xoodoo-optimized.c randen/randen.cc randen/nanobenchmark.cc)
	target_include_directories(${test_name} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/common
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo
    ${PROJECT_SOURCE_DIR}/thirdparties/XKCP/lib/low/Xoodoo/plain
	)
	endif()
endforeach(test_source ${test_sources})

target_link_libraries(wrapper4testu01 PRIVATE testu01 probdist mylib m)