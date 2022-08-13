#pragma once

namespace Pepper
{
  enum class RendererAPI : unsigned int { NONE = 0, OPEN_GL = 1 };

  class Renderer
  {
  public:
    static RendererAPI GetAPI() { return renderer_api; }

  private:
    static RendererAPI renderer_api;
  };
}