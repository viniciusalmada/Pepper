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

private:
  glm::vec2 m_position = { 0.0f, 0.0f };
  glm::vec2 m_velocity = { 0.0f, 0.0f };

  Pepper::Ref<Pepper::Texture2D> m_texture;

  float m_engine_power = 0.1f;
  float m_gravity = 0.8f;
};