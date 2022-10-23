#include "GridVoronoi.hpp"

#include "Random.hpp"

#include <set>

namespace
{
  const Color red{ 1.0f, 0.2f, 0.2f };
  const Color green{ 0.2f, 1.0f, 0.2f };
  const Color blue{ 0.2f, 0.2f, 1.0f };
}

Point GetNearestPoint(const std::set<Point, PointsCmp>& pts, Point pt)
{
  auto min_distance = std::numeric_limits<decltype(pt.x)>::max();
  Point result{};
  for (const auto& _pt : pts)
  {
    auto dist = glm::distance(_pt, pt);
    if (dist < min_distance)
    {
      min_distance = dist;
      result = _pt;
    }
  }
  return result;
}

GridVoronoi::GridVoronoi(uint32_t width, uint32_t height, std::set<Point, PointsCmp> points) :
    m_width(width),
    m_height(height),
    m_points({}),
    m_divisors(1)
{
  for (const auto& pt : points)
  {
    m_colors[pt] = { Random::Float(0.0, 1.0), Random::Float(0.0, 1.0), Random::Float(0.0, 1.0) };
  }

  Update(m_divisors, std::set<glm::dvec2, PointsCmp>());
}
void GridVoronoi::OnParticle(const std::function<void(const Point&, const Color&)>& action) const
{
  for (const auto& [pt, color, i] : m_particles)
  {
    action(pt, color);
  }
}
void GridVoronoi::Update(int divisors, std::set<Point, PointsCmp> pts)
{
  m_particles.clear();
  m_points = pts;
  m_divisors = divisors;

  for (const auto& pt : m_points)
  {
    m_colors[pt] = { Random::Float(0.0, 1.0), Random::Float(0.0, 1.0), Random::Float(0.0, 1.0) };
  }

  auto columns_count = m_width / m_divisors;
  auto row_count = m_height / m_divisors;

  for (auto r : std::views::iota(0u, row_count + 1))
  {
    for (auto c : std::views::iota(0u, columns_count + 1))
    {
      auto w = c * m_divisors;
      auto h = r * m_divisors;
      Point pt{ w, h };
      const auto& nearest = GetNearestPoint(pts, pt);
      m_particles.emplace_back(pt, m_colors[nearest], static_cast<int>(std::hash<float>{}(m_colors[nearest].r)));
    }
  }
}

void GridVoronoi::GenerateIntersectionNodes()
{
  auto columns_count = m_width / m_divisors + 1;
  auto row_count = m_height / m_divisors + 1;
  for (auto r : std::views::iota(1u, row_count))
  {
    for (auto c : std::views::iota(1u, columns_count))
    {
      auto left = c - 1;
      auto right = c;
      auto bot = r - 1;
      auto top = r;
      auto par_bot_left = m_particles[columns_count * bot + left];
      auto par_bot_right = m_particles[columns_count * bot + right];
      auto par_top_right = m_particles[columns_count * top + right];
      auto par_top_left = m_particles[columns_count * top + left];

      std::set<int> colors_ids{ std::get<2>(par_bot_left),
                                std::get<2>(par_bot_right),
                                std::get<2>(par_top_right),
                                std::get<2>(par_top_left) };

      if (colors_ids.size() <= 4 && colors_ids.size() > 1)
      {
        Point pt{ (std::get<0>(par_bot_left).x + (float)m_divisors / 2.0f),
                  (std::get<0>(par_bot_left).y + (float)m_divisors / 2.0f) };
        m_inter_pts.emplace_back(pt, static_cast<int>(colors_ids.size()));
      }
    }
  }
}
void GridVoronoi::OnInterPt(const std::function<void(const Point&, const Color&)>& action) const
{
  for (const auto& [pt, id] : m_inter_pts)
  {
    if (id == 2)
      action(pt, red);
    else if (id == 3)
      action(pt, green);
    else if (id == 4)
      action(pt, blue);
  }
}
