#include "LayerStack.hpp"

Pepper::LayerStack::LayerStack() { layer_insert = layers.begin(); }

Pepper::LayerStack::~LayerStack()
{
  for (Layer* l : layers)
    delete l;
}

void Pepper::LayerStack::PushLayer(Layer* layer)
{
  layer_insert = layers.emplace(layer_insert, layer);
}

void Pepper::LayerStack::PopLayer(Layer* layer)
{
  auto iter = FIND(layers, layer);
  if (iter != layers.end())
  {
    layers.erase(iter);
    layer_insert--;
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