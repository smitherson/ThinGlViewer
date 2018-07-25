#include "ThinglViewer.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace glm;

GLFWwindow* ThinglViewer::initGLFWAndCreateWindow(const char* windowName, const int width, const int height) {
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return NULL;
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
  
  window = glfwCreateWindow( width, height, windowName, NULL, NULL);
  isGl21 = false;
  
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Trying to switch to 2.1.\n" );
    glfwTerminate();
    glfwInit(); 
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow( width, height, windowName, NULL, NULL);
    if( window == NULL ){
      fprintf( stderr, "Failed to switch to 2.1. Exiting.\n" );
      glfwTerminate();
      return NULL;
    }
    isGl21 = true;
    fprintf(stderr, "Succesfully switched to GL 2.1\n");
  }

  glfwMakeContextCurrent(window);
  return NULL;
} 

bool ThinglViewer::initWindow(const char* windowName, const int width, const int height) {
  this->width = width;
  this->height = height;

  initGLFWAndCreateWindow(windowName, width, height);

  // Initialize GLEW
  if (!isGl21) {
    glewExperimental = true; // Needed for core profile
  }
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    glfwTerminate();
    return false;
  }
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  return true;
}

void ThinglViewer::captureInputsAndComputeMatrices() {

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
  
  if ( glfwGetKey( window, GLFW_KEY_I ) == GLFW_RELEASE ) {
    isIPressed = false;
  }
  if ( glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS && isIPressed == false ) {
    isIPressed = true;
    if ( isInputActive ) {
      isInputActive = false; 
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else { 
      lastTime = glfwGetTime();
      glfwSetCursorPos(window, width/2, height/2);
      isInputActive = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
  }

  int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

  if (!focused) {
    isInputActive = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } 

  if (!isInputActive) return;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, width/2, height/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(width/2 - xpos );
	verticalAngle   += mouseSpeed * float(- height/2 + ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	//glm::vec3 up = glm::cross( right, direction );

	// Rotate clockwise
	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS) {
    rotationAngle += deltaTime*rotationSpeed;
    up = glm::vec3( cos(rotationAngle),sin(rotationAngle),0);
	}
	if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS) {
    rotationAngle -= deltaTime*rotationSpeed;
    up = glm::vec3( cos(rotationAngle),sin(rotationAngle),0);
	}



	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS || 
        glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position -=  direction* deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS || 
        glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS || 
        glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ||
        glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Go up
	if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		position += up * deltaTime * speed;
	}
	// Go down
	if (glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS){
		position -= up * deltaTime * speed;
	}

  // Go to zero!
	if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS){
		position = glm::vec3( 0, 0, 0 );
    direction  = glm::vec3( 0, 0 , -10 ); 
    up = glm::vec3( 0, 1, 0);
    horizontalAngle = 3.14;
    verticalAngle = 0;
    lastTime = glfwGetTime();
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
  /*printf ("------\n direction %f %f %f\n", direction[0],direction[1],direction[2]);
  printf ("position %f %f %f\n", position[0], position[1], position[2]);
  printf ("up %f %f %f\n", up[0], up[1], up[2]);*/

	ViewMatrix  = glm::lookAt(
								position,           // Camera is here
								position-direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

std::string readShaderCode(const std::string & shaderFilePath) {

  std::ifstream shaderFile;
  shaderFile.open(shaderFilePath);
  if (! shaderFile.is_open() ) {
    fprintf(stderr, "Cannot open shader %s.\n", shaderFilePath.c_str());
    return "";
  }

  std::stringstream strStream;
  strStream << shaderFile.rdbuf();
  
  std::string shaderCode = strStream.str();
  return shaderCode;
}

bool compileShaderCode(const GLuint shaderId, const std::string & shaderCode ) {
	
  const GLchar *source = (const GLchar *) (shaderCode.c_str());
  glShaderSource(shaderId, 1,  &source , NULL);
	glCompileShader(shaderId);

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if ( infoLogLength > 0 ){
		std::vector<char> shaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
		printf("shader compilation problem: %s\n", &shaderErrorMessage[0]);
	}
}

GLuint ThinglViewer::createShaderPipeline(const std::string & vertexShaderPath, const std::string & fragmentShaderPath, const std::string & geometryShaderPath) {

	GLuint vertexShaderId   = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexShaderCode = readShaderCode(vertexShaderPath);
  compileShaderCode(vertexShaderId, vertexShaderCode);

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderCode = readShaderCode(fragmentShaderPath);
  compileShaderCode(fragmentShaderId, fragmentShaderCode);


  GLuint geometryShaderId;
  bool hasGeometryShader = geometryShaderPath.size() > 0;

  if (hasGeometryShader) {
    std::string geometryShaderCode;
    geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
    geometryShaderCode = readShaderCode(geometryShaderPath);   
    compileShaderCode(geometryShaderId, geometryShaderCode);
  }
 
	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
  if (hasGeometryShader) {
    glAttachShader(programId, geometryShaderId);
  }
	glLinkProgram(programId);
 
  GLint result = GL_FALSE;
  int infoLogLength; 
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("linking program eror: %s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

  if (hasGeometryShader) {
    glDetachShader(programId, geometryShaderId);
    glDeleteShader(geometryShaderId);
  }

	return programId;
}


