#ifndef PEPPERENGINE_DIAGRAMLAYER_HPP
#define PEPPERENGINE_DIAGRAMLAYER_HPP

#include <Pepper.hpp>

struct PointsCmp
{
  bool operator()(const glm::vec2& lhs, const glm::vec2 rhs) const
  {
    if (lhs.x < rhs.x)
      return true;

    if (lhs.x == rhs.x)
    {
      if (lhs.y < rhs.y)
        return true;
    }
    return false;
  }
};

class DiagramLayer : public Pepper::Layer
{
public:
  DiagramLayer();

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(Pepper::TimeStep step) override;
  void OnImGuiRender() override;
  void OnEvent(Pepper::Event& event) override;

private:
  Pepper::Scope<Pepper::OrthoCamera> m_camera;
  std::set<glm::vec2, PointsCmp> m_dots_position;

  Pepper::Ref<Pepper::VertexArray> m_diagram_VAO;

  Pepper::ShaderLibrary m_shader_library;
};

#endif // PEPPERENGINE_DIAGRAMLAYER_HPP
