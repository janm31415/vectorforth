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

set(IMGUI
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui.h
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_impl_opengl2.h
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_impl_sdl.h
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_stdlib.h 
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/dirent_portable.h
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imguifilesystem.h
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_draw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_demo.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_impl_opengl2.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_impl_sdl.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_stdlib.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imgui_widgets.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/imguifilesystem.cpp
)

set(HDRS
image.h
logging.h
pref_file.h
settings.h
shader_input.h
shader_program.h
view.h
    )
	
set(SRCS
logging.cpp
pref_file.cpp
main.cpp
settings.cpp
shader_program.cpp
view.cpp
)

set(JSON
${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/json/json.hpp
)

if (WIN32)
set(CMAKE_C_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_CXX_FLAGS_DEBUG "/W4 /MP /GF /RTCu /Od /MDd /Zi")
set(CMAKE_C_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /MP /GF /O2 /Ob2 /Oi /Ot /MD /Zi")
endif (WIN32)

if (UNIX)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mavx2 -mfma -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mavx2 -mfma -pthread")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mavx2 -mfma -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx2 -mfma -pthread")
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

if (WIN32)
add_executable(shaderforth WIN32 ${HDRS} ${SRCS} ${IMGUI} ${JSON})
endif (WIN32)

if (UNIX)
add_executable(shaderforth ${HDRS} ${SRCS} ${IMGUI} ${JSON})
endif(UNIX)

source_group("Header Files" FILES ${hdrs})
source_group("Source Files" FILES ${srcs})
source_group("ThirdParty/ImGui" FILES ${IMGUI})
source_group("ThirdParty/json" FILES ${JSON})

 target_include_directories(shaderforth
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../jtk/
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui_sdl/
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/json/
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2/include/
    )	
	
target_link_libraries(shaderforth
    PRIVATE
    asm
    ${OPENGL_LIBRARIES}
    SDL2
    SDL2main
    vectorforth
    )	

if (${JTK_THREADING} STREQUAL "tbb")
  target_include_directories(shaderforth
      PRIVATE
      ${TBB_INCLUDE_DIR}
      )
  target_link_libraries(shaderforth
      PRIVATE
      ${TBB_LIBRARIES}
      )
endif (${JTK_THREADING} STREQUAL "tbb")
