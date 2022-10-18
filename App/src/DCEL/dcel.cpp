#include "DCEL/dcel.hpp"

#include "Geom.hpp"

#include <memory>

[[maybe_unused]] bool IsInsideInterval(double begin, double end, double pang)
{
  if (pang > begin && pang < end)
    return true;

  // interval ends beyond 2.0
  if (begin > end)
  {
    if (pang > begin && pang <= 8.0)
      return true;
    if (pang > 0.0 && pang < end)
      return true;
  }

  return false;
}

Diagram* Diagram::SimpleDiagram(const Line& limits)
{
  //  auto* reg0 = new Region{ {}, {}, true };
  //  auto* reg1 = new Region{ { 880, 120 }, {} };
  //  auto* reg2 = new Region{ { 470, 190 }, {} };
  //  auto* reg3 = new Region{ { 740, 480 }, {} };
  //  auto* reg4 = new Region{ { 340, 470 }, {} };
  //
  //  auto* vtx1 = new Vertex{ { 691.9158, 254.0784 }, {}, true };
  //  auto* vtx2 = new Vertex{ { 542.0344, 393.6231 }, {}, true };
  //  auto* vtx3 = new Vertex{ { 533.8750, 720.0000 }, {}, false };
  //  auto* vtx4 = new Vertex{ { 0.0000, 141.9643 }, {}, false };
  //  auto* vtx5 = new Vertex{ { 648.5366, 0.0000 }, {}, false };
  //  auto* vtx6 = new Vertex{ { 1280.0000, 482.7778 }, {}, false };
  //
  //  auto* edg1 = new Edge{ vtx1, vtx2, reg2, reg3, {}, {} };
  //  auto* edg2 = new Edge{ vtx2, vtx3, reg4, reg3, {}, {} };
  //  auto* edg3 = new Edge{ vtx2, vtx4, reg2, reg4, {}, {} };
  //  auto* edg4 = new Edge{ vtx1, vtx5, reg1, reg2, {}, {} };
  //  auto* edg5 = new Edge{ vtx1, vtx6, reg3, reg1, {}, {} };
  //  auto* edg6 = new Edge{ vtx6, vtx3, reg3, reg0, {}, {} };
  //  auto* edg7 = new Edge{ vtx3, vtx4, reg4, reg0, {}, {} };
  //  auto* edg8 = new Edge{ vtx5, vtx4, reg0, reg2, {}, {} };
  //  auto* edg9 = new Edge{ vtx5, vtx6, reg1, reg0, {}, {} };

  auto* diagram = new Diagram();
  diagram->m_limits = limits;
  diagram->m_radius = glm::distance(limits.first, limits.second) / 2.0 * 1.10;

  return diagram;
}

Region* Diagram::NearestRegion(const Point& pt) const
{
  double distance = std::numeric_limits<double>::max();
  Region* nearest = nullptr;
  for (auto* region : m_regions)
  {
    double inner_dist = glm::distance(region->pt, pt);
    if (inner_dist < distance)
    {
      nearest = region;
      distance = inner_dist;
    }
  }

  return nearest;
}

