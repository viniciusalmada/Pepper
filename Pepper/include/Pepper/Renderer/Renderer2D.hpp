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

    static void Flush();

    // Primitives
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

    static void DrawQuad(const glm::vec3& position,
                         const glm::vec2& size,
                         const Ref<Texture2D>& quadTexture,
                         const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

    static void DrawQuad(const glm::vec2& position,
                         const glm::vec2& size,
                         const Ref<Texture2D>& tex,
                         const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

    static void DrawRotatedQuad(const glm::vec3& position,
                                const glm::vec2& size,
                                float rotationDeg,
                                const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position,
                                const glm::vec2& size,
                                float rotationDeg,
                                const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position,
                                const glm::vec2& size,
                                float rotationDeg,
                                const Ref<Texture2D>& tex,
                                const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawRotatedQuad(const glm::vec2& position,
                                const glm::vec2& size,
                                float rotationDeg,
                                const Ref<Texture2D>& tex,
                                const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
  };
}