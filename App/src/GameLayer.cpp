// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "GameLayer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

//---------------------------------------------------------------------------------------------------------------------
GameLayer::GameLayer() : Layer("GameLayer"), m_camera()
{
  auto& window = Pepper::Application::Get().GetWindow();
  CreateCamera(window.GetWidth(), window.GetHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnAttach()
{
  m_level.Init();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnDetach() {}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnUpdate(Pepper::TimeStep ts)
{
  m_level.OnUpdate(ts);

  const auto& ref_pos = m_level.GetPlayerPosition();
  m_camera->SetPosition({ ref_pos.x, 0.0f, 0.0f });

  Pepper::RenderCommand::SetClearColor({ 1.0f, 1.f, 1.f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera.get());
  m_level.OnRendererCall();
  Pepper::Renderer2D::EndScene();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnImGuiRender()
{
  m_level.OnImGuiRender();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnEvent(Pepper::Event& e)
{
  Pepper::EventDispatcher disp{ e };
  disp.Dispatch<Pepper::WindowResizeEvent>(std::bind(&GameLayer::OnWindowResize, this, std::placeholders::_1));
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
  const float aspect_ratio = (float)width / (float)height;
  const float camera_width = 20.0f;
  const float camera_height = camera_width / aspect_ratio;

  const float left = -camera_width / 2.0f;
  const float right = camera_width / 2.0f;
  const float top = camera_height / 2.0f;
  const float bot = -camera_height / 2.0f;

  m_camera = Pepper::CreateScope<Pepper::OrthoCamera>(Pepper::CameraLimits{ left, right, bot, top });
}

//---------------------------------------------------------------------------------------------------------------------
bool GameLayer::OnWindowResize(Pepper::WindowResizeEvent& e)
{
  CreateCamera(e.GetWidth(), e.GetHeight());
  return false;
}
