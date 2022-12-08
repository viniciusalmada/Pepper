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

SPiece::SPiece(glm::vec4 color, glm::vec2 origin) : Piece(color, origin)
{
  UpdateQuads();
}

void SPiece::UpdateQuads()
{
  switch (m_rotation)
  {
  case Rotation::A0:
  case Rotation::A180:
  {
    m_quads[0] = m_origin + glm::vec2{ 1 * HALF_QUAD_SIDE, 1 * HALF_QUAD_SIDE };
    m_quads[1] = m_origin + glm::vec2{ 3 * HALF_QUAD_SIDE, 1 * HALF_QUAD_SIDE };
    m_quads[2] = m_origin + glm::vec2{ 3 * HALF_QUAD_SIDE, 3 * HALF_QUAD_SIDE };
    m_quads[3] = m_origin + glm::vec2{ 5 * HALF_QUAD_SIDE, 3 * HALF_QUAD_SIDE };
    break;
  }
  case Rotation::A90:
  case Rotation::A270:
  {
    m_quads[0] = m_origin + glm::vec2{ 3 * HALF_QUAD_SIDE, 1 * HALF_QUAD_SIDE };
    m_quads[1] = m_origin + glm::vec2{ 1 * HALF_QUAD_SIDE, 3 * HALF_QUAD_SIDE };
    m_quads[2] = m_origin + glm::vec2{ 3 * HALF_QUAD_SIDE, 3 * HALF_QUAD_SIDE };
    m_quads[3] = m_origin + glm::vec2{ 1 * HALF_QUAD_SIDE, 5 * HALF_QUAD_SIDE };
    break;
  }
  }
}
