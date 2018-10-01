#pragma once
#include <FFGLSDK.h>

class GLResources
{
public:
	GLResources();
	~GLResources();
	bool Initialise();
	void Release();

	void FlipBuffers();

	GLuint GetParticleTextureID() const;
	GLuint GetFrontVAOID() const;
	GLuint GetBackVAOID() const;
	GLuint GetFrontBufferID() const;
	GLuint GetBackBufferID() const;
	const ffglex::FFGLShader& GetUpdateShader() const;
	const ffglex::FFGLShader& GetRenderShader() const;

private:
	bool LoadParticleTexture();
	bool LoadVertexBuffers();
	bool LoadShaders();

	GLuint particleTextureID;
	size_t frontIndex;            //!< We're updating to the back buffer and rendering from it.
	GLuint vaoIDs[ 2 ];
	GLuint vboIDs[ 2 ];
	ffglex::FFGLShader updateShader;
	ffglex::FFGLShader renderShader;
};