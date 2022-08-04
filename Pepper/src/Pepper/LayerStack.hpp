#pragma once

#include "Core.hpp"
#include "Layer.hpp"

#include <PepperPCH.hpp>

namespace Pepper
{
  class PEPPER_API LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return layers.begin(); }
    std::vector<Layer*>::iterator end() { return layers.end(); }

  private:
    std::vector<Layer*> layers;
    unsigned int layer_index = 0;
  };
}