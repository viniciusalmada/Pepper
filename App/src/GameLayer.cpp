// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "GameLayer.hpp"

#include "Random.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <numeric>

//---------------------------------------------------------------------------------------------------------------------
GameLayer::GameLayer() : Layer("GameLayer"), m_camera(), m_ts(0.0f)
{
  PP_PROFILE_FUNCTION();
  auto& window = Pepper::Application::Get().GetWindow();
  CreateCamera(window.GetWidth(), window.GetHeight());

  Random::Init();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnAttach()
{
  PP_PROFILE_FUNCTION();
  m_level.Init();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnDetach() {}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION();
  m_ts = ts;

  float fps = 1000.0f / ts.GetMilliSeconds();
  m_FPSs[m_FPS_id++] = fps;
  if (m_FPS_id == m_FPSs.size())
    m_FPS_id = 0;

  m_level.OnUpdate(ts);

  const auto& ref_pos = m_level.GetPlayerPosition();
  m_camera->SetPosition({ ref_pos.x, 0.0f, 0.0f });

  Pepper::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(*m_camera.get());
  m_level.OnRendererCall();
  Pepper::Renderer2D::EndScene();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnImGuiRender()
{
  PP_PROFILE_FUNCTION();
  ImGui::LabelText("Timeframe", "%1.3fms", m_ts.GetMilliSeconds());
  ImGui::LabelText("FPS", "%3.2f", 1000.0f / m_ts.GetMilliSeconds());
  ImGui::LabelText("FPS average", "%3.0f", std::accumulate(m_FPSs.begin(), m_FPSs.end(), 0.0) / m_FPSs.size());
  m_level.OnImGuiRender();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnEvent(Pepper::Event& e)
{
  PP_PROFILE_FUNCTION();
  Pepper::EventDispatcher disp{ e };
  disp.Dispatch<Pepper::WindowResizeEvent>(std::bind(&GameLayer::OnWindowResize, this, std::placeholders::_1));
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
  PP_PROFILE_FUNCTION();
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
  PP_PROFILE_FUNCTION();
  CreateCamera(e.GetWidth(), e.GetHeight());
  return false;
}
