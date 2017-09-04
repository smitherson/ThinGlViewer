#pragma once
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

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
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 35.0f; // 3 units / second
    float mouseSpeed = 0.005f;

};

