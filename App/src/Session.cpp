#include "Session.hpp"

#include "Constants.hpp"

#include <Pieces/SPiece.hpp>
#include <glm/glm.hpp>
#include <utility>

namespace
{
  constexpr auto ORIGIN_ARENA_X = 22.0f;
  constexpr auto ORIGIN_ARENA_Y = 618.0f;
  float increment_to_move = 0.0f;
}

std::vector<Pepper::Ref<Piece>> Session::m_pieces{};

void Session::Start()
{
  m_pieces.clear();

  Pepper::Ref<Piece> p = Pepper::CreateRef<SPiece>(RED, GridSquare{ 5, 19 });
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
