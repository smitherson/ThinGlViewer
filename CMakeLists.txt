# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)

add_subdirectory (external)
include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/GLFW/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
    include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130)

add_executable(thinGLViewerSample 
	src/thinGLViewerSample.cpp src/shader.cpp
)

target_link_libraries(thinGLViewerSample
	${ALL_LIBS}
)
