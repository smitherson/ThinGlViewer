#pragma once
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ThinglViewer {
  public:
    void testRender();
    bool initWindow(const char* windowName, const int width, const int height);
  protected:
    GLFWwindow* window;
    bool isInputActive = false;
    bool isIPressed = false;
    bool isGl21 = false;
    void computeMatricesFromInputs();
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    double lastTime = glfwGetTime();

    glm::vec3 position = glm::vec3( 0, 0, -1 ); 
    glm::vec3 up = glm::vec3( 0, 1, 0 ); 
    
    float rotationAngle = 3.14f/2;
    float rotationSpeed = 0.8f;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 35.0f;
    float mouseSpeed = 0.005f;

};

