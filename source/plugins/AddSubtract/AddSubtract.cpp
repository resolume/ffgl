#include <FFGL.h>
#include <FFGLLib.h>
#include "AddSubtract.h"
#include "../../ffgl/utilities/utilities.h"

#define FFPARAM_BrightnessR  (0)
#define FFPARAM_BrightnessG	 (1)
#define FFPARAM_BrightnessB	 (2)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"RE01",								// Plugin unique ID
	"Add Subtract",						// Plugin name											
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"Add and Subtract colours",			// Plugin description
	"Resolume FFGL Example"				// About
);

char *vertexShaderCode = STRINGIFY(
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	
}
);


char *fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform vec3 brightness;
void main()
{
	vec4 color = texture2D(inputTexture,gl_TexCoord[0].st);
	if (color.a > 0.0) //unpremultiply
		color.rgb /= color.a;
	color.rgb = color.rgb + brightness;
	color.rgb *= color.a; //premultiply
	gl_FragColor  =  color;
}
);

AddSubtract::AddSubtract()
:CFreeFrameGLPlugin(),
 m_initResources(1),
 m_inputTextureLocation(-1),
 m_BrightnessLocation(-1)
{
	// Input properties
	SetMinInputs(1);
	SetMaxInputs(1);

	// Parameters
	SetParamInfo(FFPARAM_BrightnessR, "R", FF_TYPE_STANDARD, 0.5f);
	m_BrightnessR = 0.5f;

	SetParamInfo(FFPARAM_BrightnessG, "G", FF_TYPE_STANDARD, 0.5f);
	m_BrightnessG = 0.5f;
	
	SetParamInfo(FFPARAM_BrightnessB, "B", FF_TYPE_STANDARD, 0.5f);
	m_BrightnessB = 0.5f;

}

AddSubtract::~AddSubtract()
{
	
}

FFResult AddSubtract::InitGL(const FFGLViewportStruct *vp)
{

	m_initResources = 0;

	//initialize gl extensions and
	//make sure required features are supported
	m_extensions.Initialize();
	if (m_extensions.multitexture==0 || m_extensions.ARB_shader_objects==0)
	  return FF_FAIL;

	//initialize gl shader
	m_shader.SetExtensions(&m_extensions);
	m_shader.Compile(vertexShaderCode,fragmentShaderCode);

	//activate our shader
	m_shader.BindShader();

	//to assign values to parameters in the shader, we have to lookup
	//the "location" of each value.. then call one of the glUniform* methods
	//to assign a value
	m_inputTextureLocation = m_shader.FindUniform("inputTexture");
	m_BrightnessLocation = m_shader.FindUniform("brightness");

	//the 0 means that the 'inputTexture' in
	//the shader will use the texture bound to GL texture unit 0
	m_extensions.glUniform1iARB(m_inputTextureLocation, 0);
	
	m_shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult AddSubtract::DeInitGL()
{
  m_shader.FreeGLResources();


  return FF_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Methods
////////////////////////////////////////////////////////////////////////////////////////////////////



FFResult AddSubtract::ProcessOpenGL(ProcessOpenGLStruct *pGL)
{
	if (pGL->numInputTextures<1) return FF_FAIL;

	if (pGL->inputTextures[0]==NULL) return FF_FAIL;

	//activate our shader
	m_shader.BindShader();

	FFGLTextureStruct &Texture = *(pGL->inputTextures[0]);

	//get the max s,t that correspond to the 
	//width,height of the used portion of the allocated texture space
	FFGLTexCoords maxCoords = GetMaxGLTexCoords(Texture);

	//assign the Brightness
	m_extensions.glUniform3fARB(m_BrightnessLocation, 
								-1.0f + (m_BrightnessR * 2.0f),
								-1.0f + (m_BrightnessG * 2.0f),
								-1.0f + (m_BrightnessB * 2.0f)		
								);
	

	//activate texture unit 1 and bind the input texture
	m_extensions.glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture.Handle);
    
	//draw the quad that will be painted by the shader/textures
	//note that we are sending texture coordinates to texture unit 1..
	//the vertex shader and fragment shader refer to this when querying for
	//texture coordinates of the inputTexture
	glBegin(GL_QUADS);

	//lower left
	m_extensions.glMultiTexCoord2f(GL_TEXTURE0, 0,0);
	glVertex2f(-1,-1);

	//upper left
	m_extensions.glMultiTexCoord2f(GL_TEXTURE0, 0, maxCoords.t);
	glVertex2f(-1,1);

	//upper right
	m_extensions.glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, maxCoords.t);
	glVertex2f(1,1);

	//lower right
	m_extensions.glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, 0);
	glVertex2f(1,-1);
	glEnd();

	//unbind the input texture
	glBindTexture(GL_TEXTURE_2D,0);


	//unbind the shader
	m_shader.UnbindShader();

	return FF_SUCCESS;
}

float AddSubtract::GetFloatParameter(unsigned int dwIndex)
{
	float retValue = 0.0;

	switch (dwIndex)
	{
	case FFPARAM_BrightnessR:
		retValue = m_BrightnessR;
		return retValue;
	case FFPARAM_BrightnessG:
		retValue = m_BrightnessG;
		return retValue;
	case FFPARAM_BrightnessB:
		retValue = m_BrightnessB;
		return retValue;
	default:
		return retValue;
	}
}

FFResult AddSubtract::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
	case FFPARAM_BrightnessR:
		m_BrightnessR = value;
		break;
	case FFPARAM_BrightnessG:
		m_BrightnessG = value;
		break;
	case FFPARAM_BrightnessB:
		m_BrightnessB = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
