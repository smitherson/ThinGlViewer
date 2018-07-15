cmake_minimum_required (VERSION 3.0)
project (ThinglExamples)

find_package(OpenGL REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(
        ../include/
)

set(GL_LIBS
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW  )

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
