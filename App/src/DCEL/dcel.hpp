#ifndef PEPPER_DCEL_HPP
#define PEPPER_DCEL_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <set>

using Point = glm::dvec2;
using Line = std::pair<Point, Point>;

struct Region;
struct Vertex;
struct BaseEdge;

struct Region
{
  Point pt;
  std::set<BaseEdge*> edges;
};

struct Vertex
{
  Point pt;
};

struct BaseEdge
{
  Vertex* v_beg = nullptr; // begin
  Vertex* v_end = nullptr; // end
  Region* r_left = nullptr;
  Region* r_right = nullptr;
  [[nodiscard]] virtual bool IsStraight() const = 0;
};

struct StraightEdge : BaseEdge
{
  [[nodiscard]] bool IsStraight() const override;
};

struct CurveEdge : BaseEdge
{
  double p_angle_begin;
  double p_angle_end;
  [[nodiscard]] bool IsStraight() const override;
};

class Diagram
{
public:
  static Diagram* SimpleDiagram(const Line& limits);

  void UpdateDiagram(Point pt);

  const std::set<BaseEdge*> Edges();

private:
  Region* NearestRegion(const Point& pt) const;

  std::set<Region*> m_regions{};
  std::set<BaseEdge*> m_edges{};
  Line m_limits{};
  double m_radius{};
  void InsertFirstRegion(const Point& pt);
  Point GetCenter() const;
  void InsertSecondRegion(const Point& pt);
};

#endif // PEPPER_DCEL_HPP
