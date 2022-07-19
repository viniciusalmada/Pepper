#pragma once

#include "Event.hpp"

#include <sstream>

namespace Butter
{

  class BUTTER_API MouseMovedEvent : public Event
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

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::MOUSE | EvCategory::INPUT;
      return EventInfo{ EvType::MOUSE_MOVED, flags };
    }

  private:
    float x_pos, y_pos;
  };

  class BUTTER_API MouseScrolledEvent : public Event
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

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::MOUSE | EvCategory::INPUT;
      return EventInfo{ EvType::MOUSE_SCROLLED, flags };
    }

  private:
    float x_offset, y_offset;
  };

  class BUTTER_API MouseButtonEvent : public Event
  {
  public:
    int GetMouseButton() const { return button; }

  protected:
    int GetFlags() const { return EvCategory::MOUSE | EvCategory::INPUT; }

    MouseButtonEvent(int bt) : button(bt) {}

    int button;
  };

  class BUTTER_API MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << button;
      return ss.str();
    }

    EventInfo GetEventInfo() const override
    {
      int flags = GetFlags();
      return EventInfo{ EvType::MOUSE_SCROLLED, flags };
    }
  };

  class BUTTER_API MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonReleasedEvent: " << button;
      return ss.str();
    }

    EventInfo GetEventInfo() const override
    {
      int flags = GetFlags();
      return EventInfo{ EvType::MOUSE_BUTTON_RELEASED, flags };
    }
  };

}