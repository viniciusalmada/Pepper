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
    virtual void SetClearColor(const glm::vec4& color) const = 0;

    virtual void Clear() const = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>&) const = 0;

    static API GetAPI() { return api; }

  private:
    static API api;
  };
}