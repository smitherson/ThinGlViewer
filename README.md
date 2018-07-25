# ThinglViewer
Very simple opengl viewer for scientific debugging and visualizing. Simpler than libqglviewer. 

Based on https://github.com/opengl-tutorials/ogl

Still in very early development.

Main usage is 

Create a derived class:
``
class ThinglPointcloudViewer : public ThinglViewer {
    public:
        void showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage, const cv::Mat_<unsigned short>& depthImage,const cv::Mat& K);
};

``

In showPointcloud - put all the logic that manages your vertex arrays, buffers, etc.

There is 'createShaderPipeline' for reading, compiling and linking the shaders.

``
programId = createShaderPipeline(shaderDir + "TransformVertexShader330.vertexshader", 
                                         shaderDir + "ColorFragmentShader330.fragmentshader", "" );
``

and 'captureInputsAndComputeMatrices' which parses the input of the keyboard nad provides model and view matrices 
``
   captureInputsAndComputeMatrices();
        glm::mat4 modelMatrix = glm::mat4(1.0);
        MVP = projectionMatrix * viewMatrix * modelMatrix;
``       

The input commands are

i: starts input in the window - you can move with the arrows and WASD keys, go up and down with 'space' and 'ctrl' and look around with the mouse (same as FPS game)
o: resets your position in the start position
