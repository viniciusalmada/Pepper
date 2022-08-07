#pragma once

#include "Event.hpp"
#include "Pepper/MouseButtonCodes.hpp"

namespace Pepper
{

  class MouseMovedEvent : public Event
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

  class MouseScrolledEvent : public Event
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

  class MouseButtonEvent : public Event
  {
  public:
    PPMouseBt GetMouseButton() const { return button; }

  protected:
    int GetFlags() const { return EvCategory::MOUSE | EvCategory::INPUT; }

    MouseButtonEvent(PPMouseBt bt) : button(bt) {}

    PPMouseBt button;
  };

  class MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonPressedEvent(PPMouseBt button) : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << button;
      return ss.str();
    }

    EVENT_INFO(GetFlags())
    STATIC_TYPE(EvType::MOUSE_BUTTON_PRESSED)
  };

  class MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonReleasedEvent(PPMouseBt button) : MouseButtonEvent(button) {}

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