#ifndef GAME_LAYER_HPP
#define GAME_LAYER_HPP

#include <Pepper.hpp>

class GameLayer : public Pepper::Layer
{
public:
  GameLayer();
  ~GameLayer() override;

  void OnAttach() override;
  void OnUpdate(Pepper::TimeStep) override;
  void OnImGuiRender() override;
  void OnEvent(Pepper::Event&) override;

private:
  static void DrawGrid();

  struct Implementation;
  Pepper::Scope<Implementation> impl;
};

#endif // GAME_LAYER_HPP
