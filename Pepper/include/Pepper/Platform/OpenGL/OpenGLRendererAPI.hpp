#pragma once

#include "Pepper/Renderer/RendererAPI.hpp"

namespace Pepper
{
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    void Init() override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const override;

    void SetClearColor(const glm::vec4& color) const override;

    void Clear() const override;

    void DrawIndexed(const Ref<VertexArray>&, uint32_t indexCount) const override;
  };
}