// clang-format off
#include <PepperPCH.hpp>
#include <Pepper/Events/KeyEvent.hpp>
// clang-format on

#include "DiagramLayer.hpp"

#include "Random.hpp"

// #include "VoronoiGenerator.hpp"
#include "DCEL/dcel.hpp"
// #include <VoronoiFortune/VoronoiDiagramGenerator.h>

namespace
{
  //  VoronoiDiagramGenerator* s_diagram;
  Diagram* s_diagram = nullptr;
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

#if 0
  for ([[maybe_unused]] int i : std::views::iota(0,10'000'000)){
    float x = Random::Float(0.0f,1280.0f);
    float y = Random::Float(0.0f,720.0f);
    m_dots_position.emplace(x,y);
  }
  std::vector<float> x_values;
  x_values.reserve(m_dots_position.size());
  std::vector<float> y_values;
  x_values.reserve(m_dots_position.size());
  for (auto& dot : m_dots_position)
  {
    x_values.push_back(static_cast<float>(dot.x));
    y_values.push_back(static_cast<float>(dot.y));
  }
  PP_PROFILE_BEGIN_SESSION("Voronoi.txt");
  s_diagram = new VoronoiDiagramGenerator();
  {
    PP_PROFILE_SCOPE("generateVoronoi");
    s_diagram->generateVoronoi(x_values.data(),
                              y_values.data(),
                              static_cast<int>(m_dots_position.size()),
                              0.0f,
                              1280.0f,
                              0.0f,
                              720.0f);
  }
  delete s_diagram;
  PP_PROFILE_END_SESSION();
  m_dots_position.clear();
#endif
  //  s_diagram = new VoronoiDiagramGenerator();
  s_diagram = Diagram::SimpleDiagram({ { 0, 0 }, { 1280, 720 } });
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
  Pepper::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera);
  for (const auto& pt : m_dots_position)
  {
    Pepper::Renderer2D::DrawQuad(pt, { 4.0f, 4.0f }, { 1.0f, 0.2f, 0.2f, 1.0f });
  }

  for (const auto& edge : s_diagram->Edges())
  {
    if (!edge->IsStraight())
      continue;
    std::pair<glm::vec2, glm::vec2> drawable{ edge->v_beg->pt, edge->v_end->pt };
    auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0f;
    auto length = glm::length(drawable.second - drawable.first);
    auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
    auto angle_deg = static_cast<float>(glm::degrees(angle));
    Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 1.5f }, angle_deg, { 0.2f, 1.0f, 0.2f, 1.0f });
  }

  /*float x1, y1, x2, y2;
  s_diagram->resetIterator();
  while (s_diagram->getNext(x1, y1, x2, y2))
  {
    std::pair<glm::vec2, glm::vec2> drawable{ { x1, y1 }, { x2, y2 } };
    auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0f;
    auto length = glm::length(drawable.second - drawable.first);
    auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
    auto angle_deg = static_cast<float>(glm::degrees(angle));
    Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 1.5f }, angle_deg, { 0.2f, 1.0f, 0.2f, 1.0f });
  }*/

  /*for (const auto& region : s_diagram.regions)
  {
    if (region->infinite)
      continue;
    Pepper::Renderer2D::DrawQuad(region->pt, { 10.0f, 10.0f }, { 1.0f, 0.2f, 0.2f, 1.0f });
  }*/

  /*for (const auto& edge : s_diagram.edges)
  {
    //    if (edge->rL->infinite)
    //      continue;
    //    if (edge->rR->infinite)
    //      continue;
    if (edge->Imcomplete())
      continue;
    std::pair<Point, Point> drawable{ edge->vB->pt, edge->vE->pt };
    auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0;
    auto length = glm::length(drawable.second - drawable.first);
    auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
    auto angle_deg = static_cast<float>(glm::degrees(angle));
    if (edge->rL->infinite || edge->rR->infinite)
      Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 1.5f }, angle_deg, { 0.2f, 1.0f, 0.2f, 1.0f });
    else
      Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 2.5f }, angle_deg, { 1.0f, 1.0f, 1.0f, 1.0f });
  }*/

  //  for (const auto& region : s_diagram)
  //  {
  //    Pepper::Renderer2D::DrawQuad(region->source, { 5.0f, 5.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
  //    for (const auto& edge : region->edges)
  //    {
  //      auto drawable = edge->GetFiniteFormEdge({ 0.0, 0.0 }, { 1280.0, 720.0 });
  //      auto mid_pt = drawable.first + (drawable.second - drawable.first) / 2.0;
  //      auto length = glm::length(drawable.second - drawable.first);
  //      auto angle = glm::atan((drawable.second.y - drawable.first.y) / (drawable.second.x - drawable.first.x));
  //      auto angle_deg = static_cast<float>(glm::degrees(angle));
  //      Pepper::Renderer2D::DrawRotatedQuad(mid_pt, { length, 2.5f }, angle_deg, { 1.0f, 1.0f, 1.0f, 1.0f });
  //    }
  //  }

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
      m_dots_position.insert(pt);
      s_diagram->UpdateDiagram(pt);

      /*  if (m_dots_position.size() < 2)
        return true;

     std::vector<float> x_values;
      x_values.reserve(m_dots_position.size());
      std::vector<float> y_values;
      x_values.reserve(m_dots_position.size());
      for (auto& dot : m_dots_position)
      {
        x_values.push_back(static_cast<float>(dot.x));
        y_values.push_back(static_cast<float>(dot.y));
      }
      s_diagram
        ->GenerateVoronoi(x_values, y_values, static_cast<int>(m_dots_position.size()), 0.0f, 1280.0f, 0.0f, 720.0f);*/
      //      UpdateDiagram(s_diagram, m_dots_position.back());
      return true;
    });

  dispatcher.Dispatch<Pepper::KeyTypedEvent>(
    [this](Pepper::KeyTypedEvent& ev)
    {
      if (ev.GetKeyCode() != PP_KEY_SPACE)
        return false;

      //        m_dots_position.emplace_back(1167, 69);
      //        VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //        m_dots_position.emplace_back(780, 317);
      //        VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //        m_dots_position.emplace_back(426, 561);
      //        VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //        m_dots_position.emplace_back(450,230);
      //        VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      //      m_dots_position.emplace_back(338, 149);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(1038, 204);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(686, 563);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(820, 160);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      //      m_dots_position.emplace_back(1036, 117);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(838, 254);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);
      //      m_dots_position.emplace_back(450, 505);
      //      VoronoiGenerator::UpdateDiagram(m_dots_position.back(), s_diagram);

      //      UpdateDiagram(s_diagram, { 1084, 201 });

      m_dots_position.insert({ 340, 470 });
      s_diagram->UpdateDiagram({ 340, 470 });
      m_dots_position.insert({ 740, 480 });
      s_diagram->UpdateDiagram({ 740, 480 });
      m_dots_position.insert({ 470, 190 });
      s_diagram->UpdateDiagram({ 470, 190 });

      return true;
    });
}
