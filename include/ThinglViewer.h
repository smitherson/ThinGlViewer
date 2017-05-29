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
        GLFWwindow* window;
        bool initWindow(const char* windowName, const int width, const int height);
    private:
         bool isGl21 = false;
};

