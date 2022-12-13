#include "SPiece.hpp"

#include <Constants.hpp>

/*
 *                     +----+
 *                     |    |
 *                     | 3  |
 *      +----+----+    +----+----+
 *      | 2  | 3  |    |    |    |
 *      |    |    |    | 1  | 2  |
 * +----+----+----+    +----+----+
 * |    |    |              |    |
 * | 0  | 1  |              | 0  |
 * +----+----+              +----+
 */

Shapes SPiece::GetShape() const
{
  return Shapes::S;
}

SPiece::SPiece(glm::vec4 color, GridSquare origin) : Piece(color, origin)
{
  this->UpdateQuads();
}

void SPiece::UpdateQuads()
{
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
    m_quads[0] = m_origin;
    m_quads[1] = m_origin + GridSquare(-1, 1);
    m_quads[2] = m_origin + GridSquare(0, 1);
    m_quads[3] = m_origin + GridSquare(-1, 2);
    break;
  }
  }
}
