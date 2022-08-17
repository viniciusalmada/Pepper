#pragma once

#include "Pepper/Renderer/RendererAPI.hpp"

namespace Pepper
{
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    void SetClearColor(const glm::vec4& color) const override;

    void Clear() const override;

    void DrawIndexed(const std::shared_ptr<VertexArray>&) const override;
  };
}