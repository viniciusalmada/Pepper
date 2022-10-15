#include "VoronoiGenerator.hpp"

#include <set>
#include <utility>

constexpr auto ZERO_F = 0.0;
constexpr auto TOLERANCE = 1.0e-7;
constexpr auto SCALE_FACTOR = 10'000.0;

enum class Ext { FINIT, INFIN, SEMI };

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

bool IsCollinear(const glm::dvec2& A, const glm::dvec2& B, const glm::dvec2& C, const glm::dvec2& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  return orientABC == Orient::ZERO && orientABD == Orient::ZERO;
}

glm::dvec2 GetInterPoint(const glm::dvec2& A, const glm::dvec2& B, const glm::dvec2& C, const glm::dvec2& D)
{
  // Calculate the intersection
  auto t_CD = Area2D(A, B, C) / (Area2D(A, B, C) - Area2D(A, B, D));
  return C + (t_CD * (D - C));
}

bool SecondLineIsOutOfFirstLineBox(const glm::dvec2& A, const glm::dvec2& B, const glm::dvec2& C, const glm::dvec2& D)
{
  if ((C.x > A.x) && (C.x > B.x) && (D.x > A.x) && (D.x > B.x))
    return true;
  if ((C.x < A.x) && (C.x < B.x) && (D.x < A.x) && (D.x < B.x))
    return true;
  if ((C.y > A.y) && (C.y > B.y) && (D.y > A.y) && (D.y > B.y))
    return true;
  if ((C.y < A.y) && (C.y < B.y) && (D.y < A.y) && (D.y < B.y))
    return true;
  return false;
}

bool SecondLineOnLeftOrRightOfFirst(const glm::dvec2& A, const glm::dvec2& B, const glm::dvec2& C, const glm::dvec2& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  if (orientABC == Orient::POSITIVE && orientABD == Orient::POSITIVE)
    return true;
  if (orientABC == Orient::NEGATIVE && orientABD == Orient::NEGATIVE)
    return true;
  return false;
}

bool SecondLineTouchesFirstLine(const glm::dvec2& A, const glm::dvec2& B, const glm::dvec2& C, const glm::dvec2& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  if (orientABC == Orient::ZERO && orientABD != Orient::ZERO)
    return true;
  if (orientABC != Orient::ZERO && orientABD == Orient::ZERO)
    return true;
  return false;
}

/**
 *
 * @param A first node of first line (if is semi-infinite is the finite node)
 * @param B second node of first line (if is semi-infinite is the infinite node)
 * @param C first node of second line (if is semi-infinite is the finite node)
 * @param D second node of second line (if is semi-infinite is the infinite node)
 * @param firstExt
 * @param secondExt
 * @return
 */
