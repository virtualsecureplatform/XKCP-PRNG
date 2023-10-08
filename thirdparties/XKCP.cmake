# Allow specifying the build target for XKCP
set(XKCP_TARGET "" CACHE STRING "Build target for XKCP (e.g., AVX2,AVX512,ARMv8A)")

# Define a custom target to build the library using its makefile with the specified target
add_custom_target(XKCP ALL
                  COMMAND make ${XKCP_TARGET}/libXKCP.a
                  WORKING_DIRECTORY  ${XKCP_ROOT}
                  COMMENT "Building XKCP using its Makefile with target: ${XKCP_TARGET}")

set(XKCP_INCLUDE_DIR "${XKCP_ROOT}/bin/${XKCP_TARGET}/libXKCP.a.headers")
set(XKCP_LIBRARY "${XKCP_ROOT}/bin/${XKCP_TARGET}/libXKCP.a")