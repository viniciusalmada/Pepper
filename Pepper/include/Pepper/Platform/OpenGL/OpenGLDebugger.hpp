#pragma once
#if !defined(PP_DIST)
  #include <glad/glad.h>

namespace Pepper
{
  void OpenGLDebuggerFunc(GLenum source,
                          GLenum type,
                          unsigned int id,
                          GLenum severity,
                          GLsizei length,
                          const char* message,
                          const void* userParam);
}
#endif