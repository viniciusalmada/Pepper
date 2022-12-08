#include "Session.hpp"

#include "Constants.hpp"

#include <Pieces/SPiece.hpp>
#include <glm/glm.hpp>
#include <utility>

std::vector<Pepper::Ref<Piece>> Session::m_pieces{};

void Session::Start()
{
  m_pieces.clear();

  Pepper::Ref<Piece> p = Pepper::CreateRef<SPiece>(RED, glm::vec2{ 140, 572 });
  AddPiece(p);
}

void Session::AddPiece(Pepper::Ref<Piece> piece)
{
  m_pieces.emplace_back(std::move(piece));
}

void Session::OnUpdate(Pepper::TimeStep ts)
{
  float increment = 20.0f * ts;
  m_pieces.front()->DownIncrement({ 0.0f, -increment });
}

void Session::OnEachPiece(std::function<void(Pepper::Ref<Piece>)> fun)
{
  std::ranges::for_each(m_pieces, std::move(fun));
}
