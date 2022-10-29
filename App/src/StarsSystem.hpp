#ifndef PEPPERENGINE_STARSSYSTEM_HPP
#define PEPPERENGINE_STARSSYSTEM_HPP

#include <Pepper.hpp>

class StarsSystem
{
public:
  StarsSystem();

  void Create(const glm::vec2& positionStart, const glm::vec2& positionEnd);

  void Update(float startLimit, Pepper::TimeStep ts);

  void OnRendererCall();

  void Clear();

private:
  struct Star
  {
    glm::vec2 position;
    glm::vec4 color_1, color_2;
    float angle_deg = 0.0f;
    float size;
  };

  std::array<Star, 50> m_star_pool;
  uint32_t m_index;
  float m_color_swap_ts = 0.0f;
  static constexpr float COLOR_SWAP_INTERVAL = 2.0f;
};

#endif // PEPPERENGINE_STARSSYSTEM_HPP
