#pragma once

#include "Constants.hpp"

#include <Pepper.hpp>
#include <glm/glm.hpp>

class Planet
{
public:
  void Update(float newXPosition, Pepper::Ref<Pepper::Texture2D> newTexture);

  [[nodiscard]] const glm::vec3& GetPosition() const;

  void OnRendererCall() const;

  [[nodiscard]] float GetRadius() const;

  bool operator==(const Planet& other) const;

private:
  glm::vec3 m_position{ 0.0f, 0.0f, Constants::Z_PLANETS };
  glm::vec2 m_size{ 1.0, 10.0f };
  Pepper::Ref<Pepper::Texture2D> m_texture;
};