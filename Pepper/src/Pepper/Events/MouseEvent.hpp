#pragma once

#include "Event.hpp"

namespace Pepper
{

  class PEPPER_API MouseMovedEvent : public Event
  {
  public:
    MouseMovedEvent(float x, float y) : x_pos(x), y_pos(y) {}

    float GetX() const { return x_pos; }

    float GetY() const { return y_pos; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseMovedEvent: " << x_pos << ", " << y_pos;
      return ss.str();
    }

    EVENT_INFO(EvCategory::INPUT | EvCategory::MOUSE)
    STATIC_TYPE(EvType::MOUSE_MOVE)

  private:
    float x_pos, y_pos;
  };

  class PEPPER_API MouseScrolledEvent : public Event
  {
  public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : x_offset(xOffset), y_offset(yOffset)
    {
    }

    float GetXOffset() const { return x_offset; }
    float GetYOffset() const { return y_offset; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
      return ss.str();
    }

    EVENT_INFO(EvCategory::INPUT | EvCategory::MOUSE)
    STATIC_TYPE(EvType::MOUSE_SCROLL)

  private:
    float x_offset, y_offset;
  };

  class PEPPER_API MouseButtonEvent : public Event
  {
  public:
    int GetMouseButton() const { return button; }

  protected:
    int GetFlags() const { return EvCategory::MOUSE | EvCategory::INPUT; }

    MouseButtonEvent(int bt) : button(bt) {}

    int button;
  };

  class PEPPER_API MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << button;
      return ss.str();
    }

    EVENT_INFO(GetFlags())
    STATIC_TYPE(EvType::MOUSE_BUTTON_PRESSED)
  };

  class PEPPER_API MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonReleasedEvent: " << button;
      return ss.str();
    }

    EVENT_INFO(GetFlags())
    STATIC_TYPE(EvType::MOUSE_BUTTON_RELEASE)
  };

}