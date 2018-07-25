cmake_minimum_required (VERSION 3.0)
project (ThinglViewer)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS " -g -std=c++14 ${CMAKE_CXX_FLAGS}" )

add_subdirectory (examples)

set (INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

include_directories( ${INCLUDE_DIRS} )

set (ThinglIncludes ${INCLUDE_DIRS} PARENT_SCOPE )

set( ThinglLibs
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW PARENT_SCOPE)

add_library(libThinglViewer STATIC 
    src/ThinglViewer.cpp  )
