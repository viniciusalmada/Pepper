#include "VoronoiGenerator.hpp"

#include <set>
#include <utility>

constexpr auto ZERO_F = 0.0;
constexpr auto TOLERANCE = 1.0e-7;
constexpr auto SCALE_FACTOR = 10'000.0;

enum class Orient { ZERO, POSITIVE, NEGATIVE };

auto Area2D(const glm::dvec2& p1, const glm::dvec2& p2, const glm::dvec2& p3)
{
  auto AB = p2 - p1;
  auto AC = p3 - p1;

  //  return glm::cross(glm::vec3{ AB.x, AB.y, 0.0 }, glm::vec3{ AC.x, AC.y, 0.0 }).z;
  return AB.x * AC.y - AC.x * AB.y;
}

Orient CalcOrient(const glm::dvec2& p1, const glm::dvec2& p2, const glm::dvec2& p3)
{
  auto d = Area2D(p1, p2, p3);
  if (std::abs(d) < TOLERANCE)
    return Orient::ZERO;
  if (d > ZERO_F)
    return Orient::POSITIVE;

  return Orient::NEGATIVE;
}

std::tuple<bool, glm::dvec2> Intersect(const glm::dvec2& A,
                                       const glm::dvec2& B,
                                       const glm::dvec2& C,
                                       const glm::dvec2& D,
                                       bool ignoreFirstSegment,
                                       bool ignoreAllSegments)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  // Check collinear
  {
    if (orientABC == Orient::ZERO && orientABD == Orient::ZERO)
      return { false, {} };
  }

  if (ignoreAllSegments)
  {
    // Calculate the intersection
    auto t_CD = Area2D(A, B, C) / (Area2D(A, B, C) - Area2D(A, B, D));
    return { true, C + (t_CD * (D - C)) };
  }

  if (ignoreFirstSegment)
  {
    // Check second segment is on left or right of the first segment
    {
      if ((C.x > A.x) && (C.x > B.x) && (D.x > A.x) && (D.x > B.x))
        return { false, {} };

      if ((C.x < A.x) && (C.x < B.x) && (D.x < A.x) && (D.x < B.x))
        return { false, {} };
    }
    // Check second segment is on top or bottom of the first segment
    {
      if ((C.y > A.y) && (C.y > B.y) && (D.y > A.y) && (D.y > B.y))
        return { false, {} };

      if ((C.y < A.y) && (C.y < B.y) && (D.y < A.y) && (D.y < B.y))
        return { false, {} };
    }

    // Check second segment on the same side of first segment
    {
      if (orientABC == Orient::POSITIVE && orientABD == Orient::POSITIVE)
        return { false, {} };

      if (orientABC == Orient::NEGATIVE && orientABD == Orient::NEGATIVE)
        return { false, {} };
    }

    // Calculate the intersection
    auto t_CD = Area2D(A, B, C) / (Area2D(A, B, C) - Area2D(A, B, D));
    return { true, C + (t_CD * (D - C)) };
  }

  // Check second segment is on left or right of the first segment
  {
    if ((C.x > A.x) && (C.x > B.x) && (D.x > A.x) && (D.x > B.x))
      return { false, {} };

    if ((C.x < A.x) && (C.x < B.x) && (D.x < A.x) && (D.x < B.x))
      return { false, {} };
  }
  // Check second segment is on top or bottom of the first segment
  {
    if ((C.y > A.y) && (C.y > B.y) && (D.y > A.y) && (D.y > B.y))
      return { false, {} };

    if ((C.y < A.y) && (C.y < B.y) && (D.y < A.y) && (D.y < B.y))
      return { false, {} };
  }

  // Check second segment on the same side of first segment
  {
    if (orientABC == Orient::POSITIVE && orientABD == Orient::POSITIVE)
      return { false, {} };

    if (orientABC == Orient::NEGATIVE && orientABD == Orient::NEGATIVE)
      return { false, {} };
  }

  auto orientCDA = CalcOrient(C, D, A);
  auto orientCDB = CalcOrient(C, D, B);
  // Check first segment on the same side of the second segment
  {
    if (orientCDA == Orient::POSITIVE && orientCDB == Orient::POSITIVE)
      return { false, {} };

    if (orientCDA == Orient::NEGATIVE && orientCDB == Orient::NEGATIVE)
      return { false, {} };
  }

  // Check if C point touch AB
  {
    if (orientABC == Orient::ZERO && orientABD != Orient::ZERO)
      return { true, C };
  }

  // Check if D point touch AB
  {
    if (orientABC != Orient::ZERO && orientABD == Orient::ZERO)
      return { true, D };
  }

  // Check if A point touch CD
  {
    if (orientCDA == Orient::ZERO && orientABD != Orient::ZERO)
      return { true, A };
  }

  // Check if B point touch CD
  {
    if (orientCDA != Orient::ZERO && orientABD == Orient::ZERO)
      return { true, B };
  }

  // Calculate the intersection
  auto t_CD = Area2D(A, B, C) / (Area2D(A, B, C) - Area2D(A, B, D));
  return { true, C + (t_CD * (D - C)) };
}

