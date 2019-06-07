#pragma once
#include <FFGLSDK.h>
#include "GLResources.h"

class Particles : public Plugin
{
public:
	Particles();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult DeInitGL() override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;

protected:
	void UpdateParticles( float deltaTime );
	void RenderParticles();

	GLResources glResources;     //!< The container of our OpenGL resources.

	//Parameters
	ParamRange::Ptr fadeoutStart;
	ParamRange::Ptr smokeStart;
	ParamRange::Ptr smokeIntensity;
	ParamRange::Ptr energyMaxAgeFactor;
	ParamRange::Ptr turbulenceDetail;		//!< In range between 0.5 .. 1.5
	ParamRange::Ptr turbulenceSpeed;		//!< In range between 0.1 .. 2.0
	ParamOption::Ptr maxAge;
	ParamRange::Ptr particleSize;			//!< In range between 1.0 .. 10.0
	ParamRange::Ptr velocityToSizeFactor;	//!< In range between 1.0 .. 20.0
	ParamRange::Ptr numBuckets;				//!< In range between 16 .. MAX_BUCKETS
	ParamRange::Ptr numParticlesPerBucket;	//!< In range between 0 .. MAX_PARTICLES_PER_BUCKET
	ParamRange::Ptr burstDuration;
	ParamRange::Ptr burstIntensity;			//!< In range between 1.0 .. 16.0
	ParamBool::Ptr simulate;
};
