#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedFBOBinding
{
public:
	enum RestoreBehaviour
	{
		RB_CLEAR, //When this behaviour is used, on finishing this fbo scope it'll clear the bound framebuffers to 0.
		RB_REVERT,//When this behaviour is used, on finishing this fbo scope it'll rever the bound framebuffers to what they were on scope construction.
	};

	ScopedFBOBinding( GLuint fboID, RestoreBehaviour restoreBehaviour );
	~ScopedFBOBinding();

	void EndScope();

private:
	bool isBound;
	RestoreBehaviour restoreBehaviour;

	GLint previousDrawFboID;
	GLint previosReadFboID;
};

}//End namespace ffglex