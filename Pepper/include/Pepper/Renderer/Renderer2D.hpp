#pragma once

#include "OrthoCamera.hpp"
#include "Texture.hpp"

namespace Pepper
{
  class Renderer2D
  {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthoCamera& camera);
    static void EndScene();

    // Primitives
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& tex);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& tex);
  };
}