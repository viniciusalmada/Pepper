#pragma once

#include "Layer.hpp"
#include "Pepper/Core/Core.hpp"

namespace Pepper
{
  class LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin();
    std::vector<Layer*>::iterator end();

  private:
    DECLARE_PIMPL
  };
}