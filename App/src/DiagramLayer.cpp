// clang-format off
#include <PepperPCH.hpp>
// clang-format on
#include <Pepper/Events/KeyEvent.hpp>

#include "DiagramLayer.hpp"

namespace
{
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
  Pepper::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera);
  Pepper::Renderer2D::EndScene();
}

void DiagramLayer::OnImGuiRender()
{
  Layer::OnImGuiRender();
}

void DiagramLayer::OnEvent(Pepper::Event& /*event*/) {}
