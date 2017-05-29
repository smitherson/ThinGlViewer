cmake_minimum_required (VERSION 3.0)
project (ThinglExamples)

find_package(OpenGL REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(
        ../external/AntTweakBar-1.16/include/
        ../external/glfw-3.1.2/include/GLFW/
        ../external/glm-0.9.7.1/
        ../external/glew-1.13.0/include/
        ../external/assimp-3.0.1270/include/
        ../external/bullet-2.81-rev2613/src/
        ../include/
)

set(GL_LIBS
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130  )

add_executable(thinglViewerSampleSimple 
	thinglViewerSampleSimple.cpp )

add_executable(thinglViewerPointCloud
	thinglViewerPointCloud.cpp )


target_link_libraries(thinglViewerSampleSimple
	${GL_LIBS}
)

target_link_libraries(thinglViewerPointCloud
	${GL_LIBS} ${OpenCV_LIBS}
 )
