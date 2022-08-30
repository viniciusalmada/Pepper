// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Layers/LayerStack.hpp"

namespace Pepper
{
  class LayerStack::Impl
  {
  public:
    std::vector<Layer*> layers;
    unsigned int layer_index = 0;
  };

  LayerStack::LayerStack() : pimp(new Impl{}) {}

  LayerStack::~LayerStack()
  {
    for (Layer* l : pimp->layers)
      delete l;
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    pimp->layers.emplace(pimp->layers.begin() + pimp->layer_index, layer);
    pimp->layer_index++;
    layer->OnAttach();
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    pimp->layers.emplace_back(overlay);
    overlay->OnAttach();
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto iter = FIND(pimp->layers, layer);
    if (iter != pimp->layers.end())
    {
      layer->OnDetach();
      pimp->layers.erase(iter);
      pimp->layer_index--;
    }
  }

  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto iter = FIND(pimp->layers, overlay);
    if (iter != pimp->layers.end())
    {
      overlay->OnDetach();
      pimp->layers.erase(iter);
    }
  }

  std::vector<Layer*>::iterator LayerStack::begin()
  {
    return pimp->layers.begin();
  }

  std::vector<Layer*>::iterator LayerStack::end()
  {
    return pimp->layers.end();
  }
}