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

if (APPLE)
  set(TBB_INCLUDE_DIR "/usr/local/include" "/usr/include")
  set(TBB_LIBRARIES "/usr/local/lib/libtbb.dylib")
  set(OPENGL_LIBRARIES "/System/Library/Frameworks/OpenGL.framework/OpenGL")
endif (APPLE)

set(HDRS
thread_pool.h
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

if (VECTORFORTH_AVX512)
add_definitions(-DAVX512)
endif (VECTORFORTH_AVX512)

if (${VECTORFORTH_SINCOS_METHOD} STREQUAL "lookup")
add_definitions(-DSINCOSLOOKUP)
endif (${VECTORFORTH_SINCOS_METHOD} STREQUAL "lookup")

if (${VECTORFORTH_SINCOS_METHOD} STREQUAL "approximate")
add_definitions(-DSINCOSAPPROX)
endif (${VECTORFORTH_SINCOS_METHOD} STREQUAL "approximate")

include ("../jtk/jtk/jtk.cmake")

add_executable(sf ${HDRS} ${SRCS})
source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})
    
if (WIN32)	
 target_include_directories(sf
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../jtk/
    )	

target_link_libraries(sf
    PRIVATE
    asm
    vectorforth
    )
endif (WIN32)    

if (UNIX)	

set(XLIBINCLUDE "")
set(XLIBLIBRARY "X11.so")

if (APPLE)
set(XLIBINCLUDE "/usr/X11/include/")
set(XLIBLIBRARY "/usr/X11/lib/libX11.dylib")
endif (APPLE)

target_include_directories(sf
   PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/../
   ${CMAKE_CURRENT_SOURCE_DIR}/../jtk/
   ${XLIBINCLUDE}
   )

target_link_libraries(sf
    PRIVATE
    asm
    vectorforth
    ${XLIBLIBRARY}
    )
endif (UNIX)

if (${JTK_THREADING} STREQUAL "tbb")
  target_include_directories(sf
      PRIVATE
      ${TBB_INCLUDE_DIR}
      )
  target_link_libraries(sf
      PRIVATE
      ${TBB_LIBRARIES}
      )
endif (${JTK_THREADING} STREQUAL "tbb")
