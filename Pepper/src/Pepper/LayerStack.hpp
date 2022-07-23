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

    std::vector<Layer*>::iterator Begin() { return layers.begin(); }
    std::vector<Layer*>::iterator End() { return layers.end(); }

  private:
    std::vector<Layer*> layers;
    std::vector<Layer*>::iterator layer_insert;
  };
}