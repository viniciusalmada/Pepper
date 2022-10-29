#include "ParticleSystem.hpp"

#include "Color.hpp"
#include "Constants.hpp"
#include "Random.hpp"
#include "Utilities.hpp"
#include "glm/gtx/compatibility.hpp"

constexpr auto PARTICLES_PER_FRAME = 1;

ParticleSystem::ParticleSystem()
{
  m_index = 0;
}
void ParticleSystem::Emit(uint32_t count,
                          const glm::vec2& position,
                          const glm::vec4& colorBegin,
                          const glm::vec4& colorEnd)
{
  Utils::Repeat(count,
                [&]
                {
                  Particle& curr_par = m_particles_pool[m_index++];
                  if (m_index == m_particles_pool.size())
                    m_index = 0u;
                  if (curr_par.active)
                    return false;

                  curr_par.position = position;
                  curr_par.velocity = { Random::Float(-5.0f, 0.0f), Random::Float(-2.0f, 2.0f) };
                  curr_par.color_begin = colorBegin;
                  curr_par.color_end = colorEnd;
                  curr_par.angle_deg = Random::Float(0.0f, 360.0f);
                  curr_par.size_begin = Random::Float(0.1f, 0.4f);
                  curr_par.size_end = 0.0f;
                  curr_par.life_time = Random::Float(0.2f, 2.0f);
                  curr_par.life_remaining = curr_par.life_time;
                  curr_par.active = true;
                  return true;
                });
}
void ParticleSystem::Update(Pepper::TimeStep ts)
{
  for (auto& particle : m_particles_pool)
  {
    if (!particle.active)
      continue;

    if (particle.life_remaining <= 0.0f)
    {
      particle.active = false;
      continue;
    }

    particle.angle_deg += 100.0f * ts;
    particle.life_remaining -= ts;
    particle.position.x += particle.velocity.x * ts;
    particle.position.y += particle.velocity.y * ts;
  }
}
void ParticleSystem::OnRendererCall()
{
  for (auto& particle : m_particles_pool)
  {
    if (!particle.active)
      continue;

    float life = particle.life_remaining / particle.life_time;
    glm::vec4 color = glm::lerp(particle.color_end, particle.color_begin, life);
    color.a = color.a * life;

    float size = glm::lerp(std::min(particle.size_begin, particle.size_end),
                           std::max(particle.size_begin, particle.size_end),
                           life);
    Pepper::Renderer2D::DrawRotatedQuad(
      { particle.position.x, particle.position.y, Constants::Z_PARTICLES },
      { size, size },
      particle.angle_deg,
      color);
  }
}

void ParticleSystem::Clear()
{
  std::ranges::for_each(m_particles_pool, [](Particle& par) { par.active = false; });
  m_index = 0;
}
