cmake_minimum_required (VERSION 3.0)
project (ThinglExamples)

find_package(OpenGL REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(
        ../../include/
)

set(GL_LIBS
        libThinglViewer
        ${OPENGL_LIBRARY}
        glfw
        GLEW  )

add_executable(thinglViewerPointCLoudExample src/main.cpp src/ThinglPointcloudViewer.cpp )


target_link_libraries(thinglViewerPointCLoudExample
	${GL_LIBS} ${OpenCV_LIBS}
)
