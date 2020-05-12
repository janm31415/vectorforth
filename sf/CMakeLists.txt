if (WIN32)
  set(TBB_INCLUDE_DIR "C:/Program Files/TBB/include")
  set(TBB_LIBRARIES "C:/Program Files/TBB/lib/intel64/vc14/tbb.lib" "C:/Program Files/TBB/lib/intel64/vc14/tbb_debug.lib")
  set(OPENGL_LIBRARIES opengl32.lib)
endif (WIN32)

if (UNIX)
  set(TBB_INCLUDE_DIR "/usr/local/include" "/usr/include")
  set(TBB_LIBRARIES "/usr/lib/x86_64-linux-gnu/libtbb.so" "/usr/lib/x86_64-linux-gnu/libtbb.so.2")
  set(OPENGL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libOpenGL.so.0.0.0")
endif (UNIX)

set(HDRS
window.h
    )
	
set(SRCS
main.cpp
window.cpp
)

if (WIN32)
set(CMAKE_C_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_CXX_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_C_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi /DNDEBUG")
endif (WIN32)


if (UNIX)
set(CMAKE_C_FLAGS_DEBUG "-mavx2 -mfma -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-mavx2 -mfma -pthread")
set(CMAKE_C_FLAGS_RELEASE "-mavx2 -mfma -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-mavx2 -mfma -pthread")
endif (UNIX)

# general build definitions
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(sf ${HDRS} ${SRCS})
source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})

 target_include_directories(sf
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${TBB_INCLUDE_DIR}
    )	
    
if (WIN32)	
target_link_libraries(sf
    PRIVATE
    asm
    vectorforth
    ${TBB_LIBRARIES}    
    )
endif (WIN32)    

if (UNIX)	
target_link_libraries(sf
    PRIVATE
    asm
    vectorforth
    X11.so
    ${TBB_LIBRARIES}    
    )
endif (UNIX)