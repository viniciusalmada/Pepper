#include "VoronoiGenerator.hpp"

#include "Geom.hpp"

#include <set>
#include <utility>

constexpr auto SCALE_FACTOR = 10'000.0;

namespace VoronoiGenerator
{
  void InsertFirstRegion(const Point& pt, Diagram& diagram)
  {
    std::shared_ptr<Region> region = std::make_shared<Region>(Region{ pt, {} });
    diagram.AddRegion(region);
  }

  void InsertSecondRegion(const Point& pt, Diagram& diagram)
  {
    auto region0 = diagram.GetFirst();
    const Point& p0 = region0->source;
    const Line perpend = GetPerpendicular(p0, pt);
    const Point mid_pt = GetMidPoint(perpend);
    const Point perpend_unit = glm::normalize(perpend.second - perpend.first);

    auto edge = Edge::InfiniteEdge(mid_pt, perpend_unit);
    diagram.AddEdge(edge);

    region0->edges.insert(edge);
    auto region1 = std::make_shared<Region>(Region{ pt, { edge } });
    diagram.AddRegion(region1);

    if (CalcOrient(p0, pt, region0->source) == Orient::POSITIVE)
    {
      edge->SetRegionL(region0);
      edge->SetRegionR(region1);
    }
    else
    {
      edge->SetRegionL(region1);
      edge->SetRegionR(region0);
    }
  }

  std::shared_ptr<Region> GetRegionContainPt(const Point& pt, Diagram& diagram)
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

