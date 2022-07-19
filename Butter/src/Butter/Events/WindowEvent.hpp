#pragma once

#include "Event.hpp"

#include <sstream>

namespace Butter
{

  class BUTTER_API WindowResizeEvent : public Event
  {
  public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : width(width), height(height)
    {
    }

    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "WindowResizeEvent: " << width << ", " << height;
      return ss.str();
    }

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::APPLICATION;
      return EventInfo{ EvType::WINDOW_RESIZE, flags };
    }

  private:
    unsigned int width, height;
  };

  class BUTTER_API WindowCloseEvent : public Event
  {
  public:
    WindowCloseEvent() {}

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::APPLICATION;
      return EventInfo{ EvType::WINDOW_CLOSE, flags };
    }
  };

  class BUTTER_API AppTickEvent : public Event
  {
  public:
    AppTickEvent() {}

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::APPLICATION;
      return EventInfo{ EvType::APP_TICK, flags };
    }
  };

  class BUTTER_API AppUpdateEvent : public Event
  {
  public:
    AppUpdateEvent() {}

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::APPLICATION;
      return EventInfo{ EvType::APP_UPDATE, flags };
    }
  };

  class BUTTER_API AppRenderEvent : public Event
  {
  public:
    AppRenderEvent() {}

    EventInfo GetEventInfo() const override
    {
      int flags = EvCategory::APPLICATION;
      return EventInfo{ EvType::APP_RENDER, flags };
    }
  };
}