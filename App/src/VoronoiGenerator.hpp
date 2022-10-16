#ifndef PEPPERENGINE_VORONOIGENERATOR_HPP
#define PEPPERENGINE_VORONOIGENERATOR_HPP

#include <glm/glm.hpp>
#include <set>
#include <utility>

using Point = glm::dvec2;
using Line = std::pair<glm::dvec2, glm::dvec2>;

namespace VoronoiGenerator
{

  class Edge;

  struct Vertex
  {
    Vertex(const Point& point, const std::shared_ptr<Edge>& e) : pt(point), edge(e) {}
    Point pt;
    std::shared_ptr<Edge> edge;
  };

  struct Region;

  // Using edge to guide the DCEL data structure
  // DCEL - Doubly Connected Edge List
  // Region - [Sample, Edge]
  // Vertex - [Vertex, Edge]
  class Edge
  {
  public:
    Edge(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, Line line) :
        m_v0(std::move(v0)),
        m_v1(std::move(v1)),
        m_line(std::move(line))
    {
    }

    [[nodiscard]] auto Length() const { return glm::length(m_v1->pt - m_v0->pt); }
    [[nodiscard]] std::shared_ptr<Region> GetMate(const std::shared_ptr<Region>& reg) const
    {
      return reg == m_region_0 ? m_region_1 : m_region_0;
    }

    [[nodiscard]] std::shared_ptr<Vertex> GetFirstVertex() const { return m_v0; }
    [[nodiscard]] std::shared_ptr<Vertex> GetSecondVertex() const { return m_v1; }

    void SetRegion0(std::shared_ptr<Region> region) { m_region_0 = std::move(region); }
    void SetRegion1(std::shared_ptr<Region> region) { m_region_1 = std::move(region); }

    [[nodiscard]] Line GetFinityFormEdge(const glm::dvec2& botLeft, const glm::dvec2& topRight) const;
    [[nodiscard]] auto GetMidPoint() const { return m_line.first + (m_line.second - m_line.first) / 2.0; };

    [[nodiscard]] const Line& GetLine() const { return m_line; };

    [[nodiscard]] bool IsInf() const { return m_v0 == nullptr && m_v1 == nullptr; }
    [[nodiscard]] bool IsSemiInf() const
    {
      return (m_v0 != nullptr && m_v1 == nullptr) || (m_v0 == nullptr && m_v1 != nullptr);
    }

    [[nodiscard]] std::shared_ptr<Vertex> GetNearVertex(const Point& reference) const
    {
      if (IsInf())
        return nullptr;
      if (m_v0 != nullptr && m_v1 == nullptr)
        return m_v0;
      if (m_v0 == nullptr && m_v1 != nullptr)
        return m_v1;
      auto dist_0 = glm::distance(reference, m_v0->pt);
      auto dist_1 = glm::distance(reference, m_v1->pt);
      if (dist_0 < dist_1)
        return m_v0;
      return m_v1;
    }

    void SetVertex0(std::shared_ptr<Vertex> vtx) { m_v0 = std::move(vtx); }
    void SetVertex1(std::shared_ptr<Vertex> vtx) { m_v1 = std::move(vtx); }

  private:
    std::shared_ptr<Vertex> m_v0; // vB
    std::shared_ptr<Vertex> m_v1; // vE
    Line m_line;

    std::shared_ptr<Region> m_region_0 = nullptr; // fL
    std::shared_ptr<Region> m_region_1 = nullptr; // fR
    std::shared_ptr<Edge> m_next = nullptr;       // eN
    std::shared_ptr<Edge> m_prev = nullptr;       // eP
  };

  struct Region
  {
    Point source;
    std::set<std::shared_ptr<Edge>> edges;
  };

  struct RegionsSorter
  {
    bool operator()(const std::shared_ptr<Region>& lhs, const std::shared_ptr<Region>& rhs) const
    {
      if (lhs->source.x < rhs->source.x)
        return true;
      if (lhs->source.x == rhs->source.x)
      {
        if (lhs->source.y < rhs->source.y)
          return true;
      }
      return false;
    }
  };

  struct VertexSorter
  {
    bool operator()(const std::shared_ptr<Vertex>& lhs, const std::shared_ptr<Vertex>& rhs) const
    {
      if (lhs->pt.x < rhs->pt.x)
        return true;
      if (lhs->pt.x == rhs->pt.x)
      {
        if (lhs->pt.y < rhs->pt.y)
          return true;
      }
      return false;
    }
  };

  class Diagram
  {
  public:
    [[nodiscard]] bool Empty() const { return m_regions.empty(); }

    void AddRegion(const std::shared_ptr<Region>& region)
    {
      m_regions.insert(region);
      m_all_edges.insert(region->edges.begin(), region->edges.end());
    }

    void AddEdge(const std::shared_ptr<Edge>& edge)
    {
      m_all_edges.insert(edge);
      auto edge_v0 = edge->GetFirstVertex();
      auto edge_v1 = edge->GetSecondVertex();
      if (edge_v0)
        m_all_vertices.insert(edge_v0);
      if (edge_v1)
        m_all_vertices.insert(edge_v1);
    }

    [[nodiscard]] auto RegionsCount() const { return m_regions.size(); }

    std::shared_ptr<Region> GetFirst() { return *m_regions.begin(); }

    auto begin() { return m_regions.begin(); }
    auto end() { return m_regions.end(); }

    [[nodiscard]] const auto& Regions() const { return m_regions; }
    [[nodiscard]] const auto& Edges() const { return m_all_edges; }
    [[nodiscard]] const auto& Vertices() const { return m_all_vertices; }

    void Clear()
    {
      this->m_regions.clear();
      this->m_all_edges.clear();
    }

  private:
    std::set<std::shared_ptr<Region>, RegionsSorter> m_regions{};
    std::set<std::shared_ptr<Edge>> m_all_edges{};
    std::set<std::shared_ptr<Vertex>, VertexSorter> m_all_vertices{};
  };

  void UpdateDiagram(const glm::dvec2& pt, Diagram& diagram);
};

#endif // PEPPERENGINE_VORONOIGENERATOR_HPP
