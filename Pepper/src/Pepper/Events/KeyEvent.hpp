#pragma once

#include "Event.hpp"

namespace Pepper
{
  class PEPPER_API KeyEvent : public Event
  {
  public:
    int GetKeyCode() const { return key_code; }

  protected:
    KeyEvent(int keyCode) : key_code(keyCode) {}

    int key_code;
  };

  class PEPPER_API KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(int keyCode, int repeatCount)
        : KeyEvent(keyCode), repeat_count(repeatCount)
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

    EVENT_INFO(EvCategory::INPUT | EvCategory::KEYBOARD)
    STATIC_TYPE(EvType::KEY_PRESS)

  private:
    int repeat_count;
  };

  class PEPPER_API KeyReleaseEvent : public KeyEvent
  {
  public:
    KeyReleaseEvent(int keyCode) : KeyEvent(keyCode) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyPressedKey: " << key_code;
      return ss.str();
    }

    EVENT_INFO(EvCategory::INPUT | EvCategory::KEYBOARD)
    STATIC_TYPE(EvType::KEY_RELEASE)
  };
}