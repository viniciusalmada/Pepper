#include "Session.hpp"

#include "Constants.hpp"

#include <Pieces/Piece.hpp>
#include <glm/glm.hpp>
#include <utility>

/*
 * +-+-+-+-+-+-+-+-+-+-+
 * |0|1|2|3|4|5|6|7|8|9|
 * +-+-+-+-+-+-+-+-+-+-+
 */

namespace
{
  constexpr auto ORIGIN_ARENA_X = 11.0f;
  constexpr auto ORIGIN_ARENA_Y = 11.0f + SCENE_HEIGHT - 2.0f;
  float increment_to_move = 0.0f;

  std::array<glm::vec4, 5> COLORS{ RED, GREEN, YELLOW, BLUE, PURPLE };
  std::map<Shapes, std::string> SHAPES{
    {Shapes::S, "S"},
    {Shapes::Z, "Z"},
    {Shapes::I, "I"},
    {Shapes::J, "J"},
    {Shapes::L, "L"},
    {Shapes::O, "O"},
    {Shapes::T, "T"},
  };
  std::map<Rotation, std::string> ROTATIONS{
    {  Rotation::A0,   "A0"},
    {Rotation::A180, "A180"},
    {Rotation::A270, "A270"},
    { Rotation::A90,  "A90"},
  };
}

void Session::Start()
{
  m_pieces.clear();
  m_squares.clear();

  AddPiece();
}

void Session::AddPiece()
{
  auto shape = Shapes::S;        // Shapes(Pepper::IntRandom(0,6));
  auto rotation = Rotation::A90; // Rotation(Pepper::IntRandom(0, 3));
  auto color = COLORS[Pepper::IntRandom(0, COLORS.size() - 1)];
  auto height = Piece::GetHeight(shape, rotation);
  auto width = Piece::GetWidth(shape, rotation);

  auto column = Pepper::IntRandom(0, 9 - (width - 1));
  auto line = Pepper::IntRandom(0, 9);

  PP_TRACE("shape={0},rot={1},h={2},w={3},col={4},lin={5}",
           SHAPES[shape],
           ROTATIONS[rotation],
           height,
           width,
           column,
           line);

  m_current_piece = Pepper::CreateRef<Piece>(shape, color, GridSquare{ column, 20 }, rotation);
}

void Session::OnUpdate(Pepper::TimeStep ts)
{
  // Check if piece can move down
  auto is_piece_at_zero = m_current_piece->GetOrigin().IsAtZero();
  if (is_piece_at_zero)
  {
    for (const auto& square : m_current_piece->GetQuads())
    {
      m_squares.emplace_back(square, m_current_piece->GetColor());
    }
    AddPiece();
    return;
  }

  auto is_piece_above_others = [this](const GridSquare& standingGS)
  {
    const auto& quads = m_current_piece->GetQuads();
    return std::ranges::all_of(quads,
                               [&standingGS](const GridSquare& quad)
                               { return quad.IsAbove(standingGS); });
  };

  if (!std::ranges::all_of(m_squares | std::views::elements<0>, is_piece_above_others))
  {
    for (const auto& square : m_current_piece->GetQuads())
    {
      m_squares.emplace_back(square, m_current_piece->GetColor());
    }
    AddPiece();
    return;
  }

  increment_to_move += 20.0f * ts;
  if (increment_to_move >= 20.0f)
  {
    m_current_piece->DownIncrement();
    increment_to_move = 0.0f;
  }
}

void Session::OnCurrentPiece(const std::function<void(Pepper::Ref<Piece>)>& fun)
{
  fun(m_current_piece);
}

void Session::OnEachSquare(std::function<void(std::pair<GridSquare, glm::vec4>)> fun)
{
  std::ranges::for_each(m_squares, std::move(fun));
}

glm::vec2 Session::ConvertSquare(GridSquare gs)
{
  if (gs.GetRow() >= 20)
    return { std::nan(""), std::nan("") };
  float x = static_cast<float>(gs.GetColumn()) * CELL_SIDE + ORIGIN_ARENA_X + CELL_SIDE / 2.0f;
  float y = ORIGIN_ARENA_Y - (static_cast<float>(gs.GetRow()) * CELL_SIDE + CELL_SIDE / 2.0f);
  return { x, y };
}
