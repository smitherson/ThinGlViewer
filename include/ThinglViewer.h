#pragma once
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

GLuint createShaderPipeline(const std::string & vertexShaderPath, const std::string & fragmentShaderPath, const std::string & geometryShaderPath);
  
class ThinglViewer {
  public:
    bool initWindow(const char* windowName, const int width, const int height);
    void destroyWindow();    
  protected:
    void captureInputsAndComputeMatrices();

    int width;
    int height;
    bool isGl21 = false; //my old laptop, which I still use, is OLD and I still use it - thus the support 
  
    GLFWwindow* window;
    
    //state variables for input managment
    bool isInputActive = false;
    bool isIPressed = false;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    double lastTime = glfwGetTime();

    //inited camera position, can be overriden by derived class 
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
    void manageIsInputActive();    
};

