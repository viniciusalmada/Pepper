#ifndef S_PIECE_HPP
#define S_PIECE_HPP

#include "Pieces/Piece.hpp"

class SPiece : public Piece
{
public:
  SPiece(glm::vec4 color, glm::vec2 origin);

  [[nodiscard]] Shapes GetShape() const override;

private:
  void UpdateQuads() override;
};

#endif // S_PIECE_HPP
