#ifndef PIECE_HPP
#define PIECE_HPP

#include "glm/glm.hpp"

enum class Shapes { S, Z, T, O, I, L, J };

enum class Rotation { A0, A90, A180, A270 };

class Piece
{
public:
  void DownIncrement(glm::vec2&& increment)
  {
    m_origin += increment;
    UpdateQuads();
  }

  virtual Shapes GetShape() const = 0;

  [[nodiscard]] const std::array<glm::vec2, 4>& GetQuads() const { return m_quads; }

  [[nodiscard]] const glm::vec4& GetColor() const { return m_color; }

protected:
  Piece(const glm::vec4& color, const glm::vec2& origin) :
      m_color(color),
      m_origin(origin),
      m_quads({}),
      m_rotation{ Rotation::A0 }
  {
  }

  virtual void UpdateQuads() = 0;

  glm::vec4 m_color;
  glm::vec2 m_origin;
  std::array<glm::vec2, 4> m_quads;
  Rotation m_rotation;
};

#endif // PIECE_HPP
