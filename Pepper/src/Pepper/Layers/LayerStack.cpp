// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "LayerStack.hpp"

namespace Pepper
{
  LayerStack::LayerStack() {}

  LayerStack::~LayerStack()
  {
    for (Layer* l : layers)
      delete l;
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    layers.emplace(layers.begin() + layer_index, layer);
    layer_index++;
    layer->OnAttach();
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    layers.emplace_back(overlay);
    overlay->OnAttach();
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto iter = FIND(layers, layer);
    if (iter != layers.end())
    {
      layer->OnDetach();
      layers.erase(iter);
      layer_index--;
    }
  }

  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto iter = FIND(layers, overlay);
    if (iter != layers.end())
    {
      overlay->OnDetach();
      layers.erase(iter);
    }
  }
}