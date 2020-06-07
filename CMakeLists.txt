cmake_minimum_required(VERSION 2.8.11)
project (vectorforth)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(asm)
add_subdirectory(asm.tests)
add_subdirectory(sf)
add_subdirectory(shaderforth)
add_subdirectory(vectorforth)
add_subdirectory(vectorforth.tests)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(AVX512 OFF CACHE BOOL "set ON if you want to use AVX-512. If OFF, AVX2 is used.")
set(LOOKUP ON CACHE BOOL "turn ON to speed up sin and cos computation with a lookup table.")
