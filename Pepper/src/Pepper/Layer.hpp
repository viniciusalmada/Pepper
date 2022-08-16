#pragma once

#include "Core.hpp"
#include "Core/Timestep.hpp"
#include "Events/Event.hpp"

namespace Pepper
{
  class Layer
  {
  public:
    Layer(const std::string& name = "Layer");

    virtual ~Layer() = default;

    virtual void OnAttach() {}

    virtual void OnDetach() {}

    virtual void OnUpdate(Timestep) {}

    virtual void OnImGuiRender() {}

    virtual void OnEvent(Event&) {}

    const std::string& GetName() const { return debugName; }

  protected:
    std::string debugName;
  };
}