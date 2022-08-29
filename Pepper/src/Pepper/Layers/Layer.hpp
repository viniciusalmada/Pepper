#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Core/TimeStep.hpp"
#include "Pepper/Events/Event.hpp"

namespace Pepper
{
  class Layer
  {
  public:
    Layer(const std::string& name = "Layer");

    virtual ~Layer() = default;

    virtual void OnAttach() {}

    virtual void OnDetach() {}

    virtual void OnUpdate(TimeStep) {}

    virtual void OnImGuiRender() {}

    virtual void OnEvent(Event&) {}

    const std::string& GetName() const { return debugName; }

  protected:
    std::string debugName;
  };
}