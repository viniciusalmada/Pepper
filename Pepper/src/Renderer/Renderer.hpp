#pragma once

#include "RendererAPI.hpp"

namespace Pepper
{
  class Renderer
  {
  public:
    static void BeginScene(); // TODO
    
    static void EndScene(); // TODO

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
  };
}