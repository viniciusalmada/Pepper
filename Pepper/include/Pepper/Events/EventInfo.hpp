#pragma once

#include "EventCategory.hpp"
#include "EventType.hpp"

#include <string>
#include <unordered_map>

namespace Pepper
{
  class EventInfo
  {
  public:
    EventInfo(EvType t, int catFlags) : type(t), category_flags(catFlags) {}

    int GetFlags() const { return category_flags; }

    std::string GetName() const { return event_type_names.at(type); }

    EvType GetType() const { return type; }

  private:
    EvType type;
    int category_flags;

    std::unordered_map<EvType, std::string> event_type_names{
      { EvType::NONE, "NONE" },
      { EvType::WINDOW_CLOSE, "WINDOW_CLOSE" },
      { EvType::WINDOW_RESIZE, "WINDOW_RESIZE" },
      { EvType::WINDOW_FOCUS, "WINDOW_FOCUS" },
      { EvType::WINDOW_LOST_FOCUS, "WINDOW_LOST_FOCUS" },
      { EvType::WINDOW_MOVED, "WINDOW_MOVED" },
      { EvType::APP_TICK, "APP_TICK" },
      { EvType::APP_UPDATE, "APP_UPDATE" },
      { EvType::APP_RENDER, "APP_RENDER" },
      { EvType::KEY_PRESS, "KEY_PRESS" },
      { EvType::KEY_RELEASE, "KEY_RELEASE" },
      { EvType::KEY_TYPED, "KEY_TYPED" },
      { EvType::MOUSE_BUTTON_PRESSED, "MOUSE_BUTTON_PRESSED" },
      { EvType::MOUSE_BUTTON_RELEASE, "MOUSE_BUTTON_RELEASE" },
      { EvType::MOUSE_MOVE, "MOUSE_MOVE" },
      { EvType::MOUSE_SCROLL, "MOUSE_SCROLL" },
    };
  };

#define EVENT_INFO(x)                                                                                                  \
  EventInfo GetEventInfo() const override                                                                              \
  {                                                                                                                    \
    return EventInfo{ GetStaticType(), (x) };                                                                          \
  }

#define STATIC_TYPE(x)                                                                                                 \
  static EvType GetStaticType()                                                                                        \
  {                                                                                                                    \
    return (x);                                                                                                        \
  }
}