#pragma once

#include "OrthoCamera.hpp"

namespace Pepper
{
  class Renderer2D
  {
  public:
    static void Init(const std::filesystem::path& assetsPath);
    static void Shutdown();

    static void BeginScene(const OrthoCamera& camera);
    static void EndScene();

    // Primitives
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
  };
}