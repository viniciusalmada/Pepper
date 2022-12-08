#ifndef SESSION_HPP
#define SESSION_HPP

#include "Pieces/Piece.hpp"

#include <Pepper.hpp>

class Session
{
public:
  static void Start();

  static void OnUpdate(Pepper::TimeStep ts);

  static void OnEachPiece(std::function<void(Pepper::Ref<Piece>)> fun);

private:
  static void AddPiece(Pepper::Ref<Piece> piece);

  static std::vector<Pepper::Ref<Piece>> m_pieces;
};

#endif // SESSION_HPP