std::tuple<bool, glm::dvec2> Intersect(const glm::dvec2& A,
                                       const glm::dvec2& B,
                                       const glm::dvec2& C,
                                       const glm::dvec2& D,
                                       Ext firstExt,
                                       Ext secondExt)
{
  if (IsCollinear(A, B, C, D))
    return { false, {} };

  if (firstExt == Ext::INFIN && secondExt == Ext::INFIN)
  {
    auto inter_pt = GetInterPoint(A, B, C, D);
    return { true, inter_pt };
  }

  if (firstExt == Ext::INFIN && secondExt == Ext::FINIT)
  {
    if (SecondLineIsOutOfFirstLineBox(A, B, C, D))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(A, B, C, D))
      return { false, {} };

    auto inter_pt = GetInterPoint(A, B, C, D);
    return { true, inter_pt };
  }

  if (firstExt == Ext::FINIT && secondExt == Ext::INFIN)
  {
    if (SecondLineIsOutOfFirstLineBox(C, D, A, B))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(C, D, A, B))
      return { false, {} };

    auto inter_pt = GetInterPoint(C, D, A, B);
    return { true, inter_pt };
  }

  if (firstExt == Ext::FINIT && secondExt == Ext::FINIT)
  {
    if (SecondLineIsOutOfFirstLineBox(A, B, C, D))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(A, B, C, D))
      return { false, {} };

    if (SecondLineIsOutOfFirstLineBox(C, D, A, B))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(C, D, A, B))
      return { false, {} };

    if (SecondLineTouchesFirstLine(A, B, C, D))
      return { true, GetInterPoint(A, B, C, D) };

    if (SecondLineTouchesFirstLine(C, D, A, B))
      return { true, GetInterPoint(C, D, A, B) };

    auto inter_pt = GetInterPoint(C, D, A, B);
    return { true, inter_pt };
  }

  auto inter_pt = GetInterPoint(C, D, A, B);
  return { true, inter_pt };
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
        auto orient_source = CalcOrient(edge->GetLine().first, edge->GetLine().second, region->source);
        auto orient_pt = CalcOrient(edge->GetLine().first, edge->GetLine().second, pt);
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

  std::vector<std::pair<std::shared_ptr<Edge>, Vertex>>
  GetEdgeIntersections(const Line& line, const std::set<std::shared_ptr<Edge>>& edges_set)
  {
    std::vector<std::pair<std::shared_ptr<Edge>, Vertex>> intersections{};
    for (const auto& edge : edges_set)
    {
      auto edge_line = edge->GetLine();
      bool found = false;
      glm::dvec2 inter_pt{};
      if (edge->IsInf())
      {
        std::tie(found, inter_pt) =
          Intersect(edge_line.first, edge_line.second, line.first, line.second, Ext::INFIN, Ext::INFIN);
      }
      else if (edge->IsSemiInf())
      {
        auto vtx = edge->GetFirstVertex();
        std::tie(found, inter_pt) =
          Intersect(edge_line.first, edge_line.second, line.first, line.second, Ext::SEMI, Ext::INFIN);
      }
      else
      {
        std::tie(found, inter_pt) =
          Intersect(*edge->GetFirstVertex(), *edge->GetSecondVertex(), line.first, line.second, Ext::FINIT, Ext::INFIN);
      }

      if (found)
      {
        intersections.emplace_back(edge, inter_pt);
      }
    }
    return intersections;
  }

  std::tuple<std::shared_ptr<Edge>, std::shared_ptr<Region>>
  GetBisector(const std::shared_ptr<Region>& existingRegion,
              const std::shared_ptr<Region>& newRegion,
              const std::set<std::shared_ptr<Vertex>, VertexSorter>& vertices)
  {
    auto vertices_finder = [](const Vertex& cmp, const std::shared_ptr<Vertex>& vtx)
    {
      return glm::distance(cmp, *vtx) < TOLERANCE;
    };

    const Line perpendicular_bisector = GetPerpendicular(existingRegion->source, newRegion->source);
    auto intersections = GetEdgeIntersections(perpendicular_bisector, existingRegion->edges);

    std::shared_ptr<Edge> new_edge = nullptr;
    if (intersections.empty())
    {
      new_edge = std::make_shared<Edge>(nullptr, nullptr, perpendicular_bisector);
      new_edge->SetRegion0(newRegion);
      new_edge->SetRegion1(existingRegion);
      return { new_edge, nullptr };
    }

    if (intersections.size() == 1)
    {
      auto [intercepted_edge, inter_pt] = intersections[0];
      auto vertex_found =
        std::find_if(vertices.begin(), vertices.end(), std::bind(vertices_finder, inter_pt, std::placeholders::_1));
      std::shared_ptr<Vertex> new_vertex;
      if (vertex_found == vertices.end())
        new_vertex = std::make_shared<Vertex>(inter_pt);
      else
        new_vertex = *vertex_found;
      new_edge = std::make_shared<Edge>(new_vertex, nullptr, perpendicular_bisector);
      new_edge->SetRegion0(newRegion);
      new_edge->SetRegion1(existingRegion);
      return { new_edge, intercepted_edge->GetMate(existingRegion) };
    }

    if (intersections.size() == 2)
    {
      auto [first_intercepted_edge, first_inter_pt] = intersections[0];
      auto [_, second_inter_pt] = intersections[1];

      auto first_vertex_found = std::find_if(vertices.begin(),
                                             vertices.end(),
                                             std::bind(vertices_finder, first_inter_pt, std::placeholders::_1));
      std::shared_ptr<Vertex> new_first_vertex;
      if (first_vertex_found == vertices.end())
        new_first_vertex = std::make_shared<Vertex>(first_inter_pt);
      else
        new_first_vertex = *first_vertex_found;

      auto second_vertex_found = std::find_if(vertices.begin(),
                                              vertices.end(),
                                              std::bind(vertices_finder, second_inter_pt, std::placeholders::_1));
      std::shared_ptr<Vertex> new_second_vertex;
      if (second_vertex_found == vertices.end())
        new_second_vertex = std::make_shared<Vertex>(second_inter_pt);
      else
        new_second_vertex = *second_vertex_found;
      new_edge = std::make_shared<Edge>(new_first_vertex, new_second_vertex, perpendicular_bisector);
      new_edge->SetRegion0(newRegion);
      new_edge->SetRegion1(existingRegion);
      return { new_edge, first_intercepted_edge->GetMate(existingRegion) };
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
    std::shared_ptr<Region> region_container = GetRegionContainPt(pt, diagram);
    // Create the new empty region
    auto new_region = std::make_shared<Region>(Region{ pt, {} });
    diagram.AddRegion(new_region);
    // Find the bisector edge (e0) between new point and next region to search
    auto [new_edge, next_region] = GetBisector(region_container, new_region, diagram.Vertices());
    diagram.AddEdge(new_edge);
    new_region->edges.insert(new_edge);
    // From the next bisector edge until returns to first region or get a null region
    while (next_region != nullptr && next_region != region_container)
    {
      std::tie(new_edge, next_region) = GetBisector(next_region, new_region, diagram.Vertices());
      diagram.AddEdge(new_edge);
      new_region->edges.insert(new_edge);
      if (next_region == nullptr)
        break;
    }

    // Update the edges based on new edges from new region
    // Update the vertices and adjacent regions
    for (auto& edge_of_new_region : new_region->edges)
    {
      // Get visited regions to update their crossing edges
      auto visited_region = edge_of_new_region->GetMate(new_region);
      // Recalculate the intersections between new edge and the visited region
      auto intersections = GetEdgeIntersections(edge_of_new_region->GetLine(), visited_region->edges);
      // For each
      for (auto [intersected_edge, vertex] : intersections)
      {
        // If is infinity, a vertex will be set up
        if (intersected_edge->IsInf())
        {
          auto inter_vtx = edge_of_new_region->GetNearVertex(vertex);
          intersected_edge->SetVertex0(inter_vtx);
        }
      }
      visited_region->edges.insert(edge_of_new_region);
    }

    PP_INFO("Regions: {0}", diagram.RegionsCount());
    PP_INFO("Edges: {0}", diagram.Edges().size());
    PP_INFO("Vertices: {0}", diagram.Vertices().size());
  }

  Line Edge::GetFinityFormEdge(const glm::dvec2& botLeft, const glm::dvec2& topRight) const
  {
    auto [_0, inter_top] =
      Intersect({ botLeft.x, topRight.y }, topRight, m_line.first, m_line.second, Ext::INFIN, Ext::INFIN);
    auto [_1, inter_bot] =
      Intersect(botLeft, { topRight.x, botLeft.y }, m_line.first, m_line.second, Ext::INFIN, Ext::INFIN);

    if (IsInf())
      return { inter_top, inter_bot };

    if ((m_v0 != nullptr && m_v1 == nullptr) || (m_v0 == nullptr && m_v1 != nullptr))
    {
      auto& existing_v = m_v0 == nullptr ? *m_v1 : *m_v0;

      // Get a perpendicular line from unit line
      auto perp_unit_line = GetPerpendicular(m_line.first, m_line.second);

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