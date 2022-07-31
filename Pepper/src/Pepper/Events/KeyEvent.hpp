#pragma once

#include "Event.hpp"
#include "Pepper/KeyCodes.hpp"

namespace Pepper
{
  class PEPPER_API KeyEvent : public Event
  {
  public:
    PPKey GetKeyCode() const { return key_code; }

  protected:
    KeyEvent(PPKey keyCode) : key_code(keyCode) {}

    PPKey key_code;
  };

  class PEPPER_API KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(PPKey keyCode, int repeatCount)
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
    KeyReleaseEvent(PPKey keyCode) : KeyEvent(keyCode) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyReleaseEvent: " << key_code;
      return ss.str();
    }

    EVENT_INFO(EvCategory::INPUT | EvCategory::KEYBOARD)
    STATIC_TYPE(EvType::KEY_RELEASE)
  };

  class PEPPER_API KeyTypedEvent : public KeyEvent
  {
  public:
    KeyTypedEvent(PPKey keyCode) : KeyEvent(keyCode) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyTypedEvent: " << key_code;
      return ss.str();
    }

    EVENT_INFO(EvCategory::INPUT | EvCategory::KEYBOARD)
    STATIC_TYPE(EvType::KEY_TYPED)
  };
}