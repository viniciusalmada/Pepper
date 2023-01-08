#include "GameLayer.hpp"

#include "Constants.hpp"
#include "Pieces/Piece.hpp"
#include "Session.hpp"

#include <imgui.h>

using namespace Pepper;

struct GameLayer::Implementation
{
  int mouse_x = 0;
  int mouse_y = 0;
  Session session;
};

GameLayer::GameLayer() : Layer("GameLayer"), impl(CreateScope<Implementation>()) {}

GameLayer::~GameLayer() = default;

void GameLayer::OnAttach()
{
  PP_INFO("GameLayer attached!");
  impl->session.Start();
}

void GameLayer::OnUpdate(TimeStep ts)
{
#ifdef ENABLE_POLLING
  if (Pepper::Input::IsKeyPressed(PP_KEY_LEFT))
    impl->session.MoveLeft(ts);
  else if (Pepper::Input::IsKeyPressed(PP_KEY_RIGHT))
    impl->session.MoveRight(ts);
  else if (Pepper::Input::IsKeyPressed(PP_KEY_DOWN))
    impl->session.DownPiece(ts);
#endif

  impl->session.OnUpdate(ts);

  RenderCommand::SetClearColor(Color::BLACK);
  RenderCommand::Clear();

  Renderer2D::BeginScene(CAMERA);
  DrawGrid();

  impl->session.OnEachSquare(
    [](std::pair<GridSquare, glm::vec4> squareAndColor)
    {
      auto position = Session::ConvertSquare(squareAndColor.first);
      Renderer2D::DrawQuad({ position.x, position.y, 0.35f },
                           { QUAD_SIDE, QUAD_SIDE },
                           squareAndColor.second);
    });

  impl->session.OnCurrentPiece(
    [](const Pepper::Ref<Piece>& piece)
    {
      for (const auto& gs : piece->GetSquares())
      {
        auto position = Session::ConvertSquare(gs);
        Renderer2D::DrawQuad({ position.x, position.y, 0.35f },
                             { QUAD_SIDE, QUAD_SIDE },
                             piece->GetColor());
      }
    });

  Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
  ImGui::Begin("Debug");
  ImGui::Text("X=%3d Y=%3d", impl->mouse_x, impl->mouse_y);
  ImGui::End();
}

void GameLayer::OnEvent(Event& event)
{
  EventDispatcher dispatcher{ event };
  dispatcher.Dispatch<MouseMovedEvent>(
    [&](MouseMovedEvent& mouseEvent)
    {
      impl->mouse_x = (int)mouseEvent.GetX();
      impl->mouse_y = (int)mouseEvent.GetY();
      return true;
    });

  dispatcher.Dispatch<KeyPressedEvent>(
    [&](KeyPressedEvent& keyEvent)
    {
      if (keyEvent.GetKeyCode() == PP_KEY_LEFT)
        impl->session.MoveLeft(0);
      else if (keyEvent.GetKeyCode() == PP_KEY_RIGHT)
        impl->session.MoveRight(0);
      else if (keyEvent.GetKeyCode() == PP_KEY_DOWN)
        impl->session.DownPiece(0);
      else if (keyEvent.GetKeyCode() == PP_KEY_UP)
        impl->session.RotatePiece();
      return true;
    });
}

void GameLayer::DrawGrid()
{
  glm::vec3 center_scene{ SCENE_WIDTH / 2.0f + 10.0f, SCENE_HEIGHT / 2.0f + 10.0f, 0.25f };
  Renderer2D::DrawQuad(center_scene, { SCENE_WIDTH, SCENE_HEIGHT }, GREY);
  for (uint32_t row_id : std::views::iota(0, 20))
  {
    float row_y = (float)row_id * CELL_SIDE + HALF_CELL_SIDE + 12.0f;
    for (uint32_t col_id : std::views::iota(0, 10))
    {
      float col_x = (float)col_id * CELL_SIDE + HALF_CELL_SIDE + 12.0f;
      Renderer2D::DrawQuad({ col_x, row_y, 0.3f }, { QUAD_SIDE, QUAD_SIDE }, Color::BLACK);
    }
  }
}
