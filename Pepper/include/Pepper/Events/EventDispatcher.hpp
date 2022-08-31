#pragma once

#include "Event.hpp"

#include <functional>

namespace Pepper
{
  class EventDispatcher
  {
    template <typename Type>
    using EventFn = std::function<bool(Type&)>;

  public:
    EventDispatcher(Event& e) : event(e) {}

    template <typename TypeParam>
    bool Dispatch(EventFn<TypeParam> fun)
    {
      EvType type = event.GetEventInfo().GetType();
      if (type == TypeParam::GetStaticType())
      {
        // Transform reference into pointer
        TypeParam* event_ptr = (TypeParam*)&event;

        event.handled = fun(*event_ptr);

        return true;
      }

      return false;
    }

  private:
    Event& event;
  };
}