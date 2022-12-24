#include "Session.hpp"

#include "Constants.hpp"

#include <Pieces/SPiece.hpp>
#include <glm/glm.hpp>
#include <utility>

namespace
{
  constexpr auto ORIGIN_ARENA_X = 11.0f;
  constexpr auto ORIGIN_ARENA_Y = 11.0f + SCENE_HEIGHT - 2.0f;
  float increment_to_move = 0.0f;

  std::array<glm::vec4, 5> COLORS{ RED, GREEN, YELLOW, BLUE, PURPLE };
}

std::vector<Pepper::Ref<Piece>> Session::m_pieces{};

void Session::Start()
{
  m_pieces.clear();

  Pepper::Ref<Piece> p =
    Pepper::CreateRef<SPiece>(COLORS[Pepper::IntRandom(0, 4)], GridSquare{ 5, 19 });
  AddPiece(p);
}

void Session::AddPiece(Pepper::Ref<Piece> piece)
{
  m_pieces.emplace_back(std::move(piece));
}

void Session::OnUpdate(Pepper::TimeStep ts)
{
  increment_to_move += 20.0f * ts;
  if (increment_to_move < 20.0f)
    return;
  m_pieces.front()->DownIncrement();
  increment_to_move = 0.0f;
}

void Session::OnEachPiece(std::function<void(Pepper::Ref<Piece>)> fun)
{
  std::ranges::for_each(m_pieces, std::move(fun));
}

glm::vec2 Session::ConvertSquare(GridSquare gs)
{
  float x = static_cast<float>(gs.GetColumn()) * CELL_SIDE + ORIGIN_ARENA_X + CELL_SIDE / 2.0f;
  float y = ORIGIN_ARENA_Y - (static_cast<float>(gs.GetRow()) * CELL_SIDE + CELL_SIDE / 2.0f);
  return { x, y };
}
