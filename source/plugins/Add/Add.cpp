#include <FFGL.h>
#include <FFGLLib.h>

#include "Add.h"
#include "../../lib/ffgl/utilities/utilities.h"

#define FFPARAM_MixVal  (0)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	Add::CreateInstance,				// Create method
	"RM01",								// Plugin unique ID
	"Add",								// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"Blend two videos by adding the first texture to the second texture. Looks like 50Add mixer",// Plugin description
	"Resolume FFGL example by Natspir"	// About
);

static const std::string vertexShaderCode = STRINGIFY(
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0; 
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_FrontColor = gl_Color;
}
);

//This is the shader. It's using to make the transition.
//The code below will be applied by each pixel of your output screen by the Graphic Card
static const std::string fragmentShaderCode = STRINGIFY(

uniform sampler2D textureDest; 
uniform sampler2D textureSrc;
//the value defined by the slider to switch between the two images
uniform float mixVal;


void main()
{
	//get the two fragments to mix
	vec4 colorDest = texture2D(textureDest, gl_TexCoord[0].st);
	vec4 colorSrc = texture2D(textureSrc, gl_TexCoord[1].st);

	//here we add the colorSrc r,g,b,a pixel value to the colorDest pixel value according to the mixVal value
	vec4 mix = colorDest + colorSrc * mixVal;

	//Here we use the built-in function min(val1,val2) to get the minimum between val1 and val2 and always keep output pixel value between 0.0 and 1.0
	gl_FragColor = min(mix, 1.0);
}
);

Add::Add()
:CFreeFrameGLPlugin(),
 m_initResources(1),
 m_inputTextureLocation1(-1),
 m_inputTextureLocation2(-1),
 m_MixValLocation(-1)
{
	// Input properties
	SetMinInputs(2);
	SetMaxInputs(2);

	// Parameters
	SetParamInfo(FFPARAM_MixVal, "Mixer Value", FF_TYPE_STANDARD, 0.5f);
	m_MixVal = 0.5f;


}

Add::~Add()
{
	
}

FFResult Add::InitGL(const FFGLViewportStruct *vp)
{

	m_initResources = 0;


	//initialize gl shader
	m_shader.Compile(vertexShaderCode,fragmentShaderCode);

	//activate our shader
	m_shader.BindShader();

	//to assign values to parameters in the shader, we have to lookup
	//the "location" of each value.. then call one of the glUniform* methods
	//to assign a value
	m_inputTextureLocation1 = m_shader.FindUniform("textureDest");
	m_inputTextureLocation2 = m_shader.FindUniform("textureSrc");
	m_MixValLocation = m_shader.FindUniform("mixVal");

	//the 0 means that the 'inputTexture' in
	//the shader will use the texture bound to GL texture unit 0
	glUniform1i(m_inputTextureLocation1, 0);
	glUniform1i(m_inputTextureLocation2, 1);
	
	m_shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult Add::DeInitGL()
{
  m_shader.FreeGLResources();


  return FF_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Methods
////////////////////////////////////////////////////////////////////////////////////////////////////



FFResult Add::ProcessOpenGL(ProcessOpenGLStruct *pGL)
{
	if (pGL->numInputTextures<2)
		return FF_FAIL;

	if (pGL->inputTextures[0] == NULL)
		return FF_FAIL; 
	if (pGL->inputTextures[1] == NULL)
		return FF_FAIL;

	//activate our shader
	m_shader.BindShader();

	FFGLTextureStruct &TextureDest = *(pGL->inputTextures[0]);
	FFGLTextureStruct &TextureSrc = *(pGL->inputTextures[1]);

	//get the max s,t that correspond to the 
	//width,height of the used portion of the allocated texture space
	FFGLTexCoords maxCoordsDest = GetMaxGLTexCoords(TextureDest);
	FFGLTexCoords maxCoordsSrc = GetMaxGLTexCoords(TextureSrc);

	//assign the mixer value
	glUniform1f(m_MixValLocation, m_MixVal);
	

	//activate texture unit 1 and bind the input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureDest.Handle);
	//activate texture unit 2 and bind the input texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureSrc.Handle);
    
	//draw the quad that will be painted by the shader/textures
	//note that we are sending texture coordinates to texture unit 1..
	//the vertex shader and fragment shader refer to this when querying for
	//texture coordinates of the inputTexture
	glBegin(GL_QUADS);

	//lower left
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
	glMultiTexCoord2f(GL_TEXTURE1, 0, 0);
	glVertex2f(-1,-1);

	//upper left
	glMultiTexCoord2f(GL_TEXTURE0, 0, maxCoordsDest.t);
	glMultiTexCoord2f(GL_TEXTURE1, 0, maxCoordsSrc.t);
	glVertex2f(-1,1);

	//upper right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoordsDest.s, maxCoordsDest.t);
	glMultiTexCoord2f(GL_TEXTURE1, maxCoordsSrc.s, maxCoordsSrc.t);
	glVertex2f(1,1);

	//lower right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoordsDest.s, 0);
	glMultiTexCoord2f(GL_TEXTURE1, maxCoordsSrc.s, 0);
	glVertex2f(1,-1);
	glEnd();

	//unbind the input texture
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind the shader
	m_shader.UnbindShader();

	return FF_SUCCESS;
}

float Add::GetFloatParameter(unsigned int dwIndex)
{
	float retValue = 0.0;

	switch (dwIndex)
	{
	case FFPARAM_MixVal:
		retValue = m_MixVal;
		return retValue;
	default:
		return retValue;
	}
}

FFResult Add::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
	case FFPARAM_MixVal:
		m_MixVal = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
