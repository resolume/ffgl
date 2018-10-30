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
    GLuint vaoID; //a Vertex Array Object lets the GPU know where to get its data from, what the data is, and where to send it in the shader
    GLuint vboID; // a Vertex Buffer Object is used to store information about our vertices
    ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
};
