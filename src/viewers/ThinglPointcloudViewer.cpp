#include "ThinglPointcloudViewer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <shader.hpp>




void ThinglPointcloudViewer::showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage,
        const cv::Mat_<unsigned short>& depthImage,
        const cv::Mat& K) {
    

    size_t found;
    std::string str(__FILE__);
    found=str.find_last_of("/\\");
    const char* dir = str.substr(0,found).c_str();


    printf("%s\n", dir);

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
    for (int row=0;row<colorImage.rows;row++) { //height
        for (int col=0;col<colorImage.cols;col++) { //width
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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID;
    GLuint vertexPosition_modelspaceID = 0;
    GLuint vertexColorID = 0;

    if (!isGl21) {
        programID = LoadShaders( "../examples/TransformVertexShader330.vertexshader", "../examples/ColorFragmentShader330.fragmentshader" );
        vertexPosition_modelspaceID = 0;
        vertexColorID = 1;
    } else {
        programID = LoadShaders( "../examples/TransformVertexShader120.vertexshader", "../examples/ColorFragmentShader120.fragmentshader" );
        vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
        vertexColorID = glGetAttribLocation(programID, "vertexColor");
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

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
    position = glm::vec3(0,0, 1);
    ViewMatrix       = glm::lookAt(    
            position , // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );  
    glm::mat4 Model      = glm::mat4(1.0f);
    glm::mat4 MVP        = ProjectionMatrix * ViewMatrix * Model; // Remember, matrix multiplication is the other way around

    do {

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        //we get the matrix of the camera here
        computeMatricesFromInputs();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glVertexAttribPointer(
                vertexPosition_modelspaceID,                   // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                   // size
                GL_FLOAT,            // type
                GL_FALSE,            // normalized?
                0,                   // stride
                (void*)0             // array buffer offset
                );

        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
        glVertexAttribPointer(
                vertexColorID,                   // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                   // size
                GL_FLOAT,             // type
                GL_FALSE,            // normalized?
                0,                   // stride
                (void*)0             // array buffer offset
                );

        glDrawArrays(GL_POINTS, 0, vertexBufferNumPoints);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &vertexBufferId);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    delete cloudVertices;
    delete cloudColors;
    glfwTerminate();
}