  std::vector<std::pair<std::shared_ptr<Edge>, Point>>
  GetEdgeIntersections(const Line& line, const std::set<std::shared_ptr<Edge>>& edges_set)
  {
    std::vector<std::pair<std::shared_ptr<Edge>, Point>> intersections{};
    for (const auto& edge : edges_set)
    {
      auto edge_line = edge->GetLine();
      bool found = false;
      Point inter_pt{};
      if (edge->IsInf())
      {
        std::tie(found, inter_pt) =
          Intersect(edge_line.first, edge_line.second, line.first, line.second, Ext::INFIN, Ext::INFIN);
      }
      else if (edge->IsSemiInf())
      {
        auto vtx = edge->GetFirstVertex();
        auto dist_vtx_line_first = glm::distance(edge_line.first, vtx->pt);
        auto dist_vtx_line_second = glm::distance(edge_line.second, vtx->pt);
        auto line_pt = dist_vtx_line_first > dist_vtx_line_second ? edge_line.first : edge_line.second;
        std::tie(found, inter_pt) = Intersect(vtx->pt, line_pt, line.first, line.second, Ext::SEMI, Ext::INFIN);
      }
      else
      {
        std::tie(found, inter_pt) = Intersect(edge->GetFirstVertex()->pt,
                                              edge->GetSecondVertex()->pt,
                                              line.first,
                                              line.second,
                                              Ext::FINIT,
                                              Ext::INFIN);
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
    auto vertices_finder = [](const Point& cmp, const std::shared_ptr<Vertex>& vtx)
    {
      return glm::distance(cmp, vtx->pt) < TOLERANCE;
    };

    const Line perpendicular_bisector = GetPerpendicular(existingRegion->source, newRegion->source);
    auto intersections = GetEdgeIntersections(perpendicular_bisector, existingRegion->edges);

    std::shared_ptr<Edge> new_edge = nullptr;
    if (intersections.empty())
    {
      Point mid_pt = GetMidPoint(perpendicular_bisector);
      Point direction = glm::normalize(perpendicular_bisector.second - perpendicular_bisector.first);

      new_edge = Edge::InfiniteEdge(mid_pt, direction);
      new_edge->SetRegionL(newRegion);
      new_edge->SetRegionR(existingRegion);
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
      Point direction = GetMidPoint(perpendicular_bisector) - inter_pt;
      new_edge = Edge::SemiInfEdge(new_vertex, glm::normalize(direction));
      new_edge->SetRegionL(newRegion);
      new_edge->SetRegionR(existingRegion);
      return { new_edge, intercepted_edge->GetMate(existingRegion) };
    }

    if (intersections.size() == 2)
    {
      auto [first_intercepted_edge, first_inter_pt] = intersections[0];
      auto [second_intercepted_edge, second_inter_pt] = intersections[1];
      // Find the next region from intercepted edges
      std::shared_ptr<Edge> edge_adj_next_region;
      {
        auto first_mate = first_intercepted_edge->GetMate(existingRegion);
        auto second_mate = second_intercepted_edge->GetMate(existingRegion);
        for (auto& new_region_edge : newRegion->edges)
        {
          if (new_region_edge->GetMate(newRegion) == first_mate)
          {
            edge_adj_next_region = second_intercepted_edge;
            continue;
          }
          if (new_region_edge->GetMate(newRegion) == second_mate)
          {
            edge_adj_next_region = first_intercepted_edge;
            continue;
          }
        }
      }

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
      new_edge = Edge::FiniteEdge(new_first_vertex, new_second_vertex);

      new_edge->SetRegionL(newRegion);
      new_edge->SetRegionR(existingRegion);
      return { new_edge, edge_adj_next_region->GetMate(existingRegion) };
    }

    return { nullptr, nullptr };
  }

  void UpdateDiagram(const Point& pt, Diagram& diagram)
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
    std::vector<std::shared_ptr<Region>> regions_visited;
    std::shared_ptr<Region> region_container = GetRegionContainPt(pt, diagram);
    regions_visited.push_back(region_container);

    // Create the new empty region
    auto new_region = std::make_shared<Region>(Region{ pt, {} });
    diagram.AddRegion(new_region);

    // Find the bisector edge (e0) between new point and next region to search
    auto [new_edge, next_region] = GetBisector(region_container, new_region, diagram.Vertices());
    diagram.AddEdge(new_edge);
    new_region->edges.insert(new_edge);
    regions_visited.push_back(next_region);

    // From the next bisector edge until returns to first region or get a null region
    while (next_region != nullptr && next_region != region_container)
    {
      std::tie(new_edge, next_region) = GetBisector(next_region, new_region, diagram.Vertices());
      diagram.AddEdge(new_edge);
      new_region->edges.insert(new_edge);
      if (next_region == nullptr)
        break;
      if (std::find(regions_visited.begin(), regions_visited.end(), next_region) != regions_visited.end())
        break;
      regions_visited.push_back(next_region);
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
      for (auto [intersected_edge, point] : intersections)
      {
        // If is infinity, a vertex will be set up
        // And a direction will be updated
        if (intersected_edge->IsInf())
        {
          auto inter_vtx = edge_of_new_region->GetNearVertex(point);
          intersected_edge->SetVertex0(inter_vtx);
          intersected_edge->UpdateInfToSemiInf();
        }
        else if (intersected_edge->IsSemiInf())
        {
          auto inter_vtx = edge_of_new_region->GetNearVertex(point);
          auto old_v0 = intersected_edge->GetFirstVertex();
          std::erase_if(visited_region->edges,
                        [&](const std::shared_ptr<Edge>& reg_edge)
                        {
                          if (reg_edge == intersected_edge)
                            return false;
                          if (reg_edge->GetFirstVertex() == old_v0)
                            return true;
                          if (reg_edge->GetSecondVertex() == old_v0)
                            return true;
                          return false;
                        });
          intersected_edge->SetVertex0(inter_vtx);
        }
      }
      visited_region->edges.insert(edge_of_new_region);
    }

    std::set<std::shared_ptr<Edge>> edges_of_all_regions;
    for (const auto& region : diagram.Regions())
    {
      edges_of_all_regions.insert(region->edges.begin(), region->edges.end());
    }
    diagram.Edges() = edges_of_all_regions;

    std::set<std::shared_ptr<Vertex>, VertexSorter> vtx_of_all_edges;
    for (const auto& edge : edges_of_all_regions)
    {
      auto v0 = edge->GetFirstVertex();
      auto v1 = edge->GetSecondVertex();
      if (v0)
        vtx_of_all_edges.insert(v0);
      if (v1)
        vtx_of_all_edges.insert(v1);
    }
    diagram.Vertices() = vtx_of_all_edges;

    PP_INFO("Regions: {0}", diagram.RegionsCount());
    PP_INFO("Edges: {0}", diagram.Edges().size());
    PP_INFO("Vertices: {0}", diagram.Vertices().size());
  }

  Line Edge::GetFiniteFormEdge(const Point& botLeft, const Point& topRight) const
  {
    Line line = GetLine();
    auto [_0, inter_top] =
      Intersect({ botLeft.x, topRight.y }, topRight, line.first, line.second, Ext::INFIN, Ext::INFIN);
    auto [_1, inter_bot] =
      Intersect(botLeft, { topRight.x, botLeft.y }, line.first, line.second, Ext::INFIN, Ext::INFIN);

    if (IsInf())
      return { inter_top, inter_bot };

    if ((m_v0 != nullptr && m_v1 == nullptr) || (m_v0 == nullptr && m_v1 != nullptr))
    {
      Point& existing_v = m_v0 == nullptr ? m_v1->pt : m_v0->pt;

      // Get a perpendicular line from unit line
      auto perp_unit_line = GetPerpendicular(line.first, line.second);

      auto top_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, inter_top);
      auto bot_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, inter_bot);
      auto v0_orient = CalcOrient(perp_unit_line.first, perp_unit_line.second, existing_v);
      if (v0_orient == top_orient)
        return { existing_v, inter_bot };

      if (v0_orient == bot_orient)
        return { existing_v, inter_top };
    }

    return { m_v0->pt, m_v1->pt };
  }

  void Edge::UpdateInfToSemiInf()
  {
    // Update the direction based on adjacent point's regions
    auto pt_up = m_v0->pt + m_direction;
    auto pt_down = m_v0->pt - m_direction;
    auto perpen_v0 = GetPerpendicular(pt_up, pt_down);

    auto orient_region_left = CalcOrient(perpen_v0.first, perpen_v0.second, m_region_L->source);
    //    auto orient_region_right = CalcOrient(perpen_v0.first, perpen_v0.second, m_region_R->source);
    auto orient_pt_up = CalcOrient(perpen_v0.first, perpen_v0.second, pt_up);
    if (orient_region_left != orient_pt_up)
      m_direction = -m_direction;
  }
}