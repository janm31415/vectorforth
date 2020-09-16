cmake_minimum_required(VERSION 2.8.11)
project (vectorforth)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JTK_THREADING "tbb" CACHE STRING  "Choose your threading library: tbb is Intel's Threading Building Blocks library, ppl is Windows' concurrency library, std uses std::thread, and none does not use threading.")
 
set_property(CACHE JTK_THREADING PROPERTY STRINGS tbb ppl std none)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(AVX512 OFF CACHE BOOL "Set ON for AVX-512, set OFF for AVX2.")

set(SINCOS_METHOD "approximate" CACHE STRING  "Choose the method to compute sin/cos. Vectorclass is accurate. Approximate is least accurate but fastest. Lookup uses a lookup table and is faster than vectorclass but less accurate.")
 
set_property(CACHE SINCOS_METHOD PROPERTY STRINGS vectorclass lookup approximate)
 
message(STATUS "SINCOS_METHOD='${SINCOS_METHOD}'")

add_subdirectory(asm)
add_subdirectory(asm.tests)
add_subdirectory(jtk)
add_subdirectory(sf)
add_subdirectory(shaderforth)
add_subdirectory(vectorforth)
add_subdirectory(vectorforth.tests)



