#pragma once

#include "Pepper/Core/Window.hpp"
#include "Pepper/Renderer/GraphicsContext.hpp"

namespace Pepper
{
  class OpenGLContext : public GraphicsContext
  {
  public:
    OpenGLContext(Window& window);
    ~OpenGLContext();

    void Init() override;
    void SwapBuffers() override;

  private:
    PIMPL()
  };
}