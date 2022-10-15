// clang-format off
#include <PepperPCH.hpp>
#include <Pepper/Events/KeyEvent.hpp>
// clang-format on

#include "DiagramLayer.hpp"

#include "VoronoiGenerator.hpp"

namespace
{
  VoronoiGenerator::Diagram s_diagram;
}

DiagramLayer::DiagramLayer() :
    Layer("DiagramLayer"),
    m_diagram_VAO(Pepper::VertexArray::Create()),
    m_shader_library(),
    m_dots_position()
{
  const float aspect_ratio = 1280.0f / 720.0f;
  const float camera_width = 1280.0f;
  const float camera_height = camera_width / aspect_ratio;

  const float left = 0.0f;
  const float right = camera_width;
  const float top = camera_height;
  const float bot = 0.0;

  m_camera = Pepper::CreateScope<Pepper::OrthoCamera>(Pepper::CameraLimits{ left, right, bot, top });
}

void DiagramLayer::OnAttach()
{
  Layer::OnAttach();
}

void DiagramLayer::OnDetach()
{
  Layer::OnDetach();
}

void DiagramLayer::OnUpdate(Pepper::TimeStep /*step*/)
{
  Pepper::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera);
  for (const auto& region : s_diagram)
  {
    Pepper::Renderer2D::DrawQuad(region->source, { 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
    for (const auto& edge : region->edges)
    {
      auto drawable = edge->GetDrawableEdge({ 1280.0, 720.0 });
      auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0;
      auto length = glm::length(drawable.second - drawable.first);
      auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
      auto angle_deg = static_cast<float>(glm::degrees(angle));
      Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 2.5f }, angle_deg, { 0.0f, 0.0f, 0.0f, 1.0f });
    }
  }

  //  for (const auto& edge : s_diagram.Edges())
  //  {
  //    auto drawable = edge->GetDrawableEdge({ 1280.0, 720.0 });
  //    auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0;
  //    auto length = glm::length(drawable.second - drawable.first);
  //    auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
  //    auto angle_deg = static_cast<float>(glm::degrees(angle));
  //    Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 2.5f }, angle_deg, { 1.0f, 0.2f, 0.2f, 1.0f });
  //  }

  Pepper::Renderer2D::EndScene();
}

void DiagramLayer::OnImGuiRender()
{
  Layer::OnImGuiRender();
}

void DiagramLayer::OnEvent(Pepper::Event& event)
{
  Pepper::EventDispatcher dispatcher{ event };
  dispatcher.Dispatch<Pepper::MouseButtonPressedEvent>(
    [this](Pepper::MouseButtonPressedEvent& ev)
    {
      if (ev.GetMouseButton() != PP_MOUSE_BUTTON_LEFT)
        return false;

      const glm::vec2 pt{ Pepper::Input::GetMouseX(), 720 - Pepper::Input::GetMouseY() };
      PP_INFO("x={0}, y={1}", pt.x, pt.y);
      m_dots_position.emplace_back(pt);
      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      return true;
    });

  dispatcher.Dispatch<Pepper::KeyTypedEvent>(
    [this](Pepper::KeyTypedEvent& ev)
    {
      if (ev.GetKeyCode() != PP_KEY_SPACE)
        return false;

      //      m_dots_position.emplace_back(1167, 69);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(780, 317);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(426, 561);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(450,230);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      //      m_dots_position.emplace_back(338, 149);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(1038, 204);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(686, 563);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      m_dots_position.emplace_back(1036, 117);
      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      m_dots_position.emplace_back(838, 254);
      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      m_dots_position.emplace_back(450, 505);
      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      return true;
    });
}
