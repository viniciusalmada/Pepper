#pragma once

#include "Pepper/Events/KeyEvent.hpp"
#include "Pepper/Events/MouseEvent.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Layer.hpp"

namespace Pepper
{
  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;

    void OnDetach() override;

    void OnImGuiRender() override;

    void Begin() const;

    void End() const;

  private:
    float time = 0.0f;
  };
}