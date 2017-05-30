#include "ThinglPointcloudViewer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <shader.hpp>
#include <controls.hpp>


void ThinglPointcloudViewer::showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage,
                    const cv::Mat_<unsigned short>& depthImage,
                    const cv::Mat& K) {

    float fxi,fyi,cxi,cyi;
    cv::Mat_<float> Kinv = K.inv();
    fxi = Kinv(0,0);
    fyi = Kinv(1,1);
    cxi = Kinv(0,2);
    cyi = Kinv(1,2);

    CloudVertexNoColor* cloudBuffer = new CloudVertexNoColor[colorImage.rows*colorImage.cols];
    unsigned short depth;
    int points = 0;
    for (int row=0;row<colorImage.rows;row++) { //height
        for (int col=0;col<colorImage.cols;col++) { //width
            if ( depthImage(row,col) <= 0 )
                continue;
            points++;

            depth = depthImage(row, col);
            cloudBuffer[points].point[0] = (col*fxi + cxi) * depth;
            cloudBuffer[points].point[1] = (row*fyi + cyi) * depth;
            cloudBuffer[points].point[2] = depth;

            /*cloudBuffer[points].color[3] = 100;
            cloudBuffer[points].color[2] = colorImage(row,col)[2];
            cloudBuffer[points].color[1] = colorImage(row,col)[1];
            cloudBuffer[points].color[0] = colorImage(row,col)[0];*/

        } 
    }
    printf("nubber of points:%d\n", points);
    int vertexBufferNumPoints = points;
    // create new ones, static

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID;
    if (!isGl21) {
        programID = LoadShaders( "../src/SimpleTransform330.vertexshader", "../src/SingleColor330.fragmentshader" );
    } else {
        programID = LoadShaders( "../src/SimpleTransform120.vertexshader", "../src/SingleColor120.fragmentshader" );
    }

    vertexBufferId=0;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);         // for vertex coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(CloudVertexNoColor) * points, cloudBuffer, GL_STATIC_DRAW);

    delete cloudBuffer;

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glm::mat4 MVP      = glm::mat4(1.0f);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    do {

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        //we get the matrix of the camera here
        computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        //now we add the buffers
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

        glDrawArrays(GL_POINTS, 0, vertexBufferNumPoints);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0 );

	glDeleteBuffers(1, &vertexBufferId);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

