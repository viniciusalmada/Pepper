#pragma once

#include "ParticleSystem.hpp"

#include <Pepper.hpp>
#include <glm/glm.hpp>

class Player
{
public:
  Player();

  void Init();

  void OnUpdate(Pepper::TimeStep);

  void OnImGuiLayer();

  void OnRendererCall();

  [[nodiscard]] const glm::vec3& GetPosition() const;

  [[nodiscard]] const std::array<glm::vec4, 4>& GetBoundingBox() const;

  void Reset();

private:
  void LoadAssets();

  void UpdateBoundingBox();

  glm::vec3 m_position{};
  glm::vec2 m_velocity{};
  float m_rotation_deg{};
  float m_flame_alpha{};
  bool m_move{};

  // top-left, top-right, bot-right, bot-left
  std::array<glm::vec4, 4> m_bounding_box{};

  Pepper::Ref<Pepper::Texture2D> m_rocket_tex;
  Pepper::Ref<Pepper::Texture2D> m_flame_tex;

  const float m_engine_power = 30.0f;
  const float m_gravity = 24.0f;
  const glm::vec2 m_rocket_size = { 0.5f, 0.875f };
  const glm::vec2 m_flames_size = { 0.5f, 2.0f };

  float m_time = 0.0f;
  static constexpr float SMOKE_EMIT_INTERVAL = 0.4f;
  static constexpr float FLAME_EMIT_INTERVAL = 0.05f;
  float m_smoke_next_emit_time = SMOKE_EMIT_INTERVAL;
  float m_flame_next_emit_time = FLAME_EMIT_INTERVAL;

  ParticleSystem m_particle_system;
};