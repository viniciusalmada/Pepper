#include "StarsSystem.hpp"

#include "Color.hpp"
#include "Constants.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

auto GetStarColorBeginEnd()
{
  auto grey_beg = Random::Float(0.3f, 1.0f);
  auto grey_end = Random::Float(0.3f, 1.0f);
  auto grey_color = glm::vec4{ grey_beg, grey_beg, grey_beg, 0.5f };

  auto rgb_color = glm::vec4{ Random::Float(0.0f, 1.0f),
                              Random::Float(0.0f, 1.0f),
                              Random::Float(0.0f, 1.0f),
                              0.5f };
  if (Random::Int(0, 1))
    return std::make_tuple(rgb_color, glm::vec4{ grey_end, grey_end, grey_end, 0.5f });
  else
    return std::make_tuple(glm::vec4{ grey_end, grey_end, grey_end, 0.5f }, rgb_color);
}

StarsSystem::StarsSystem()
{
  m_index = 0;
}
void StarsSystem::Create(const glm::vec2& positionStart, const glm::vec2& positionEnd)
{
  Utils::Repeat(m_star_pool.size(),
                [&]
                {
                  auto& curr_star = m_star_pool[m_index++];

                  curr_star.position.x = Random::Float(positionStart.x, positionEnd.x);
                  curr_star.position.y = Random::Float(-10.0f, 10.0f);
                  std::tie(curr_star.color_1, curr_star.color_2) = GetStarColorBeginEnd();
                  curr_star.angle_deg = Random::Float(0.0f, 360.0f);
                  curr_star.size = Random::Float(0.05f, 0.1f);

                  return true;
                });
}

void StarsSystem::Update(float startLimit, Pepper::TimeStep ts)
{
  m_color_swap_ts += ts;

  auto is_outside = [&](Star& star) { return star.position.x < startLimit; };

  for (auto& star : m_star_pool | std::views::filter(is_outside))
  {
    star.position.x += 20.0f;
    std::tie(star.color_1, star.color_2) = GetStarColorBeginEnd();
    star.angle_deg = Random::Float(0.0f, 360.0f);
    star.size = Random::Float(0.05f, 0.1f);
  }

  if (m_color_swap_ts >= COLOR_SWAP_INTERVAL)
  {
    for (auto& star : m_star_pool)
    {
      std::swap(star.color_1, star.color_2);
    }
    m_color_swap_ts = 0.0f;
  }
}
void StarsSystem::OnRendererCall()
{
  for (auto& star : m_star_pool)
  {
    Pepper::Renderer2D::DrawRotatedQuad({ star.position.x, star.position.y, Constants::Z_STARS },
                                        { star.size, star.size },
                                        star.angle_deg,
                                        star.color_1);
  }
}
void StarsSystem::Clear()
{
  m_index = 0;
}
