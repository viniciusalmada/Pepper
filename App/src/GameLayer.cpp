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
  DrawGrid();

  Session::OnEachPiece(
    [](const Ref<Piece>& p)
    {
      const auto& quads = p->GetQuads();
      auto draw_quad = [color = p->GetColor()](GridSquare gs)
      {
        auto position = Session::ConvertSquare(gs);
        Renderer2D::DrawQuad({ position.x, position.y, 0.35f }, { QUAD_SIDE, QUAD_SIDE }, color);
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