namespace VoronoiGenerator
{
  Line GetPerpendicular(const glm::dvec2& p0, const glm::dvec2& p1)
  {
    const glm::dvec2 line_seg = p1 - p0;
    const glm::dvec2 mid_point = p0 + line_seg / 2.0;

    const glm::dvec2 line_normal{ -line_seg.y, line_seg.x };
    const glm::dvec2 perpend_0 = mid_point - line_normal / 2.0;
    const glm::dvec2 perpend_1 = mid_point + line_normal / 2.0;
    return { perpend_0, perpend_1 };
  }

  void InsertFirstRegion(const Sample& pt, Diagram& diagram)
  {
    std::shared_ptr<Region> region = std::make_shared<Region>(Region{ pt, {} });
    diagram.AddRegion(region);
  }

  void InsertSecondRegion(const glm::dvec2& pt, Diagram& diagram)
  {
    auto region0 = diagram.GetFirst();
    const glm::dvec2& p0 = region0->source;
    const Line perpend = GetPerpendicular(p0, pt);

    auto edge = std::make_shared<Edge>(Edge{ nullptr, nullptr, perpend });
    diagram.AddEdge(edge);

    region0->edges.insert(edge);
    auto region1 = std::make_shared<Region>(Region{ pt, { edge } });
    diagram.AddRegion(region1);

    edge->SetRegion0(region0);
    edge->SetRegion1(region1);

    //    if (CalcOrient(edge->First(), edge->Second(), region0->source) == Orient::POSITIVE)
    //    {
    //      edge->region_left = region1;
    //      edge->region_right = region0;
    //    }
    //    else
    //    {
    //      edge->region_right = region1;
    //      edge->region_left = region0;
    //    }
  }

  std::shared_ptr<Region> GetRegionContainPt(const glm::dvec2& pt, Diagram& diagram)
  {
    std::shared_ptr<Region> region_container = nullptr;
    for (const auto& region : diagram)
    {
      bool same_side_as_source = true;
      for (const auto& edge : region->edges)
      {
        auto orient_source = CalcOrient(edge->First(), edge->Second(), region->source);
        auto orient_pt = CalcOrient(edge->First(), edge->Second(), pt);
        if (orient_source != orient_pt)
        {
          same_side_as_source = false;
          break;
        }
      }

      if (same_side_as_source)
      {
        region_container = region;
        break;
      }
    }

    return region_container;
  }

