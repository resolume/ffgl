#pragma once

#include "FFGLPluginSDK.h"

#include "Fireworks.h"

#define NFIREWORKS	15

class Plugin :
public CFreeFrameGLPlugin,
public ParticleListener<Particle>
{
public:
    Plugin();
    ~Plugin();
    
    ///////////////////////////////////////////////////
    // FreeFrame plugin methods
    ///////////////////////////////////////////////////
    
    FFResult SetFloatParameter(unsigned int dwIndex, float value) override;
    float GetFloatParameter(unsigned int index) override;
    FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
    FFResult InitGL(const FFGLViewportStruct *vp) override;
    FFResult DeInitGL() override;
    
    //for returning custom diplay value
    char s_Displayvalue[15];
    char* GetParameterDisplay(unsigned int index) override;
    
    ///////////////////////////////////////////////////
    // Factory method
    ///////////////////////////////////////////////////
    
    static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
    {
        *ppOutInstance = new Plugin();
        if (*ppOutInstance != NULL)
            return FF_SUCCESS;
        return FF_FAIL;
    }
    
protected:
    void shoot();
    
    void drawParticles();
    void drawFirework( Firework & f, float ptSize );
    
    void onParticleDead( Particle * particle, Emitter<Particle> * emitter ) override;
    void onParticleAdded( Particle * particle, Emitter<Particle> * emitter ) override;
    void onEmitterEmpty( Emitter<Particle> * emitter ) override;
    void onParticleEvent( Particle * particle, Emitter<Particle> * emitter, int eventId ) override;
    
    bool spawnTrigger;
    
    float 		camDistance;
    Firework	f[NFIREWORKS];
    
    bool		doFlashes;
    bool		autoShoot;
    
    int			curFirework;
    float		rotx;
    float		rotySpeed;
    float		pointSize;
    
    float		spawnRadius;
    float		damping;
    float		explosionPower;
    float		explosionRandomness;
    float		explosionDuration;

    float		trailAlpha;
    
    float		skyLight;

    GLuint ptexHandle;
    
private:
    double pTicks;
    int width;
    int height;
};


	
