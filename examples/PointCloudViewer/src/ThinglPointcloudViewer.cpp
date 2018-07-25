#include "ThinglPointcloudViewer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

void ThinglPointcloudViewer::showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage,
        const cv::Mat_<unsigned short>& depthImage,
        const cv::Mat& K) {
    

    size_t found;
    std::string str(__FILE__);
    found=str.find_last_of("/\\");
    const std::string shaderDir = str.substr(0,found+1)+"shaders/";

    //printf("Shader files are in: %s\n", shaderDir.c_str());

    float fxi,fyi,cxi,cyi;
    cv::Mat_<float> Kinv = K.inv();
    fxi = Kinv(0,0);
    fyi = Kinv(1,1);
    cxi = Kinv(0,2);
    cyi = Kinv(1,2);

    float *cloudVertices = new float[colorImage.rows*colorImage.cols*3];
    float *cloudColors = new float[colorImage.rows*colorImage.cols*3];

    float depth;
    int points = 0, colors = 0;
    for (int row=0;row<colorImage.rows;row++) {
        for (int col=0;col<colorImage.cols;col++) {
            if ( depthImage(row,col) <= 0 )
                continue;

            depth = ((float) depthImage(row, col))/(float)800;
            cloudVertices[points++] = - (col*fxi + cxi) * depth;
            cloudVertices[points++] = - (row*fyi + cyi) * depth;
            cloudVertices[points++] = depth;

            cloudColors[colors++] = colorImage(row,col)[2]/255.0f;
            cloudColors[colors++] = colorImage(row,col)[1]/255.0f;
            cloudColors[colors++] = colorImage(row,col)[0]/255.0f;
        } 
    }
    int vertexBufferNumPoints = points/3;

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint programId;
    GLuint vertexPosition_modelspaceId = 0;
    GLuint vertexColorId = 0;

    if (!isGl21) {
        programId = createShaderPipeline(shaderDir + "TransformVertexShader330.vertexshader", 
                                         shaderDir + "ColorFragmentShader330.fragmentshader", "" );
        vertexPosition_modelspaceId = 0;
        vertexColorId = 1;
    } else {
        programId = createShaderPipeline(shaderDir + "TransformVertexShader120.vertexshader", 
                                         shaderDir + "ColorFragmentShader120.fragmentshader", "" );
        vertexPosition_modelspaceId = glGetAttribLocation(programId, "vertexPosition_modelspace");
        vertexColorId = glGetAttribLocation(programId, "vertexColor");
    }
    GLuint vertexBufferId, colorBufferId;

    vertexBufferId=0;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);         // for vertex coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexBufferNumPoints, cloudVertices, GL_STATIC_DRAW);

    colorBufferId=0;
    glGenBuffers(1, &colorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);         // for vertex coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexBufferNumPoints, cloudColors, GL_STATIC_DRAW);

    GLuint matrixId = glGetUniformLocation(programId, "MVP");
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    projectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
    position = glm::vec3(0,0, 1);
    viewMatrix       = glm::lookAt(    
            position , // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );  
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 MVP        = projectionMatrix * viewMatrix * model; // Remember, matrix multiplication is the other way around

    do {

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programId);

        //we get the matrix of the camera here
        captureInputsAndComputeMatrices();
        glm::mat4 modelMatrix = glm::mat4(1.0);
        MVP = projectionMatrix * viewMatrix * modelMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(vertexPosition_modelspaceId);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glVertexAttribPointer(
                vertexPosition_modelspaceId,                   // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                   // size
                GL_FLOAT,            // type
                GL_FALSE,            // normalized?
                0,                   // stride
                (void*)0             // array buffer offset
                );

        glEnableVertexAttribArray(vertexColorId);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
        glVertexAttribPointer(
                vertexColorId,                   // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                   // size
                GL_FLOAT,             // type
                GL_FALSE,            // normalized?
                0,                   // stride
                (void*)0             // array buffer offset
                );

        glDrawArrays(GL_POINTS, 0, vertexBufferNumPoints);
        glDisableVertexAttribArray(vertexPosition_modelspaceId);
        glDisableVertexAttribArray(vertexColorId);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &vertexBufferId);
    glDeleteProgram(programId);
    glDeleteVertexArrays(1, &vertexArrayId);
    delete cloudVertices;
    delete cloudColors;
}

void ThinglViewer::destroyWindow() {
  glfwTerminate();
}