  std::tuple<std::shared_ptr<Edge>, std::shared_ptr<Region>> GetBisector(const std::shared_ptr<Region>& region0,
                                                                         const std::shared_ptr<Region>& newRegion)
  {
    const Line perpendicular_bisector = GetPerpendicular(region0->source, newRegion->source);
    std::vector<std::pair<std::shared_ptr<Edge>, Vertex>> intersections{};
    for (const auto& edge : region0->edges)
    {
      auto [found, inter_pt] = Intersect(perpendicular_bisector.first,
                                         perpendicular_bisector.second,
                                         edge->First(),
                                         edge->Second(),
                                         true,
                                         false);
      if (found)
      {
        intersections.emplace_back(edge, inter_pt);
      }
    }

    std::shared_ptr<Edge> new_edge = nullptr;
    if (intersections.empty())
    {
      new_edge = std::make_shared<Edge>(nullptr, nullptr, perpendicular_bisector);
      return { new_edge, nullptr };
    }

    if (intersections.size() == 1)
    {
      auto [intercepted_edge, inter_pt] = intersections[0];
      std::shared_ptr<Vertex> new_vertex = std::make_shared<Vertex>(inter_pt);
      new_edge = std::make_shared<Edge>(new_vertex, nullptr, perpendicular_bisector);
      return { new_edge, intercepted_edge->GetMate(region0) };
    }

    if (intersections.size() == 2)
    {
      auto [first_intercepted_edge, first_inter_pt] = intersections[0];
      auto [_, second_inter_pt] = intersections[1];
      std::shared_ptr<Vertex> new_first_vertex = std::make_shared<Vertex>(first_inter_pt);
      std::shared_ptr<Vertex> new_second_vertex = std::make_shared<Vertex>(second_inter_pt);
      new_edge = std::make_shared<Edge>(new_first_vertex, new_second_vertex, perpendicular_bisector);
      return { new_edge, first_intercepted_edge->GetMate(region0) };
    }

    return { nullptr, nullptr };
  }

  void UpdateDiagram(const glm::dvec2& pt, Diagram& diagram)
  {
    // Empty diagram - insert infinity region
    if (diagram.Empty())
    {
      InsertFirstRegion(pt, diagram);
      return;
    }

    // Single region - insert second with one edge
    if (diagram.RegionsCount() == 1)
    {
      InsertSecondRegion(pt, diagram);
      return;
    }

    // Generic algorithm
    // Find the region (Rc) that contain the new point (pt)
    std::set<std::shared_ptr<Region>> neighbor_regions{};
    std::shared_ptr<Region> region_container = GetRegionContainPt(pt, diagram);
    neighbor_regions.insert(region_container);
    // Create the new empty region
    auto new_region = std::make_shared<Region>(Region{ pt, {} });
    diagram.AddRegion(new_region);
    // Find the bisector edge (e0) between new point and next region to search
    auto [new_edge, next_region] = GetBisector(region_container, new_region);
    if (next_region != nullptr)
      neighbor_regions.insert(next_region);
    diagram.AddEdge(new_edge);
    new_region->edges.insert(new_edge);
    // From the next bisector edge until returns to first region or get a null region
    while (next_region != nullptr && next_region != region_container)
    {
      std::tie(new_edge, next_region) = GetBisector(next_region, new_region);
      diagram.AddEdge(new_edge);
      new_region->edges.insert(new_edge);
      if (next_region == nullptr)
        break;
      neighbor_regions.insert(next_region);
    }

    // Update the edges based on new edges from new region
  }

  Line Edge::GetDrawableEdge(const glm::dvec2& limits) const
  {
    if (m_v0 == nullptr && m_v1 == nullptr)
    {
      // Intersection with top line [(0, limits.y), (limits.x, limits.y)]
      auto [_0, inter_top] =
        Intersect({ 0.0, limits.y }, { limits.x, limits.y }, m_unit_line.first, m_unit_line.second, false, true);
      auto [_1, inter_bot] =
        Intersect({ 0.0, 0.0 }, { limits.x, 0.0 }, m_unit_line.first, m_unit_line.second, false, true);
      return { inter_top, inter_bot };
    }

    if ((m_v0 != nullptr && m_v1 == nullptr) || (m_v0 == nullptr && m_v1 != nullptr))
    {
      auto& existing_v = m_v0 == nullptr ? *m_v1 : *m_v0;

      // Get a perpendicular line from unit line
      auto perp_unit_line = GetPerpendicular(m_unit_line.first, m_unit_line.second);

      // Calculates three intersections - top, bottom, perpendicular v0
      auto [_0, inter_top] =
        Intersect({ 0.0, limits.y }, { limits.x, limits.y }, m_unit_line.first, m_unit_line.second, false, true);
      auto [_1, inter_bot] =
        Intersect({ 0.0, 0.0 }, { limits.x, 0.0 }, m_unit_line.first, m_unit_line.second, false, true);

      auto top_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, inter_top);
      auto bot_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, inter_bot);
      auto v0_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, existing_v);
      if (v0_orient == top_orient)
        return { existing_v, inter_bot };

      if (v0_orient == bot_orient)
        return { existing_v, inter_top };
    }

    return { *m_v0, *m_v1 };
  }
}