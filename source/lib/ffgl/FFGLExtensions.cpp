#include <FFGL.h>
#include <FFGLExtensions.h>
#include <stdlib.h>
#include <string.h>

#ifdef TARGET_OS_MAC
//for the NS* calls in GetProcAddress
#include <mach-o/dyld.h>
#endif
 
#ifdef __linux__
#include <GL/glx.h>
#endif

FFGLExtensions::FFGLExtensions()
{
  memset(this, 0, sizeof(FFGLExtensions));
}

void FFGLExtensions::Initialize()
{
#ifdef _WIN32
  InitWGLEXTSwapControl();
#endif

  InitMultitexture();
  InitARBShaderObjects();
  InitEXTFramebufferObject();
}

void *FFGLExtensions::GetProcAddress(const char *name)
{

#ifdef _WIN32

  void *result = wglGetProcAddress(name);

  if (result!=NULL)
    return result;
    
#else

#ifdef TARGET_OS_MAC
  
  // Prepend a '_' for the Unix C symbol mangling convention
  int symbolLength = strlen(name) + 2; //1 for the _, another for the trailing null
  char symbolName[1024];
  if (symbolLength>sizeof(symbolName))
  {
    //symbol name too long;
    throw;
    return NULL;
  }
  
  strcpy(symbolName + 1, name);
  symbolName[0] = '_';

  NSSymbol symbol = NULL;
  
  if (NSIsSymbolNameDefined(symbolName))
    symbol = NSLookupAndBindSymbol(symbolName);
  
  if (symbol!=NULL)
  {
    return NSAddressOfSymbol(symbol);
  }

#else

#ifdef __linux__

  void *result = (void *)glXGetProcAddress((const GLubyte *)name);

  if (result!=NULL)
    return result;

#else
  
#error Define this for your OS
  
#endif
#endif
#endif
  throw;//this will be caught by one of the Init() functions below
  return NULL;
}

void FFGLExtensions::InitMultitexture()
{
  try
  {  
  glActiveTexture = (glActiveTexturePROC)GetProcAddress("glActiveTexture");
  glClientActiveTexture = (glClientActiveTexturePROC)GetProcAddress("glClientActiveTexture");

  glMultiTexCoord1d = (glMultiTexCoord1dPROC)GetProcAddress("glMultiTexCoord1d");
  glMultiTexCoord1dv = (glMultiTexCoord1dvPROC)GetProcAddress("glMultiTexCoord1dv");
  glMultiTexCoord1f = (glMultiTexCoord1fPROC)GetProcAddress("glMultiTexCoord1f");
  glMultiTexCoord1fv = (glMultiTexCoord1fvPROC)GetProcAddress("glMultiTexCoord1fv");
  glMultiTexCoord1i = (glMultiTexCoord1iPROC)GetProcAddress("glMultiTexCoord1i");
  glMultiTexCoord1iv = (glMultiTexCoord1ivPROC)GetProcAddress("glMultiTexCoord1iv");
  glMultiTexCoord1s = (glMultiTexCoord1sPROC)GetProcAddress("glMultiTexCoord1s");
  glMultiTexCoord1sv = (glMultiTexCoord1svPROC)GetProcAddress("glMultiTexCoord1sv");

  glMultiTexCoord2d = (glMultiTexCoord2dPROC)GetProcAddress("glMultiTexCoord2d");
  glMultiTexCoord2dv = (glMultiTexCoord2dvPROC)GetProcAddress("glMultiTexCoord2dv");
  glMultiTexCoord2f = (glMultiTexCoord2fPROC)GetProcAddress("glMultiTexCoord2f");
  glMultiTexCoord2fv = (glMultiTexCoord2fvPROC)GetProcAddress("glMultiTexCoord2fv");
  glMultiTexCoord2i = (glMultiTexCoord2iPROC)GetProcAddress("glMultiTexCoord2i");
  glMultiTexCoord2iv = (glMultiTexCoord2ivPROC)GetProcAddress("glMultiTexCoord2iv");
  glMultiTexCoord2s = (glMultiTexCoord2sPROC)GetProcAddress("glMultiTexCoord2s");
  glMultiTexCoord2sv = (glMultiTexCoord2svPROC)GetProcAddress("glMultiTexCoord2sv");

  glMultiTexCoord3d = (glMultiTexCoord3dPROC)GetProcAddress("glMultiTexCoord3d");
  glMultiTexCoord3dv = (glMultiTexCoord3dvPROC)GetProcAddress("glMultiTexCoord3dv");
  glMultiTexCoord3f = (glMultiTexCoord3fPROC)GetProcAddress("glMultiTexCoord3f");
  glMultiTexCoord3fv = (glMultiTexCoord3fvPROC)GetProcAddress("glMultiTexCoord3fv");
  glMultiTexCoord3i = (glMultiTexCoord3iPROC)GetProcAddress("glMultiTexCoord3i");
  glMultiTexCoord3iv = (glMultiTexCoord3ivPROC)GetProcAddress("glMultiTexCoord3iv");
  glMultiTexCoord3s = (glMultiTexCoord3sPROC)GetProcAddress("glMultiTexCoord3s");
  glMultiTexCoord3sv = (glMultiTexCoord3svPROC)GetProcAddress("glMultiTexCoord3sv");

  glMultiTexCoord4d = (glMultiTexCoord4dPROC)GetProcAddress("glMultiTexCoord4d");
  glMultiTexCoord4dv = (glMultiTexCoord4dvPROC)GetProcAddress("glMultiTexCoord4dv");
  glMultiTexCoord4f = (glMultiTexCoord4fPROC)GetProcAddress("glMultiTexCoord4f");
  glMultiTexCoord4fv = (glMultiTexCoord4fvPROC)GetProcAddress("glMultiTexCoord4fv");
  glMultiTexCoord4i = (glMultiTexCoord4iPROC)GetProcAddress("glMultiTexCoord4i");
  glMultiTexCoord4iv = (glMultiTexCoord4ivPROC)GetProcAddress("glMultiTexCoord4iv");
  glMultiTexCoord4s = (glMultiTexCoord4sPROC)GetProcAddress("glMultiTexCoord4s");
  glMultiTexCoord4sv = (glMultiTexCoord4svPROC)GetProcAddress("glMultiTexCoord4sv");
  }
  catch (...)
  {
    //not supported
    multitexture = 0;
    return;
  }
  
  //if we get this far w/no exceptions, ARB_shader_objects shoudl be fully
  //supported
  multitexture = 1;
}

