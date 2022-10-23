#ifndef PEPPER_GRIDVORONOI_HPP
#define PEPPER_GRIDVORONOI_HPP

#include "PointsCmp.hpp"

#include <cstdint>
#include <glm/glm.hpp>
#include <map>
#include <set>

using Point = glm::dvec2;
using Color = glm::vec3;

template <typename In, typename Out>
std::set<Out> fmap(std::function<Out(In)> fun, std::set<In> input)
{
  std::set<Out> output;
  std::transform(input.begin(), input.end(), std::inserter(output, output.begin()), fun);
  return output;
}

struct Region
{
  Point pt;
  Color color;
  int id;

  bool operator<(const Region& other) const { return PointsCmp()(this->pt, other.pt); }
};

class GridVoronoi
{
public:
  GridVoronoi(uint32_t width, uint32_t height, std::set<Point, PointsCmp> points);

  void OnParticle(const std::function<void(const Point&, const Color&)>& action) const;

  void OnInterPt(const std::function<void(const Point&, const Color&)>& action) const;

  void Update(int divisors, std::set<Point, PointsCmp> points);

  void GenerateIntersectionNodes();

private:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_divisors;
  std::set<Region> m_points;
  std::vector<std::pair<Point, int>> m_particles;
  std::map<Region, Color> m_colors;
  std::vector<std::pair<Point, int>> m_inter_pts;
};

#endif // PEPPER_GRIDVORONOI_HPP
