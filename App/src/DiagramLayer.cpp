// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "DiagramLayer.hpp"

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
  const float top = 0.0;
  const float bot = camera_height;

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
  for (const auto& dot : m_dots_position)
    Pepper::Renderer2D::DrawQuad(dot, { 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
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

      PP_INFO("x={0}, y={1}", Pepper::Input::GetMouseX(), Pepper::Input::GetMouseY());
      m_dots_position.emplace_back(Pepper::Input::GetMouseX(), Pepper::Input::GetMouseY());
      return true;
    });
}
