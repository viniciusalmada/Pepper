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
    Vertex(const Point& point) : pt(point) {}
    Point pt;
  };

  struct Region;

  // Using edge to guide the DCEL data structure
  // DCEL - Doubly Connected Edge List
  // Region - [Sample, Edge]
  // Vertex - [Vertex, Edge]
  class Edge
  {
  public:
    static std::shared_ptr<Edge> InfiniteEdge(Point midPoint, Point direction)
    {
      return std::make_shared<Edge>(Edge(nullptr, nullptr, direction, midPoint));
    }

    static std::shared_ptr<Edge> SemiInfEdge(std::shared_ptr<Vertex> vtx, Point directionFromVtx)
    {
      return std::make_shared<Edge>(Edge{ std::move(vtx), nullptr, directionFromVtx, {} });
    }

    static std::shared_ptr<Edge> FiniteEdge(std::shared_ptr<Vertex> vtx0, std::shared_ptr<Vertex> vtx1)
    {
      return std::make_shared<Edge>(Edge{ std::move(vtx0), std::move(vtx1), {}, {} });
    }

    [[nodiscard]] std::shared_ptr<Region> GetMate(const std::shared_ptr<Region>& reg) const
    {
      return reg == m_region_L ? m_region_R : m_region_L;
    }

    [[nodiscard]] std::shared_ptr<Vertex> GetFirstVertex() const { return m_v0; }
    [[nodiscard]] std::shared_ptr<Vertex> GetSecondVertex() const { return m_v1; }

    void SetRegionL(std::shared_ptr<Region> region) { m_region_L = std::move(region); }
    void SetRegionR(std::shared_ptr<Region> region) { m_region_R = std::move(region); }

    [[nodiscard]] Line GetFiniteFormEdge(const glm::dvec2& botLeft, const glm::dvec2& topRight) const;

    [[nodiscard]] Line GetLine() const
    {
      if (IsInf())
      {
        auto pt0 = m_mid_point + m_direction / 2.0;
        auto pt1 = m_mid_point - m_direction / 2.0;
        return { pt0, pt1 };
      }
      if (IsSemiInf())
      {
        auto pt0 = m_v0->pt;
        auto pt1 = m_v0->pt + m_direction;
        return { pt0, pt1 };
      }
      return { m_v0->pt, m_v1->pt };
    };

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

    void UpdateInfToSemiInf();

  private:
    Edge(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, Point direction, Point midPoint) :
        m_v0(std::move(v0)),
        m_v1(std::move(v1)),
        m_direction(direction),
        m_mid_point(midPoint)
    {
    }

    std::shared_ptr<Vertex> m_v0; // vB
    std::shared_ptr<Vertex> m_v1; // vE
    Point m_direction;
    Point m_mid_point;

    std::shared_ptr<Region> m_region_L = nullptr; // fL
    std::shared_ptr<Region> m_region_R = nullptr; // fR
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

    [[nodiscard]] auto& Regions() { return m_regions; }
    [[nodiscard]] auto& Edges() { return m_all_edges; }
    [[nodiscard]] auto& Vertices() { return m_all_vertices; }

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
