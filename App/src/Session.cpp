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
  const float INCREMENT = 50.0f;
  const float VELOCITY = 20.0f;
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
  m_locked_squares.clear();

  AddPiece();
}

void Session::AddPiece()
{
  auto shape = Shapes::S; // Shapes(Pepper::IntRandom(0,6));
  auto rotation =
    Pepper::IntRandom(0, 1) ? Rotation::A90 : Rotation::A0; // Rotation(Pepper::IntRandom(0, 3));
  auto color = COLORS[Pepper::IntRandom(1, COLORS.size()) - 1];
  auto height = Piece::GetHeight(shape, rotation);
  auto width = Piece::GetWidth(shape, rotation);

  auto column = (10 - width) / 2;
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
  increment_to_move += INCREMENT * ts;
  if (increment_to_move >= VELOCITY)
  {
    m_current_piece->MoveDown();
    increment_to_move = 0.0f;
    return;
  }

  auto is_piece_at_zero = m_current_piece->GetOrigin().IsAtZero();
  if (is_piece_at_zero)
  {
    LockPiece();
    AddPiece();
    return;
  }

  bool is_piece_above_others = true;
  for (auto& [locked_square, _] : m_locked_squares)
  {
    auto& piece_squares = m_current_piece->GetSquares();
    for (auto& piece_square : piece_squares)
    {
      if (piece_square.GetColumn() != locked_square.GetColumn())
        continue;

      if (piece_square.GetRow() != locked_square.GetRow() + 1)
        continue;

      is_piece_above_others = false;
    }
    if (!is_piece_above_others)
      break;
  }

  if (!is_piece_above_others)
  {
    LockPiece();
    AddPiece();
    return;
  }
}
void Session::LockPiece()
{
  for (const auto& square : m_current_piece->GetSquares())
  {
    m_locked_squares.emplace_back(square, m_current_piece->GetColor());
  }
}

void Session::OnCurrentPiece(const std::function<void(Pepper::Ref<Piece>)>& fun)
{
  fun(m_current_piece);
}

void Session::OnEachSquare(const std::function<void(std::pair<GridSquare, glm::vec4>)>& fun)
{
  std::ranges::for_each(m_locked_squares, fun);
}

glm::vec2 Session::ConvertSquare(GridSquare gs)
{
  if (gs.GetRow() >= 20)
    return { std::nan(""), std::nan("") };
  float x = static_cast<float>(gs.GetColumn()) * CELL_SIDE + ORIGIN_ARENA_X + CELL_SIDE / 2.0f;
  float y = ORIGIN_ARENA_Y - (static_cast<float>(gs.GetRow()) * CELL_SIDE + CELL_SIDE / 2.0f);
  return { x, y };
}

void Session::MoveLeft()
{
  m_current_piece->MoveLeft();
}

void Session::MoveRigth()
{
  m_current_piece->MoveRight();
}

void Session::DownPiece()
{
  m_current_piece->MoveDown();
}

void Session::RotatePiece()
{
  m_current_piece->Rotate();
}
