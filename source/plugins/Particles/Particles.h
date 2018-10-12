#pragma once
#include <FFGLSDK.h>
#include "GLResources.h"

class Particles : public CFreeFrameGLPlugin
{
public:
	Particles();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult DeInitGL() override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;

	float GetFloatParameter( unsigned int index ) override;

protected:
	void UpdateParticles( float deltaTime );
	void RenderParticles();

	GLResources glResources;     //!< The container of our OpenGL resources.

	//Parameters
	float fadeoutStart;
	float smokeStart;
	float smokeIntensity;
	float energyMaxAgeFactor;
	float turbulenceDetail;      //!< In range between 0.5 .. 1.5
	float turbulenceSpeed;       //!< In range between 0.1 .. 2.0
	float maxAge;
	float particleSize;          //!< In range between 1.0 .. 10.0
	float velocityToSizeFactor;  //!< In range between 1.0 .. 20.0
	int numBuckets;              //!< In range between 16 .. MAX_BUCKETS
	int numParticlesPerBucket;   //!< In range between 0 .. MAX_PARTICLES_PER_BUCKET
	float burstDuration;
	float burstIntensity;        //!< In range between 1.0 .. 16.0
	bool simulate;
	float lastUpdate;            //!< Time of the last update in seconds.
};
