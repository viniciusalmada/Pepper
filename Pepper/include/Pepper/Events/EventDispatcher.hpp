#pragma once

#include "Event.hpp"

#include <functional>

namespace Pepper
{
  class EventDispatcher
  {
  public:
    EventDispatcher(Event& e) : event(e) {}

    template <typename T, typename Fun>
    bool Dispatch(const Fun& fun)
    {
      EvType type = event.GetEventInfo().GetType();
      if (type == T::GetStaticType())
      {
        // Transform reference into pointer
        T& event_ptr = static_cast<T&>(event);

        event.handled = fun(event_ptr);

        return true;
      }

      return false;
    }

  private:
    Event& event;
  };
}