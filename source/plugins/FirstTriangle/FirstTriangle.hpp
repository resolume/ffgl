#pragma once
#include <FFGLSDK.h>

class Triangle : public CFreeFrameGLPlugin
{
public:
    Triangle();
    
    //CFreeFrameGLPlugin
    FFResult InitGL( const FFGLViewportStruct* vp ) override;
    FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
    FFResult DeInitGL() override;
    
private:
    GLuint vaoID; //the Vertex Array Object we'll use to store the vertex data on the GPU
    GLuint vboID; // the Vertex Buffer Object we'll use to upload the vertex data to the GPU
    ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
};
