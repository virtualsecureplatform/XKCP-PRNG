
file(GLOB test_sources RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "*.cpp")
enable_language(ASM)
foreach(test_source ${test_sources})
    string( REPLACE ".cpp" "" test_name ${test_source} )
    add_executable(${test_name} ${test_source} 
    randen/randen.cc randen/nanobenchmark.cc)
	target_include_directories(${test_name} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${XKCP_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparties/BLAKE3/c
    ${PROJECT_SOURCE_DIR}/thirdparties/low-latency-crypto-areion/include
    )
    target_link_libraries(${test_name} PRIVATE ${XKCP_LIBRARY} blake3 areion)
endforeach(test_source ${test_sources})

target_link_libraries(wrapper4testu01 PRIVATE testu01 m)