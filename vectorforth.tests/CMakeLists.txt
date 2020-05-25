
set(HDRS
compile_tests.h
debug_tests.h
optimize_tests.h
test_assert.h
tokenize_tests.h
    )
	
set(SRCS
compile_tests.cpp
debug_tests.cpp
optimize_tests.cpp
test_assert.cpp
test.cpp
tokenize_tests.cpp
)

if (WIN32)
set(CMAKE_C_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_CXX_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_C_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
endif (WIN32)

if (UNIX)
set(CMAKE_C_FLAGS_DEBUG "-mavx2 -mfma")
set(CMAKE_CXX_FLAGS_DEBUG "-mavx2 -mfma")
set(CMAKE_C_FLAGS_RELEASE "-mavx2 -mfma")
set(CMAKE_CXX_FLAGS_RELEASE "-mavx2 -mfma")
endif (UNIX)

# general build definitions
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_definitions(-DMEMORY_LEAK_TRACKING)
#add_definitions(-DAVX512)

add_executable(vectorforth.tests ${HDRS} ${SRCS})
source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})




target_include_directories(vectorforth.tests
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../
    )
	
target_link_libraries(vectorforth.tests
    PRIVATE
    asm
    vectorforth
    )	