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
};

GameLayer::GameLayer() : Layer("GameLayer"), impl(CreateScope<Implementation>()) {}

GameLayer::~GameLayer() = default;

void GameLayer::OnAttach()
{
  PP_INFO("GameLayer attached!");
}

void GameLayer::OnUpdate(TimeStep ts)
{
  Session::OnUpdate(ts);

  RenderCommand::SetClearColor(Color::BLACK);
  RenderCommand::Clear();

  Renderer2D::BeginScene(CAMERA);
  Renderer2D::DrawQuad({ 167.0f, 320.0f, 0.1f }, { 294.0f, 600.0f }, Color::WHITE);
  Renderer2D::DrawQuad({ 167.0f, 320.0f, 0.2f }, { 290.0f, 596.0f }, Color::BLACK);
  Renderer2D::DrawQuad({ 397.0f, 320.0f, 0.1f }, { 126.0f, 600.0f }, Color::WHITE);
  Renderer2D::DrawQuad({ 397.0f, 320.0f, 0.2f }, { 122.0f, 596.0f }, Color::BLACK);
  DrawGrid();

  Session::OnEachPiece(
    [](const Ref<Piece>& p)
    {
      const auto& quads = p->GetQuads();
      Renderer2D::DrawQuad({ quads[0].x, quads[0].y, 0.4f },
                           { QUAD_SIDE, QUAD_SIDE },
                           p->GetColor());
      Renderer2D::DrawQuad({ quads[1].x, quads[1].y, 0.4f },
                           { QUAD_SIDE, QUAD_SIDE },
                           p->GetColor());
      Renderer2D::DrawQuad({ quads[2].x, quads[2].y, 0.4f },
                           { QUAD_SIDE, QUAD_SIDE },
                           p->GetColor());
      Renderer2D::DrawQuad({ quads[3].x, quads[3].y, 0.4f },
                           { QUAD_SIDE, QUAD_SIDE },
                           p->GetColor());
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
      impl->mouse_y = WINDOW_HEIGHT - (int)mouseEvent.GetY();
      return true;
    });
}

void GameLayer::DrawGrid()
{
  uint32_t start_x = 22;
  for (uint32_t col : std::views::iota(1, 10))
  {
    uint32_t x = start_x + col * 29;
    Renderer2D::DrawQuad({ x, 320.0f, 0.3f }, { 1.0f, 596.0f }, GREY);
  }

  uint32_t start_y = 22;
  for (uint32_t row : std::views::iota(1, 21))
  {
    uint32_t y = start_y + row * 29;
    Renderer2D::DrawQuad({ 167.0f, y, 0.3f }, { 290.0f, 1.0f }, GREY);
  }
}
