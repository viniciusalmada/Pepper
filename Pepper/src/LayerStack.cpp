// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Layers/LayerStack.hpp"

namespace Pepper
{
  class LayerStack::Impl
  {
  public:
    std::vector<Ref<Layer>> layers;
    unsigned int layer_index = 0;
  };

  LayerStack::LayerStack() : pimp(CreateScope<Impl>()) {}

  LayerStack::~LayerStack()
  {
    for (Ref<Layer> l : pimp->layers)
    {
      l->OnDetach();
    }
  }

  void LayerStack::PushLayer(Ref<Layer> layer)
  {
    pimp->layers.emplace(pimp->layers.begin() + pimp->layer_index, layer);
    pimp->layer_index++;
  }

  void LayerStack::PushOverlay(Ref<Layer> overlay)
  {
    pimp->layers.emplace_back(overlay);
  }

  void LayerStack::PopLayer(Ref<Layer> layer)
  {
    auto iter = std::find(pimp->layers.begin(), pimp->layers.begin() + pimp->layer_index, layer);
    if (iter != pimp->layers.begin() + pimp->layer_index)
    {
      layer->OnDetach();
      pimp->layers.erase(iter);
      pimp->layer_index--;
    }
  }

  void LayerStack::PopOverlay(Ref<Layer> overlay)
  {
    auto iter = std::find(pimp->layers.begin() + pimp->layer_index, pimp->layers.end(), overlay);
    if (iter != pimp->layers.end())
    {
      overlay->OnDetach();
      pimp->layers.erase(iter);
    }
  }

  std::vector<Ref<Layer>>::iterator LayerStack::begin()
  {
    return pimp->layers.begin();
  }

  std::vector<Ref<Layer>>::iterator LayerStack::end()
  {
    return pimp->layers.end();
  }
}