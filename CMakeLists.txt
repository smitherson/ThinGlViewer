cmake_minimum_required (VERSION 3.0)
project (ThinglViewer)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS " -g -std=c++11 ${CMAKE_CXX_FLAGS}" )

add_subdirectory (external)
add_subdirectory (examples)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/GLFW/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
  ./include/
)

set(ALL_LIBS
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130  )

add_library(libThinglViewer STATIC 
    src/ThinglViewer.cpp 
    src/viewers/ThinglPointcloudViewer.cpp
    src/shader.cpp 
    src/controls.cpp )
