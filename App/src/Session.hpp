#ifndef SESSION_HPP
#define SESSION_HPP

#include "GridSquare.hpp"
#include "Pieces/Piece.hpp"

#include <Pepper.hpp>

class Session
{
public:
  static void Start();

  static void OnUpdate(Pepper::TimeStep ts);

  static void OnEachPiece(std::function<void(Pepper::Ref<Piece>)> fun);

  static glm::vec2 ConvertSquare(GridSquare gs);

private:
  static void AddPiece(Pepper::Ref<Piece> piece);

  static std::vector<Pepper::Ref<Piece>> m_pieces;
};

#endif // SESSION_HPP
