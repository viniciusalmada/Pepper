#ifndef PIECE_HPP
#define PIECE_HPP

#include "GridSquare.hpp"

#include <glm/glm.hpp>

enum class Shapes : uint32_t { S = 0, Z = 1, T = 2, O = 3, I = 4, L = 5, J = 6 };

enum class Rotation : uint32_t { A0 = 0, A90 = 1, A180 = 2, A270 = 3 };

class Piece
{
public:
  Piece(Shapes shape, const glm::vec4& color, const GridSquare& origin, Rotation rot);

  void DownIncrement();

  [[nodiscard]] Shapes GetShape() const { return m_shape; };

  [[nodiscard]] static uint8_t GetWidth(Shapes s, Rotation r);
  [[nodiscard]] static uint8_t GetHeight(Shapes s, Rotation r);

  [[nodiscard]] const std::array<GridSquare, 4>& GetQuads() const { return m_quads; }

  [[nodiscard]] const glm::vec4& GetColor() const { return m_color; }

private:
  void UpdateQuads();

  Shapes m_shape;
  glm::vec4 m_color;
  GridSquare m_origin;
  std::array<GridSquare, 4> m_quads;
  Rotation m_rotation;
};

#endif // PIECE_HPP
