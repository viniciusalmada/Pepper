#pragma once
#include "Core.hpp"
#include "Events/Event.hpp"
#include "PepperPCH.hpp"

namespace Pepper
{
  class PEPPER_API Layer
  {
  public:
    Layer(const std::string& name = "Layer");

    virtual ~Layer();

    virtual void OnAttach() {}

    virtual void OnDetach() {}

    virtual void OnUpdate() {}

    virtual void OnEvent(Event&) {}

    const std::string& GetName() const { return debugName; }

  protected:
    std::string debugName;
  };
}