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
