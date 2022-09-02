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

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);
    void PopLayer(Ref<Layer> layer);
    void PopOverlay(Ref<Layer> overlay);

    std::vector<Ref<Layer>>::iterator begin();
    std::vector<Ref<Layer>>::iterator end();

  private:
    DECLARE_PIMPL
  };
}