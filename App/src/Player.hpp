#pragma once

#include <Pepper.hpp>
#include <glm/glm.hpp>

class Player
{
public:
  Player();

  void LoadAssets();

  void OnUpdate(Pepper::TimeStep);

  void OnImGuiLayer();

  void OnRendererCall();

  const glm::vec3& GetPosition() const;

private:
  void UpdateBoundingBox();

  glm::vec3 m_position = { 0.0f, 0.0f, 0.7f };
  glm::vec2 m_velocity = { 2.0f, 0.0f };
  float m_rotation_deg = 0.0f;
  float m_flame_alpha = 1.0f;
  bool m_move = false;

  // top-left, top-right, bot-right, bot-left
  std::array<glm::vec4, 4> m_bounding_box;

  Pepper::Ref<Pepper::Texture2D> m_rocket_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame1_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame2_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame3_tex;

  float m_engine_power = 30.0f;
  float m_gravity = 24.0f;
  glm::vec2 m_rocket_size = { 0.5f, 0.875f };
  glm::vec2 m_flames_size = { 0.5f, 2.0f };
};