void FFGLExtensions::InitARBShaderObjects()
{
  try
  {

  glDeleteObjectARB = (glDeleteObjectARBPROC)GetProcAddress("glDeleteObjectARB");
  glGetHandleARB = (glGetHandleARBPROC)GetProcAddress("glGetHandleARB");
  glDetachObjectARB = (glDetachObjectARBPROC)GetProcAddress("glDetachObjectARB");
  glCreateShaderObjectARB = (glCreateShaderObjectARBPROC)GetProcAddress("glCreateShaderObjectARB");
  glShaderSourceARB = (glShaderSourceARBPROC)GetProcAddress("glShaderSourceARB");
  glCompileShaderARB = (glCompileShaderARBPROC)GetProcAddress("glCompileShaderARB");
  glCreateProgramObjectARB = (glCreateProgramObjectARBPROC)GetProcAddress("glCreateProgramObjectARB");
  glAttachObjectARB = (glAttachObjectARBPROC)GetProcAddress("glAttachObjectARB");
  glLinkProgramARB = (glLinkProgramARBPROC)GetProcAddress("glLinkProgramARB");
  glUseProgramObjectARB = (glUseProgramObjectARBPROC)GetProcAddress("glUseProgramObjectARB");
  glValidateProgramARB = (glValidateProgramARBPROC)GetProcAddress("glValidateProgramARB");
  glUniform1fARB = (glUniform1fARBPROC)GetProcAddress("glUniform1fARB");
  glUniform2fARB = (glUniform2fARBPROC)GetProcAddress("glUniform2fARB");
  glUniform3fARB = (glUniform3fARBPROC)GetProcAddress("glUniform3fARB");
  glUniform4fARB = (glUniform4fARBPROC)GetProcAddress("glUniform4fARB");
  glUniform1iARB = (glUniform1iARBPROC)GetProcAddress("glUniform1iARB");
  glUniform2iARB = (glUniform2iARBPROC)GetProcAddress("glUniform2iARB");
  glUniform3iARB = (glUniform3iARBPROC)GetProcAddress("glUniform3iARB");
  glUniform4iARB = (glUniform4iARBPROC)GetProcAddress("glUniform4iARB");
  glUniform1fvARB = (glUniform1fvARBPROC)GetProcAddress("glUniform1fvARB");
  glUniform2fvARB = (glUniform2fvARBPROC)GetProcAddress("glUniform2fvARB");
  glUniform3fvARB = (glUniform3fvARBPROC)GetProcAddress("glUniform3fvARB");
  glUniform4fvARB = (glUniform4fvARBPROC)GetProcAddress("glUniform4fvARB");
  glUniform1ivARB = (glUniform1ivARBPROC)GetProcAddress("glUniform1ivARB");
  glUniform2ivARB = (glUniform2ivARBPROC)GetProcAddress("glUniform2ivARB");
  glUniform3ivARB = (glUniform3ivARBPROC)GetProcAddress("glUniform3ivARB");
  glUniform4ivARB = (glUniform4ivARBPROC)GetProcAddress("glUniform4ivARB");
  glUniformMatrix2fvARB = (glUniformMatrix2fvARBPROC)GetProcAddress("glUniformMatrix2fvARB");
  glUniformMatrix3fvARB = (glUniformMatrix3fvARBPROC)GetProcAddress("glUniformMatrix3fvARB");
  glUniformMatrix4fvARB = (glUniformMatrix4fvARBPROC)GetProcAddress("glUniformMatrix4fvARB");
  glGetObjectParameterfvARB = (glGetObjectParameterfvARBPROC)GetProcAddress("glGetObjectParameterfvARB");
  glGetObjectParameterivARB = (glGetObjectParameterivARBPROC)GetProcAddress("glGetObjectParameterivARB");
  glGetInfoLogARB = (glGetInfoLogARBPROC)GetProcAddress("glGetInfoLogARB");
  glGetAttachedObjectsARB = (glGetAttachedObjectsARBPROC)GetProcAddress("glGetAttachedObjectsARB");
  glGetUniformLocationARB = (glGetUniformLocationARBPROC)GetProcAddress("glGetUniformLocationARB");
  glGetActiveUniformARB = (glGetActiveUniformARBPROC)GetProcAddress("glGetActiveUniformARB");
  glGetUniformfvARB = (glGetUniformfvARBPROC)GetProcAddress("glGetUniformfvARB");
  glGetUniformivARB = (glGetUniformivARBPROC)GetProcAddress("glGetUniformivARB");
  glGetShaderSourceARB = (glGetShaderSourceARBPROC)GetProcAddress("glGetShaderSourceARB");

  }
  catch (...)
  {
    //not supported
    ARB_shader_objects = 0;
    return;
  }

  //if we get this far w/no exceptions, ARB_shader_objects shoudl be fully
  //supported
  ARB_shader_objects = 1;
}

