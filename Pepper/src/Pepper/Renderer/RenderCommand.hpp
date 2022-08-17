#pragma once

#include "RendererAPI.hpp"

namespace Pepper
{
  class RenderCommand
  {
  public:
    static void SetClearColor(const glm::vec4& color) { renderer_api->SetClearColor(color); };

    static void Clear() { renderer_api->Clear(); };

    static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { renderer_api->DrawIndexed(vertexArray); }

  private:
    static RendererAPI* renderer_api;
  };
}
