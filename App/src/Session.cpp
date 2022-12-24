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

std::vector<Pepper::Ref<Piece>> Session::m_pieces{};

void Session::Start()
{
  m_pieces.clear();

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

  auto piece = Pepper::CreateRef<Piece>(shape, color, GridSquare{ column, 20 }, rotation);
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
  if (gs.GetRow() >= 20)
    return { std::nan(""), std::nan("") };
  float x = static_cast<float>(gs.GetColumn()) * CELL_SIDE + ORIGIN_ARENA_X + CELL_SIDE / 2.0f;
  float y = ORIGIN_ARENA_Y - (static_cast<float>(gs.GetRow()) * CELL_SIDE + CELL_SIDE / 2.0f);
  return { x, y };
}
