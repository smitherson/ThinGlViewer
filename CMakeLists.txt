cmake_minimum_required (VERSION 3.0)
project (ThinglViewer)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS " -g -std=c++11 ${CMAKE_CXX_FLAGS}" )

add_subdirectory (external)
add_subdirectory (examples)



set (INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar-1.16/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.1.2/include/GLFW/
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.7.1/
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glew-1.13.0/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp-3.0.1270/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet-2.81-rev2613/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

include_directories( ${INCLUDE_DIRS} )

set (ThinglIncludes ${INCLUDE_DIRS} PARENT_SCOPE )

set( ThinglLibs
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130 PARENT_SCOPE)

add_library(libThinglViewer STATIC 
    src/ThinglViewer.cpp 
    src/viewers/ThinglPointcloudViewer.cpp
    src/shader.cpp )
