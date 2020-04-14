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
    //a Vertex Array Object lets the GPU know where to get its data from, what the data is, and where to send it
    //it's meant to store everything needed to correctly render an object
    //in our case, we'll use it to store sets of 3 floats that make up a vertex coordinate
    //that get sent to a specific slot in a shader
    GLuint vaoID;
    
    // a Vertex Buffer Object is used to store information about our vertices
    // in our case, we'll just use to store the position of the vertex
    // but it could also be used to store uv texture coordinates, colors, normals or what have you
    GLuint vboID;
    
    ffglex::FFGLShader shader; //!< Utility to help us compile and link some shaders into a program.
};
