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
  //  Renderer2D::DrawQuad({ 167.0f, 322.0f, 0.1f }, { 294.0f, 584.0f - 0.0f }, Color::WHITE);
  //  Renderer2D::DrawQuad({ 167.0f, 322.0f, 0.2f }, { 290.0f, 580.0f - 0.0f }, Color::BLACK);
  //  Renderer2D::DrawQuad({ 397.0f, 322.0f, 0.1f }, { 126.0f, 584.0f - 0.0f }, Color::WHITE);
  //  Renderer2D::DrawQuad({ 397.0f, 322.0f, 0.2f }, { 122.0f, 580.0f - 0.0f }, Color::BLACK);
  DrawGrid();

  Session::OnEachPiece(
    [](const Ref<Piece>& p)
    {
      const auto& quads = p->GetQuads();
      auto draw_quad = [color = p->GetColor()](GridSquare gs)
      {
        auto position = Session::ConvertSquare(gs);
        Renderer2D::DrawQuad({ position.x, position.y, 0.3f }, { QUAD_SIDE, QUAD_SIDE }, color);
      };

      std::ranges::for_each(quads, draw_quad);
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
}

void GameLayer::DrawGrid()
{
  constexpr uint32_t start_x = 10;
  constexpr uint32_t start_y = WINDOW_HEIGHT - 10;
  constexpr float VERTICAL_POS = WINDOW_HEIGHT / 2.0f;
  constexpr float HORIZONTAL_POS = ((10 * CELL_SIDE) / 2.0f) + start_x;

  for (uint32_t col : std::views::iota(0, 11))
  {
    uint32_t x = start_x + col * (int)CELL_SIDE;
    Renderer2D::DrawQuad({ x, VERTICAL_POS, 0.3f }, { 1.0f, SCENE_HEIGHT }, GREY);
  }

  for (uint32_t row : std::views::iota(0, 21))
  {
    uint32_t y = start_y - row * (int)CELL_SIDE;
    Renderer2D::DrawQuad({ HORIZONTAL_POS, y, 0.3f }, { SCENE_WIDTH, 1.0f }, GREY);
  }
}
