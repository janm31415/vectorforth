
set(HDRS
asm_aux.h
avx_mathfun.h
compiler.h
compile_data.h
compile_error.h
constant_folding.h
context.h
context_defs.h
debug.h
dictionary.h
expand.h
expand_data.h
namespace.h
primitives.h
stdlib.h
strength_reduction.h
tokenize.h
vectorforth_api.h
)
	
set(SRCS
asm_aux.cpp
avx_mathfun.cpp
context.cpp
compiler.cpp
compile_error.cpp
constant_folding.cpp
debug.cpp
expand.cpp
dictionary.cpp
primitives.cpp
stdlib.cpp
strength_reduction.cpp
tokenize.cpp
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
#add_definitions(-DAVX512)

add_library(vectorforth SHARED ${HDRS} ${SRCS})
source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})

target_include_directories(vectorforth
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/vectorclass
    )	
    
	
target_link_libraries(vectorforth
    PRIVATE	
    asm
    )	
