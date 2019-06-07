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
	ffglex::FFGLShader& GetUpdateShader();
	ffglex::FFGLShader& GetRenderShader();

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