void Diagram::UpdateDiagram(Point pt)
{
  if (m_regions.empty())
  {
    InsertFirstRegion(pt);
    return;
  }

  if (m_regions.size() == 1)
  {
    InsertSecondRegion(pt);
    return;
  }

  Point center = GetCenter();
  auto* region = NearestRegion(pt);
  auto* new_region = new Region{ pt };
  m_regions.insert(new_region);
  auto bisector = GetPerpendicular(region->pt, pt);
  std::array<Vertex*, 2> new_edge_vertices{ { nullptr, nullptr } };
  for (auto* base_edge : region->edges)
  {
    if (base_edge->IsStraight())
    {
      auto [found, inter_pt] =
        Intersect(bisector.first, bisector.second, base_edge->v_beg->pt, base_edge->v_end->pt, Ext::INFIN, Ext::FINIT);
      if (!found)
        continue;

      auto* new_vertex = new Vertex{ inter_pt };
      if (new_edge_vertices[0] == nullptr)
        new_edge_vertices[0] = new_vertex;
      else
        new_edge_vertices[1] = new_vertex;
      // change the vertex of edge on the side of the new point based on bisector
      auto orient_region_edge_v_beg = CalcOrient(bisector.first, bisector.second, base_edge->v_beg->pt);
      auto orient_new_pt = CalcOrient(bisector.first, bisector.second, pt);
      if (orient_new_pt == orient_region_edge_v_beg)
        base_edge->v_beg = new_vertex;
      else
        base_edge->v_end = new_vertex;
      continue;
    }

    // If curve
    auto* curve_edge = (CurveEdge*)base_edge;
    auto inter_pts = InterCircleLine(center, m_radius, bisector.first, bisector.second);
    // Filter point on curve edge interval
    auto p_angle_first_inter = GetPseudoAngle(center, inter_pts.first);
    auto p_angle_second_inter = GetPseudoAngle(center, inter_pts.second);
    bool use_first = false;
    if (IsInsideInterval(curve_edge->p_angle_begin, curve_edge->p_angle_end, p_angle_first_inter))
    {
      use_first = true;
    }
    auto* new_vertex = new Vertex{ use_first ? inter_pts.first : inter_pts.second };
    if (new_edge_vertices[0] == nullptr)
      new_edge_vertices[0] = new_vertex;
    else
      new_edge_vertices[1] = new_vertex;

    // Split the curve edge
    auto* new_edge = new CurveEdge{};
    new_edge->v_beg = new_vertex;
    new_edge->v_end = curve_edge->v_end;
    new_edge->p_angle_begin = use_first ? p_angle_first_inter : p_angle_second_inter;
    new_edge->p_angle_end = curve_edge->p_angle_end;
    new_edge->r_left = new_region;
    new_region->edges.insert(new_edge);

    curve_edge->v_end = new_vertex;
    curve_edge->p_angle_end = use_first ? p_angle_first_inter : p_angle_second_inter;
    ;
  }
  auto* new_str_edge = new StraightEdge{};
  new_str_edge->v_beg = new_edge_vertices[0];
  new_str_edge->v_end = new_edge_vertices[1];
  if (CalcOrient(new_str_edge->v_beg->pt, new_str_edge->v_beg->pt, pt) == Orient::POSITIVE)
  {
    new_str_edge->r_left = new_region;
    new_str_edge->r_right = region;
  }
  else
  {
    new_str_edge->r_left = region;
    new_str_edge->r_right = new_region;
  }
  new_region->edges.insert(new_str_edge);
  region->edges.insert(new_str_edge);

  m_edges.insert(new_str_edge);
}
void Diagram::InsertSecondRegion(const Point& pt)
{
  auto* existing_region = m_regions.begin().operator*();
  auto* new_region = new Region{ pt };
  auto bisector_dir = GetPerpendicular(existing_region->pt, new_region->pt);
  Point center = GetCenter();
  auto inter_pts = InterCircleLine(center, m_radius, bisector_dir.first, bisector_dir.second);

  auto* vtx0 = new Vertex{ inter_pts.first };
  auto* vtx1 = new Vertex{ inter_pts.second };

  auto* straight_edge = new StraightEdge{};
  straight_edge->v_beg = vtx0;
  straight_edge->v_end = vtx1;
  auto orient_existing_region = CalcOrient(vtx0->pt, vtx1->pt, existing_region->pt);
  if (orient_existing_region == Orient::POSITIVE)
  {
    straight_edge->r_left = existing_region;
    straight_edge->r_right = new_region;
  }
  else
  {
    straight_edge->r_left = new_region;
    straight_edge->r_right = existing_region;
  }
  existing_region->edges.insert(straight_edge);
  new_region->edges.insert(straight_edge);

  auto p_ang_vtx0 = GetPseudoAngle(center, vtx0->pt); // 2.1
  auto p_ang_vtx1 = GetPseudoAngle(center, vtx1->pt); // 5.8
  auto* curve_edge0 = new CurveEdge{};
  auto* curve_edge1 = new CurveEdge{};
  if (p_ang_vtx0 < p_ang_vtx1)
  {
    curve_edge0->p_angle_begin = p_ang_vtx0;
    curve_edge0->p_angle_end = p_ang_vtx1;
    curve_edge0->v_beg = vtx0;
    curve_edge0->v_end = vtx1;

    curve_edge1->p_angle_begin = p_ang_vtx1;
    curve_edge1->p_angle_end = p_ang_vtx0;
    curve_edge1->v_beg = vtx1;
    curve_edge1->v_end = vtx0;
  }
  else
  {
    curve_edge0->p_angle_begin = p_ang_vtx1;
    curve_edge0->p_angle_end = p_ang_vtx0;
    curve_edge0->v_beg = vtx1;
    curve_edge0->v_end = vtx0;

    curve_edge1->p_angle_begin = p_ang_vtx0;
    curve_edge1->p_angle_end = p_ang_vtx1;
    curve_edge1->v_beg = vtx0;
    curve_edge1->v_end = vtx1;
  }

  auto p_ang_existing_region = GetPseudoAngle(center, existing_region->pt);
  auto p_ang_new_region = GetPseudoAngle(center, new_region->pt);
  if (IsInsideInterval(curve_edge0->p_angle_begin, curve_edge0->p_angle_end, p_ang_existing_region) &&
      IsInsideInterval(curve_edge1->p_angle_begin, curve_edge1->p_angle_end, p_ang_new_region))
  {
    curve_edge0->r_left = existing_region;
    existing_region->edges.insert(curve_edge0);
    curve_edge1->r_left = new_region;
    new_region->edges.insert(curve_edge1);
  }
  else
  {
    curve_edge0->r_left = new_region;
    new_region->edges.insert(curve_edge0);
    curve_edge1->r_left = existing_region;
    existing_region->edges.insert(curve_edge1);
  }
  m_edges.insert(curve_edge0);
  m_edges.insert(curve_edge1);
  m_edges.insert(straight_edge);
  m_regions.insert(new_region);
}
Point Diagram::GetCenter() const
{
  auto center = GetMidPoint(m_limits);
  return center;
}
void Diagram::InsertFirstRegion(const Point& pt)
{
  auto* region = new Region{ pt };
  m_regions.insert(region);
}
const std::set<BaseEdge*> Diagram::Edges()
{
  return m_edges;
}
bool StraightEdge::IsStraight() const
{
  return true;
}
bool CurveEdge::IsStraight() const
{
  return false;
}
