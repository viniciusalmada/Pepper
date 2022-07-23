#pragma once

#include "Core.hpp"

namespace Butter
{

  // Events in Butter are currently blocking, meaning when an event occurs it
  // immediately gets dispatched and must be dealt with right then an there.

  enum class EvType {
    NONE,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_FOCUS,
    WINDOW_LOST_FOCUS,
    WINDOW_MOVED,
    APP_TICK,
    APP_UPDATE,
    APP_RENDER,
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASE,
    MOUSE_MOVE,
    MOUSE_SCROLL
  };

  // clang-format off
  enum EvCategory : int {
    NONE         = 0,
    APPLICATION  = 0b00001,
    INPUT        = 0b00010,
    KEYBOARD     = 0b00100,
    MOUSE        = 0b01000,
    MOUSE_BUTTON = 0b10000,
  };
  // clang-format on

  class EventInfo
  {
  public:
    EventInfo(EvType t, int catFlags) : type(t), category_flags(catFlags) {}

    int GetFlags() const { return category_flags; }

    std::string GetName() const { return event_type_names.at(type); }

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
      { EvType::MOUSE_BUTTON_PRESSED, "MOUSE_BUTTON_PRESSED" },
      { EvType::MOUSE_BUTTON_RELEASE, "MOUSE_BUTTON_RELEASE" },
      { EvType::MOUSE_MOVE, "MOUSE_MOVE" },
      { EvType::MOUSE_SCROLL, "MOUSE_SCROLL" },
    };
  };

  class BUTTER_API Event
  {
    friend class EventDispatcher;

  public:
    virtual EventInfo GetEventInfo() const = 0;

    virtual std::string ToString() const { return GetName(); };

    std::string GetName() const { return GetEventInfo().GetName(); };

    int GetCategoryFlags() const { return GetEventInfo().GetFlags(); };

    bool IsInCategory(EvCategory category)
    {
      return GetCategoryFlags() & (int)category;
    }

  protected:
    bool handled = false;
  };

  class EventDispatcher
  {
    template <typename Type>
    using EventFn = std::function<bool(Type&)>;

  public:
    EventDispatcher(Event& e) : event(e) {}

    template <typename Type>
    bool Dispatch(EventFn<Type> fun)
    {
      if (event.GetEventInfo() == Type::GetStaticType())
      {
        // Transform reference into pointer
        Type* event_ptr = (Type*)&event;

        event.handled = fun(*event_ptr);

        return true;
      }

      return false;
    }

  private:
    Event& event;
  };

  std::ostream& operator<<(std::ostream& os, const Event& e);
}