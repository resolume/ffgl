#include "FFGLShader.h"
#include <stdio.h>

#define LOGSHADERERRORS

FFGLShader::FFGLShader() :
	m_linkStatus(0),
	m_glProgram(0),
	m_glVertexShader(0),
	m_glFragmentShader(0)

{
}

void FFGLShader::CreateGLResources()
{
	if (m_glProgram==0)
		m_glProgram = glCreateProgram();

	if (m_glVertexShader==0)
		m_glVertexShader = glCreateShader(GL_VERTEX_SHADER_ARB);

	if (m_glFragmentShader==0)
		m_glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
}

void FFGLShader::FreeGLResources()
{
	if (m_glFragmentShader)
	{
		glDeleteShader(m_glFragmentShader);
		m_glFragmentShader = 0;
	}

	if (m_glVertexShader)
	{
		glDeleteShader(m_glVertexShader);
		m_glVertexShader = 0;
	}

	if (m_glProgram)
	{
		glDeleteProgram(m_glProgram);
		m_glProgram = 0;
	}
}

int FFGLShader::BindShader()
{
	if (m_glProgram==0)
		return 0;

	glUseProgram(m_glProgram);

	return 1;
}

int FFGLShader::UnbindShader()
{
	glUseProgram(0);

	return 1;
}

FFGLShader::~FFGLShader()
{
}

int FFGLShader::IsReady()
{
	return (m_glProgram!=0 && m_glVertexShader!=0 && m_glFragmentShader!=0 && m_linkStatus==1);
}


int FFGLShader::Compile(const std::string& vtxProgram, const std::string& fragProgram)
{
	return Compile(vtxProgram.c_str(), fragProgram.c_str());
}

int FFGLShader::Compile(const char *vtxProgram, const char *fragProgram)
{
  if (m_glProgram==0)
	  CreateGLResources();
  
  int doLink = 0;

  //if we can compile a fragment shader, do it.
  if (m_glFragmentShader !=0 &&
      m_glProgram !=0 &&
      fragProgram != NULL &&
	  fragProgram[0] !=0)
  {
    const char *strings[] =
    {
      fragProgram,
      NULL
    };

    // Load Shader Sources
    glShaderSource(m_glFragmentShader, 1, strings, NULL);

    // Compile The Shaders
    glCompileShader(m_glFragmentShader);
    
    GLint compileSuccess;
    glGetShaderiv(m_glFragmentShader, GL_COMPILE_STATUS, &compileSuccess);

    if (compileSuccess == GL_TRUE)
    {
		//attach it to the program
		glAttachShader(m_glProgram, m_glFragmentShader);
		doLink = 1;
    }
    else
    {
		//get the log so we can peek at the error string
		char log[1024];
		GLsizei returnedLength;

		glGetProgramInfoLog(m_glFragmentShader, sizeof(log)-1, &returnedLength, log);
		log[returnedLength] = 0;

		#ifdef LOGSHADERERRORS
		printf( "Fragment Shader error: %s \n", log );
		#endif
		int a;
		a=0;
    }
  }

	//if we can compile a vertex shader, do it
	if (m_glVertexShader!=0 && m_glProgram!=0 && vtxProgram!=0 && vtxProgram[0]!=0)
	{
		const char *strings[] =
		{
			vtxProgram,
			NULL
		};

		// Load Shader Sources
		glShaderSource(m_glVertexShader, 1, strings, NULL);

		// Compile The Shaders
		glCompileShader(m_glVertexShader);

		GLint compileSuccess;

		glGetShaderiv(m_glVertexShader, GL_COMPILE_STATUS, &compileSuccess);

		if (compileSuccess == GL_TRUE)
		{
			//attach it to the program
			glAttachShader(m_glProgram, m_glVertexShader);
			doLink = 1;
		} 
		else
		{
			//get the log so we can peek at the error string
			char log[1024];
			GLsizei returnedLength;
			glGetProgramInfoLog(m_glVertexShader, sizeof(log)-1, &returnedLength, log);
			log[returnedLength] = 0;
			int a;
			a=0;

			#ifdef LOGSHADERERRORS
			printf( "Vertex Shader error: %s \n", log );
			#endif
		}     
	}
  
  //check if linking worked
  GLint linkSuccess = 0;
  if (doLink)
  {
	  // Link The Program Object
	  glLinkProgram(m_glProgram);
	  glGetProgramiv(m_glProgram, GL_LINK_STATUS, &linkSuccess);
  }
  
  m_linkStatus = linkSuccess;
  return linkSuccess;
}

GLuint FFGLShader::FindUniform(const char *name)
{
	return glGetUniformLocation(m_glProgram,name);
}
