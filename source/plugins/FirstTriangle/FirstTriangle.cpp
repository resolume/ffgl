//
//  FirstTriangle.cpp
//  FFGLPlugins
//
//  Created by Joris on 25/10/2018.
//

#include "FirstTriangle.hpp"

static CFFGLPluginInfo PluginInfo(
                                  PluginFactory< Triangle >,// Create method
                                  "RS03",                        // Plugin unique ID
                                  "First Triangle",              // Plugin name
                                  2,                             // API major version number
                                  1,                             // API minor version number
                                  1,                             // Plugin major version number
                                  000,                           // Plugin minor version number
                                  FF_SOURCE,                     // Plugin type
                                  "My First Triangle",           // Plugin description
                                  "Resolume FFGL Example"        // About
);

static const char vertexShaderCode[] = R"(#version 410 core
//"location" is basically a slot we use to get data into the shader
//in a bit, we'll tell our VAO to send vertex data into the first slot
layout( location = 0 ) in vec3 vPosition;

void main()
{
    //set the vertices based on the data from the VBO
    gl_Position.xyz = vPosition;
    gl_Position.w = 1.0;
}
)";

static const char fragmentShaderCode[] = R"(#version 410 core
out vec4 color;
void main(){
    color = vec4(1,0,0,1); //opaque red
}
)";

Triangle::Triangle()
{
    // Input properties
    SetMinInputs( 0 );
    SetMaxInputs( 0 );
}

FFResult Triangle::InitGL( const FFGLViewportStruct* vp )
{
    //create the VAO and VBO
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    
    //if we can't compile our shaders, quit here
    //the plugin will load, but it will just pass through the texture unaffected when applied
    if( !shader.Compile( vertexShaderCode, fragmentShaderCode ) )
    {
        DeInitGL();
        return FF_FAIL;
    }
    
    // An array of x,y and z coordinates which represent 3 vertices
    static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };
    
    //bind the VAO and VBO we just created
    //again, a VAO is basically a bunch of settings that tell the GPU what kind of data we're using
    //(in this case it will be sets of 3 floats that make up a vertex coordinate)
    //and where it needs to send it
    //(in this case it will be the first slot in the shader)
    ffglex::ScopedVAOBinding vaoBinding( vaoID );
    ffglex::ScopedVBOBinding vboBinding(vboID);
    
    //now we need to tell the VAO the vertices should be used as data for the shader
    //we only have to do this once during init, because the data doesn't change
    
    //first write the vertex data into a VBO
    //the VBO was created in line 54 and bound in line 77
    //basically, it's a copy of the data in the float array, except it's stored in the high speed memory of the GPU
    //because our VBO was bound after our VAO, our VAO now knows it has to use the data in this VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //now we tell the VAO to open the first slot in the shader
    //without this, the VAO won't have anywhere to send the data we just put in the VBO
    glEnableVertexAttribArray( 0 );
    
    //now we tell the VAO how the numbers in the VBO are arranged and where to send them
    glVertexAttribPointer(
                          0,                  // attribute 0, this will let the VAO know to send the vertex data in our buffer to the "vPosition" slot of our shader
                          3,                  // size is 3, every vertex has a GLfloat for x, y and z
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized
                          0,                  // stride is 0 because the array doesn't have any other data besides x, y and z
                          (void*)0            // array buffer offset, we start at the x value of the first vertex
                          );
    
    //be a good programmer and unbind the VAO and VBO
    //the scoped class does this for us when we forget
    //but it's good practice to do it the right way
    vboBinding.EndScope();
    vaoBinding.EndScope();
    
    //Use base-class init as success result so that it retains the viewport.
    return CFreeFrameGLPlugin::InitGL( vp );
}

FFResult Triangle::DeInitGL()
{
    //clean up our VAO, VBO and shader
    glDeleteBuffers( 1, &vboID );
    vboID = 0;
    glDeleteVertexArrays( 1, &vaoID );
    vaoID = 0;
    shader.FreeGLResources();
    
    return FF_SUCCESS;
}

FFResult Triangle::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
    //clear to a color
    glClearColor(0.0f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //here we tell the GPU to use the information from the VAO and shader we created during init
    //remember, VAO's are a way to recall the GPU to the correct state
    //so essentially, we're telling the GPU again to
    // 1) use the same VBO we created and set during init
    // 2) open the fist slot in the shader
    // 3) that the data in the VBO are sets of 3 floats that need to be sent to the first slot
    // 4) use the same shader we created during init
    //FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that
    ffglex::ScopedVAOBinding vaoBinding( vaoID );
    ffglex::ScopedShaderBinding shaderBinding( shader.GetGLID() );
  
    // Draw the triangle
    // The 3 vertices we wrote in the VBO are now used to draw the three corner points of a triangle
    // Starting from vertex 0; 3 vertices in total makes one beautiful triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //that's it, we're done. because everything was set up and set in the VAO during init, we can leave this party and go home
    //clean up again
    //this is important becasue otherwise any rendering done by Resolume or other plugins after this plugin, might use our settings!
    shaderBinding.EndScope();
    vaoBinding.EndScope();

    return FF_SUCCESS;
}
