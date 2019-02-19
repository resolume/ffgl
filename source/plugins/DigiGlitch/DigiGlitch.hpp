//
//  DigiGlitch.hpp
//  FFGLPlugins
//
//  Created by Joris on 19/02/2019.
//

#ifndef DigiGlitch_hpp
#define DigiGlitch_hpp

#include <FFGLSDK.h>

class DigiGlitch : public CFreeFrameGLPlugin
{
public:
    DigiGlitch();
    ~DigiGlitch();
    
    virtual FFResult InitGL( const FFGLViewportStruct* vp ) override;
    virtual FFResult DeInitGL() override;
    virtual FFResult ProcessOpenGL( ProcessOpenGLStruct* pOpenGLData ) override;
    virtual FFResult SetFloatParameter( unsigned int index, float value ) override;
    virtual float GetFloatParameter( unsigned int index ) override;
    
private:
    ffglex::FFGLShader shader;
    ffglex::FFGLScreenQuad quad;
    
};
#endif /* DigiGlitch_hpp */
