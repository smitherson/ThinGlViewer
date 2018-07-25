#pragma once
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class ThinglViewer {
  public:
    bool initWindow(const char* windowName, const int width, const int height);
    GLuint createShaderPipeline(const std::string & vertexShaderPath, const std::string & fragmentShaderPath, const std::string & geometryShaderPath);
  
  protected:
    int width;
    int height;

    GLFWwindow* window;
    bool isInputActive = false;
    bool isIPressed = false;
    bool isGl21 = false; //my old laptop, which I still use, is OLD and I still use it - thus the support 
    void captureInputsAndComputeMatrices();

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    double lastTime = glfwGetTime();

    //inited camera position
    glm::vec3 position = glm::vec3( 0, 0, -1 ); 
    glm::vec3 up = glm::vec3( 0, 1, 0 ); 
    float rotationAngle = 3.14f/2;
    float rotationSpeed = 0.8f;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 35.0f; //speed of movement when pressing the arows
    float mouseSpeed = 0.005f; //speed of momvement when moving the mouse

  private:
    GLFWwindow* initGLFWAndCreateWindow(const char* windowName, const int width, const int height);

};

