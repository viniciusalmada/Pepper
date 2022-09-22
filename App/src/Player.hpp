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
  float m_rotation_deg = 0.0f;
  float m_flame_alpha = 1.0f;
  bool m_move = true;

  Pepper::Ref<Pepper::Texture2D> m_rocket_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame1_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame2_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame3_tex;
  
  float m_engine_power = 0.5f;
  float m_gravity = 0.4f;
  glm::vec2 m_rocket_size = { 0.25f, 0.46875f };
  glm::vec2 m_flames_size = { 0.25f, 0.96875f };
};