#pragma once

#include "Level.hpp"

#include <Pepper.hpp>
#include <list>

class GameLayer : public Pepper::Layer
{
public:
  GameLayer();

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(Pepper::TimeStep) override;
  void OnImGuiRender() override;
  void OnEvent(Pepper::Event&) override;

private:
  void CreateCamera(uint32_t width, uint32_t height);

  bool OnWindowResize(Pepper::WindowResizeEvent& e);

  Level m_level;
  Pepper::Scope<Pepper::OrthoCamera> m_camera;
};