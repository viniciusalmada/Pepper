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

    static void BeginScene(OrthoCamera& camera);

    static void EndScene();

    static void Submit(const Ref<Shader>& shader,
                       const Ref<VertexArray>& vertexArray,
                       const glm::mat4& transform = glm::mat4{ 1.0f });

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  private:
    struct SceneData
    {
      glm::mat4 view_proj_matrix;
    };

    static SceneData* scene_data;
  };
}