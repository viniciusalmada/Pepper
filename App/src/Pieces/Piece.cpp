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
      m_quads[0] = m_origin;
      m_quads[1] = m_origin + GridSquare(1, 0);
      m_quads[2] = m_origin + GridSquare(1, 1);
      m_quads[3] = m_origin + GridSquare(2, 1);
      break;
    }
    case Rotation::A90:
    case Rotation::A270:
    {
      m_quads[0] = m_origin + GridSquare(1, 0);
      m_quads[1] = m_origin + GridSquare(0, 1);
      m_quads[2] = m_origin + GridSquare(1, 1);
      m_quads[3] = m_origin + GridSquare(0, 2);
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

void Piece::DownIncrement()
{
  m_origin += { 0, -1 };
  UpdateQuads();
}

Piece::Piece(Shapes shape, const glm::vec4& color, const GridSquare& origin, Rotation rot) :
    m_shape(shape),
    m_color(color),
    m_origin(origin),
    m_quads({}),
    m_rotation{ rot }
{
  UpdateQuads();
}
