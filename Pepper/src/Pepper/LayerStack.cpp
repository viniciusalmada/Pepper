#include "LayerStack.hpp"

Pepper::LayerStack::LayerStack()
{
}

Pepper::LayerStack::~LayerStack()
{
  for (Layer* l : layers)
    delete l;
}

void Pepper::LayerStack::PushLayer(Layer* layer)
{
  layers.emplace(layers.begin() + layer_index, layer);
  layer_index++;
}

void Pepper::LayerStack::PushOverlay(Layer* overlay)
{
  layers.emplace_back(overlay);
}

void Pepper::LayerStack::PopLayer(Layer* layer)
{
  auto iter = FIND(layers, layer);
  if (iter != layers.end())
  {
    layers.erase(iter);
    layer_index--;
  }
}

void Pepper::LayerStack::PopOverlay(Layer* overlay)
{
  auto iter = FIND(layers, overlay);
  if (iter != layers.end())
  {
    layers.erase(iter);
  }
}