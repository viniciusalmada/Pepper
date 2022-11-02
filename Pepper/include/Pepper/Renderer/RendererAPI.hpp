#pragma once

#include "VertexArray.hpp"

#include <glm/glm.hpp>

namespace Pepper
{
  class RendererAPI
  {
  public:
    enum class API : unsigned int { NONE = 0, OPEN_GL = 1 };

  public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const = 0;

    virtual void SetClearColor(const glm::vec4& color) const = 0;

    virtual void Clear() const = 0;

    virtual void DrawIndexed(const Ref<VertexArray>&, uint32_t) const = 0;

    static API GetAPI() { return api; }

  private:
    static API api;
  };
}