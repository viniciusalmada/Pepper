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
  void OnEachSquare(const std::function<void(std::pair<GridSquare, glm::vec4>)>& fun);

  void MoveLeft(Pepper::TimeStep ts);
  void MoveRight(Pepper::TimeStep ts);
  void DownPiece(Pepper::TimeStep ts);
  void RotatePiece();

  static glm::vec2 ConvertSquare(GridSquare gs);

private:
  void AddNewPiece();

  Pepper::Ref<Piece> m_current_piece{};
  std::vector<Pepper::Ref<Piece>> m_pieces{};
  std::vector<std::pair<GridSquare, glm::vec4>> m_locked_squares{};
  void LockPiece();
};

#endif // SESSION_HPP