void FFGLExtensions::InitEXTFramebufferObject()
{
  try
  {

  glBindFramebufferEXT = (glBindFramebufferEXTPROC)GetProcAddress("glBindFramebufferEXT");
  glBindRenderbufferEXT = (glBindRenderbufferEXTPROC)GetProcAddress("glBindRenderbufferEXT");
  glCheckFramebufferStatusEXT = (glCheckFramebufferStatusEXTPROC)GetProcAddress("glCheckFramebufferStatusEXT");
  glDeleteFramebuffersEXT = (glDeleteFramebuffersEXTPROC)GetProcAddress("glDeleteFramebuffersEXT");
  glDeleteRenderBuffersEXT = (glDeleteRenderBuffersEXTPROC)GetProcAddress("glDeleteRenderbuffersEXT");
  glFramebufferRenderbufferEXT = (glFramebufferRenderbufferEXTPROC)GetProcAddress("glFramebufferRenderbufferEXT");
  glFramebufferTexture1DEXT = (glFramebufferTexture1DEXTPROC)GetProcAddress("glFramebufferTexture1DEXT");
  glFramebufferTexture2DEXT = (glFramebufferTexture2DEXTPROC)GetProcAddress("glFramebufferTexture2DEXT");
  glFramebufferTexture3DEXT = (glFramebufferTexture3DEXTPROC)GetProcAddress("glFramebufferTexture3DEXT");
  glGenFramebuffersEXT = (glGenFramebuffersEXTPROC)GetProcAddress("glGenFramebuffersEXT");
  glGenRenderbuffersEXT = (glGenRenderbuffersEXTPROC)GetProcAddress("glGenRenderbuffersEXT");
  glGenerateMipmapEXT = (glGenerateMipmapEXTPROC)GetProcAddress("glGenerateMipmapEXT");
  glGetFramebufferAttachmentParameterivEXT = (glGetFramebufferAttachmentParameterivEXTPROC)GetProcAddress("glGetFramebufferAttachmentParameterivEXT");
  glGetRenderbufferParameterivEXT = (glGetRenderbufferParameterivEXTPROC)GetProcAddress("glGetRenderbufferParameterivEXT");
  glIsFramebufferEXT = (glIsFramebufferEXTPROC)GetProcAddress("glIsFramebufferEXT");
  glIsRenderbufferEXT = (glIsRenderbufferEXTPROC)GetProcAddress("glIsRenderbufferEXT");
  glRenderbufferStorageEXT = (glRenderbufferStorageEXTPROC)GetProcAddress("glRenderbufferStorageEXT");

  }
  catch (...)
  {
    //not supported
    EXT_framebuffer_object = 0;
    return;
  }

  EXT_framebuffer_object = 1;
}

#ifdef _WIN32
void FFGLExtensions::InitWGLEXTSwapControl()
{
  try
  {
  wglSwapIntervalEXT = (wglSwapIntervalEXTPROC) GetProcAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (wglGetSwapIntervalEXTPROC) GetProcAddress("wglGetSwapIntervalEXT");
  }
  catch (...)
  {
    //not supported
    WGL_EXT_swap_control = 0;
    return;
  }

  WGL_EXT_swap_control = 1;
}
#endif
