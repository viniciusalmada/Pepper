#include "Piece.hpp"
/**
 * The S piece
 *                      +----+
 *                      |    |
 *                      | 3  |
 *      +----+----+     +----+----+
 *      |    |    |     |    |    |
 *      | 2  | 3  |     | 1  | 2  |
 * +----+----+----+     +----+----+
 * |    |    |               |    |
 * | 0  | 1  |               | 0  |
 * +----+----+               +----+
 *
 */

uint8_t Piece::GetWidth(Shapes s, Rotation r)
{
  switch (s)
  {
  case Shapes::S:
    switch (r)
    {
    case Rotation::A0:
    case Rotation::A180:
      return 3;
    case Rotation::A90:
    case Rotation::A270:
      return 2;
    }
  case Shapes::Z:
  case Shapes::T:
  case Shapes::O:
  case Shapes::I:
  case Shapes::L:
  case Shapes::J:
    return 0;
  }
  return 0;
}

uint8_t Piece::GetHeight(Shapes s, Rotation r)
{
  switch (s)
  {
  case Shapes::S:
    switch (r)
    {
    case Rotation::A0:
    case Rotation::A180:
      return 2;
    case Rotation::A90:
    case Rotation::A270:
      return 3;
    }
  case Shapes::Z:
  case Shapes::T:
  case Shapes::O:
  case Shapes::I:
  case Shapes::L:
  case Shapes::J:
    return 0;
  }
  return 0;
}

void Piece::UpdateQuads()
{
  switch (m_shape)
  {
  case Shapes::S:
    switch (m_rotation)
    {
    case Rotation::A0:
    case Rotation::A180:
    {
      m_squares[0] = m_origin;
      m_squares[1] = m_origin + GridSquare(1, 0);
      m_squares[2] = m_origin + GridSquare(1, 1);
      m_squares[3] = m_origin + GridSquare(2, 1);
      break;
    }
    case Rotation::A90:
    case Rotation::A270:
    {
      m_squares[0] = m_origin + GridSquare(1, 0);
      m_squares[1] = m_origin + GridSquare(0, 1);
      m_squares[2] = m_origin + GridSquare(1, 1);
      m_squares[3] = m_origin + GridSquare(0, 2);
      break;
    }
    }
  case Shapes::Z:
  case Shapes::T:
  case Shapes::O:
  case Shapes::I:
  case Shapes::L:
  case Shapes::J:
    break;
  }
}

void Piece::MoveDown()
{
  m_origin += { 0, -1 };
  UpdateQuads();
}

void Piece::MoveLeft()
{
  m_origin += { -1, 0 };
  UpdateQuads();
}

void Piece::MoveRight()
{
  m_origin += { +1, 0 };
  UpdateQuads();
}

Piece::Piece(Shapes shape, const glm::vec4& color, const GridSquare& origin, Rotation rot) :
    m_shape(shape),
    m_color(color),
    m_origin(origin),
    m_squares({}),
    m_rotation{ rot }
{
  UpdateQuads();
}

void Piece::Rotate()
{
  auto new_rotation = static_cast<uint32_t>(m_rotation) + 1;
  if (new_rotation > 3)
    m_rotation = Rotation::A0;
  else
    m_rotation = static_cast<Rotation>(new_rotation);
  UpdateQuads();
}
