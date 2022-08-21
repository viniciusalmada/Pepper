#pragma once

#include <glad/glad.h>

namespace Pepper
{
  class OpenGLDebugger
  {
  public:
    static void Func(GLenum source,
                     GLenum type,
                     unsigned int id,
                     GLenum severity,
                     GLsizei length,
                     const char* message,
                     const void* userParam);
  };
}