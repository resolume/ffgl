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
	void UpdateParticles();
	void RenderParticles();

	GLResources glResources;

	//Parameters
	float fadeoutStart;
	float smokeStart;
	float smokeIntensity;
	float energyMaxAgeFactor;
	float turbulenceDetail;      //!< In range between 0.5 .. 1.5
	float turbulenceSpeed;       //!< In range between 0.1 .. 2.0
	float maxAge;
};
