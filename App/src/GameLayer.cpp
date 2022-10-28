// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "GameLayer.hpp"

#include "Pepper/Events/KeyEvent.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtx/color_space.hpp>
#include <imgui.h>
#include <numeric>

//---------------------------------------------------------------------------------------------------------------------
GameLayer::GameLayer() :
    Layer("GameLayer"),
    m_camera(),
    m_clear_color_hsv({ 0.0f, 0.3f, 0.1f }),
    m_state(GameState::SPLASH),
    m_level()
{
  PP_PROFILE_FUNCTION()
  auto& window = Pepper::Application::Get().GetWindow();
  CreateCamera(window.GetWidth(), window.GetHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnAttach()
{
  PP_PROFILE_FUNCTION()
  m_level.SetGameOverCallback(
    [this](uint32_t score)
    {
      m_state = GameState::GAME_OVER;
      m_game_over_score = score;
    });
  m_level.Init();

  ImGuiIO io = ImGui::GetIO();
  m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/ARCADECLASSIC.TTF", 48);
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnDetach() {}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION()
  m_clear_color_hsv.x += 0.1f * 360.f * ts;
  if (m_clear_color_hsv.x > 360.0f)
    m_clear_color_hsv.x = 0.0f;

  auto clear_color_rgb = glm::rgbColor(m_clear_color_hsv);
  Pepper::RenderCommand::SetClearColor(
    { clear_color_rgb.r, clear_color_rgb.g, clear_color_rgb.b, 1.0f });
  Pepper::RenderCommand::Clear();

  if (m_state != GameState::PLAYING)
    return;

  m_level.OnUpdate(ts);

  const auto& ref_pos = m_level.GetPlayerPosition();
  m_camera->SetPosition({ ref_pos.x, ref_pos.y / 2.0f, 0.0f });

  Pepper::Renderer2D::BeginScene(*m_camera.get());
  m_level.OnRendererCall();
  Pepper::Renderer2D::EndScene();
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnImGuiRender()
{
  PP_PROFILE_FUNCTION()
  ImGui::Begin("Settings");
  m_level.OnImGuiRender();
  ImGui::End();

  int xpos, ypos;
  glfwGetWindowPos(
    std::any_cast<GLFWwindow*>(Pepper::Application::Get().GetWindow().GetNativeWindow()),
    &xpos,
    &ypos);

  switch (m_state)
  {
  case GameState::SPLASH:
  {
    ImGui::GetForegroundDrawList()->AddText(
      m_font,
      48,
      { static_cast<float>(xpos) + 1280 / 2.0f - 515.0f / 2.0f,
        static_cast<float>(ypos) + 720 / 2.0f },
      0xFFFFFFFF,
      "Press  SPACE  to  start!",
      nullptr,
      515.0f);
    break;
  }
  case GameState::PLAYING:
  {
    uint32_t player_score = m_level.GetPlayerScore();
    std::string score = "Score: " + std::to_string(player_score);

    ImGui::GetForegroundDrawList()->AddText(
      m_font,
      24,
      { static_cast<float>(xpos + 100), static_cast<float>(ypos + 40) },
      0xFFFFFFFF,
      score.c_str());
    break;
  }
  case GameState::GAME_OVER:
  {
    auto text = "Game  Over!\n"
                "Your  Score:  " +
                std::to_string(m_game_over_score) +
                "\n"
                "Press  ENTER  to  restart.";
    ImGui::GetForegroundDrawList()->AddText(
      m_font,
      48,
      { static_cast<float>(xpos) + 1280 / 2.0f - 515.0f / 2.0f,
        static_cast<float>(ypos) + 720 / 2.0f - 50.0f },
      0xFFFFFFFF,
      text.c_str(),
      nullptr,
      700.0f);
    break;
  }
  }
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::OnEvent(Pepper::Event& e)
{
  PP_PROFILE_FUNCTION()
  Pepper::EventDispatcher disp{ e };
  disp.Dispatch<Pepper::WindowResizeEvent>(
    [this](auto&& PH1) -> bool { return OnWindowResize(std::forward<decltype(PH1)>(PH1)); });
  disp.Dispatch<Pepper::KeyPressedEvent>(
    [this](Pepper::KeyPressedEvent& pressedEvent) -> bool
    {
      bool splash_check = m_state == GameState::SPLASH && pressedEvent.GetKeyCode() == PP_KEY_SPACE;
      bool go_check = m_state == GameState::GAME_OVER && pressedEvent.GetKeyCode() == PP_KEY_ENTER;

      if (splash_check || go_check)
      {
        m_level.Restart();
        m_state = GameState::PLAYING;
        return true;
      }
      return false;
    });
}

//---------------------------------------------------------------------------------------------------------------------
void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
  PP_PROFILE_FUNCTION()
  const float aspect_ratio = (float)width / (float)height;
  const float camera_width = 20.0f;
  const float camera_height = camera_width / aspect_ratio;

  const float left = -camera_width / 2.0f;
  const float right = camera_width / 2.0f;
  const float top = camera_height / 2.0f;
  const float bot = -camera_height / 2.0f;

  m_camera =
    Pepper::CreateScope<Pepper::OrthoCamera>(Pepper::CameraLimits{ left, right, bot, top });
}

//---------------------------------------------------------------------------------------------------------------------
bool GameLayer::OnWindowResize(Pepper::WindowResizeEvent& e)
{
  PP_PROFILE_FUNCTION()
  CreateCamera(e.GetWidth(), e.GetHeight());
  if (e.GetHeight() != 0 || e.GetWidth() != 0)
    return false;
  return true;
}
