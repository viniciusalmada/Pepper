#pragma once

#include "EventInfo.hpp"
#include "Pepper/Core/Core.hpp"

namespace Pepper
{

  // Events in Pepper are currently blocking, meaning when an event occurs it
  // immediately gets dispatched and must be dealt with right then an there.

  class Event
  {
    friend class EventDispatcher;

  public:
    virtual EventInfo GetEventInfo() const = 0;

    virtual std::string ToString() const { return GetName(); };

    std::string GetName() const { return GetEventInfo().GetName(); };

    int GetCategoryFlags() const { return GetEventInfo().GetFlags(); };

    bool IsInCategory(EvCategory category) { return GetCategoryFlags() & (int)category; }

    bool IsHandled() const { return handled; }

  protected:
    bool handled = false;
  };

  std::ostream& operator<<(std::ostream& os, const Event& e);
}