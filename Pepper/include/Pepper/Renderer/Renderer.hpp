#pragma once

#include "OrthoCamera.hpp"
#include "RendererAPI.hpp"
#include "Shader.hpp"

namespace Pepper
{
  class Renderer
  {
  public:
    static void Init();
    static void Shutdown();

    static void OnViewportResize(uint32_t width, uint32_t height);

    static void BeginScene(const OrthoCamera& camera);

    static void EndScene();

    static void Submit(const Ref<Shader>& shader,
                       const Ref<VertexArray>& vertexArray,
                       const glm::mat4& transform = glm::mat4{ 1.0f });

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  private:
    DECLARE_PIMPL
  };
}