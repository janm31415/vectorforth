set(HDRS
asm_api.h
assembler.h
asmcode.h
namespace.h
)
	
set(SRCS
assembler.cpp
asmcode.cpp
)

if (WIN32)
set(CMAKE_C_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_CXX_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_C_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
endif (WIN32)


# general build definitions
add_definitions(-D_SCL_SECURE_NO_WARNINGS)

add_library(asm SHARED ${HDRS} ${SRCS})
source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})

target_include_directories(asm
    PRIVATE
    )	
	
target_link_libraries(asm
    PRIVATE	
    )	