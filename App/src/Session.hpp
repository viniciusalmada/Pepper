#ifndef SESSION_HPP
#define SESSION_HPP

#include "GridSquare.hpp"
#include "Pieces/Piece.hpp"

#include <Pepper.hpp>

class Session
{
public:
  void Start();

  void OnUpdate(Pepper::TimeStep standingGS);

  void OnCurrentPiece(const std::function<void(Pepper::Ref<Piece>)>& fun);
  void OnEachSquare(std::function<void(std::pair<GridSquare, glm::vec4>)> fun);

  static glm::vec2 ConvertSquare(GridSquare gs);

private:
  void AddPiece();

  Pepper::Ref<Piece> m_current_piece{};
  std::vector<Pepper::Ref<Piece>> m_pieces{};
  std::vector<std::pair<GridSquare, glm::vec4>> m_squares{};
};

#endif // SESSION_HPP