#pragma once

#include "Event.hpp"

namespace Butter
{
  class BUTTER_API KeyEvent : public Event
  {
  public:
    int GetKeyCode() const { return key_code; }

  protected:
    KeyEvent(int keyCode) : key_code(keyCode) {}

    int key_code;
  };

  class BUTTER_API KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(int keyCode, int repeatCount)
        : KeyEvent(key_code), repeat_count(repeatCount)
    {
    }

    int GetRepeatedCount() const { return repeat_count; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyPressedKey: " << key_code << " (" << repeat_count
         << " repeats)";
      return ss.str();
    }

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::INPUT | EvCategory::KEYBOARD;

      return EventInfo{ EvType::KEY_PRESSED, flags };
    }

  private:
    int repeat_count;
  };

  class BUTTER_API KeyReleaseEvent : public KeyEvent
  {
  public:
    KeyReleaseEvent(int keyCode) : KeyEvent(key_code) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyPressedKey: " << key_code;
      return ss.str();
    }

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::INPUT | EvCategory::KEYBOARD;

      return EventInfo{ EvType::KEY_PRESSED, flags };
    }
  };
}