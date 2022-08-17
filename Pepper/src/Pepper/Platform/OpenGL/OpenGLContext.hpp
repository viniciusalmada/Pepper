#pragma once

#include "Pepper/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Pepper
{
  class OpenGLContext : public GraphicsContext
  {
  public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

  private:
    GLFWwindow* window_handle;
  };
}