////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGLFBO.h
//
// FreeFrame is an open-source cross-platform real-time video effects plugin system.
// It provides a framework for developing video effects plugins and hosts on Windows,
// Linux and Mac OSX.
//
// Copyright (c) 2018 www.freeframe.org
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Redistribution and use in source and binary forms, with or without modification,
//	are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//  * Neither the name of FreeFrame nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//	IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//	OF THE POSSIBILITY OF SUCH DAMAGE.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// First version, Menno Vink (menno@resolume.com)
// www.resolume.com
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FFGLFBO_STANDARD
#define FFGLFBO_STANDARD
#include "../ffgl/FFGL.h"

namespace ffglex
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		FFGLFBO
/// \brief		FFGLFBO can be used to capture the output of your OpenGL draw commands into a texture.
/// \author		Menno Vink (menno@resolume.com)
/// \date		2019-09-02
///
/// The FFGLFBO class can be used to initialize an OpenGL FramebufferObject at a certain size. It'll automatically take care of
/// creating the framebuffer object, the texture and the depth buffer and attaching these buffers back into the framebuffer object.
/// To cause your draw commands to end up in this framebuffer you need to bind it. For that purpose you need to use the
/// ScopedFBOBinding class in combination with this class's GetGLID. Then after you're done rendering you need to unbind the fbo
/// (end the scoped fbo binding's scope). You can then get the information of the texture to which you've rendered by calling GetTextureInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FFGLFBO
{
public:
	FFGLFBO();

	/// Makes the FBO intialise it's OpenGL resources. It'll create the fbo, texture, depth buffer and attach these.
	/// You need to call this function in a function where there's an active OpenGL context, usually your plugin's InitGL or ProcessOpenGL functions.
	///
	/// \param width				The width the framebuffer should have in number of pixels.
	/// \param height				The height the framebuffer should have in number of pixels.
	/// \param internalColorFormat	The OpenGL texture format to which you want to render. See glTexImage2D's internalFormat for possible formats. You cannot specify
	///								compressed formats here.
	bool Initialise( GLsizei width, GLsizei height, GLint internalColorFormat = GL_RGBA8 );
	/// Releases the OpenGL resources. Call this when you're done with the FBO, usually from your plugin's DeInitGL function.
	void Release();

	void ResizeViewPort();

	/// Get the width of the framebuffer in number of pixels.
	GLuint GetWidth() const;
	/// Get the height of the framebuffer in number of pixels.
	GLuint GetHeight() const;
	/// Get the OpenGL handle for the framebuffer. Pass this value into the ScopedFBOBinding to bind this FBO.
	GLuint GetGLID() const;
	/// Get the TextureStruct describing this FBO's texture. If you want to apply this fbo's texture to some surface you
	/// need the information returned from this function.
	FFGLTextureStruct GetTextureInfo() const;

protected:
	bool GenerateFBO();
	bool GenerateDepthBuffer();
	bool GenerateColorTexture();

	GLsizei width;             //!< Width of the fbo in number of pixels. 0 if we're not initialized.
	GLsizei height;            //!< Height of the fbo in number of pixels. 0 if we're not initialized.
	GLint internalColorFormat; //!< The internal format of our texture.

	GLuint fboID;              //!< OpenGL's id for our FBO. 0 if we're not initialized.
	GLuint depthBufferID;      //!< OpenGL's id for our renderbuffer we use to store depth information. 0 if we're not initialized.
	GLuint colorTextureID;     //!< OpenGL's id for our texture we use to store color information. 0 if we're not initialized.
};

}//End namespace ffglex

#endif