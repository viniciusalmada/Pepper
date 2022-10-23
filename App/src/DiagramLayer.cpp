// clang-format off
#include <PepperPCH.hpp>
// clang-format on
#include "DiagramLayer.hpp"

#include "GridVoronoi/GridVoronoi.hpp"
#include "Random.hpp"

#include <Pepper/Events/KeyEvent.hpp>
#include <imgui.h>

namespace
{
  Pepper::Scope<GridVoronoi> s_grid = nullptr;
  auto divisors = 4;
  auto point_size = (float)divisors;
  auto nodes_number = 4;
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
  CreatePoints();
  //  m_dots_position = { { 162, 600 }, { 299, 349 }, { 420, 505 }, { 1206, 490 } };
  //    m_dots_position = { { 0, 0 }, { 100, 100 }, { 100, 0 }, { 0, 100 } };

  s_grid = std::make_unique<GridVoronoi>(GridVoronoi{ 1280, 720, m_dots_position });
  s_grid->Update(divisors, m_dots_position);
  s_grid->GenerateIntersectionNodes();
}

void DiagramLayer::CreatePoints()
{
  m_dots_position.clear();
  for ([[maybe_unused]] int i : std::views::iota(0, nodes_number))
  {
    Point pt{ Random::Int(0, 1280), Random::Int(0, 720) };
    m_dots_position.insert(pt);
  }
  //  if (s_grid)
  //    s_grid->Update(divisors, m_dots_position);
}

void DiagramLayer::OnAttach()
{
  Layer::OnAttach();
}

void DiagramLayer::OnDetach()
{
  Layer::OnDetach();
}

auto render_pt_color = [](const auto& pt, const auto& color) {
  Pepper::Renderer2D::DrawQuad(pt, { point_size, point_size }, glm::vec4{ color.r, color.g, color.b, 1.0f });
};

void DiagramLayer::OnUpdate(Pepper::TimeStep /*step*/)
{
  Pepper::RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera);
  //  s_grid->OnParticle(render_pt_color);
  s_grid->OnInterPt(render_pt_color);

  std::for_each(
    m_dots_position.begin(),
    m_dots_position.end(),
    [](const auto& pt) {
      Pepper::Renderer2D::DrawQuad({ pt.x, pt.y, 0.2f }, { 8.0f, 8.0f }, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
    });
  Pepper::Renderer2D::EndScene();
}

void DiagramLayer::OnImGuiRender()
{
  //  Layer::OnImGuiRender();
  //  ImGui::DragInt("Divisors", &divisors, 1.0f, 2, 100);
  //  ImGui::DragFloat("PointSize", &point_size, 1.0f, 1, 50);
  //  ImGui::DragInt("Number nodes", &nodes_number, 1.0f, 2, 100);
  //  if (ImGui::Button("Confirm nodes"))
  //    CreatePoints();
}

void DiagramLayer::OnEvent(Pepper::Event& event)
{
  auto event_dispatcher = Pepper::EventDispatcher(event);
  event_dispatcher.Dispatch<Pepper::MouseButtonPressedEvent>(
    [](const Pepper::MouseButtonPressedEvent&)
    {
      PP_INFO("{0},{1}", Pepper::Input::GetMouseX(), 720 - Pepper::Input::GetMouseY());
      return true;
    });
}
