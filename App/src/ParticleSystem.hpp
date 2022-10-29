#ifndef PEPPER_PARTICLESYSTEM_HPP
#define PEPPER_PARTICLESYSTEM_HPP

#include <Pepper.hpp>

class ParticleSystem
{
public:
  ParticleSystem();

  void Emit(uint32_t count,
            const glm::vec2& position,
            const glm::vec4& colorBegin,
            const glm::vec4& colorEnd);

  void Update(Pepper::TimeStep ts);

  void OnRendererCall();

  void Clear();

private:
  struct Particle
  {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color_begin, color_end;
    float angle_deg = 0.0f;
    float size_begin, size_end;

    float life_time = 1.0f;
    float life_remaining = 0.0f;

    bool active = false;
  };

  std::array<Particle, 1000> m_particles_pool;
  decltype(m_particles_pool.size()) m_index;
};

#endif // PEPPER_PARTICLESYSTEM_HPP
