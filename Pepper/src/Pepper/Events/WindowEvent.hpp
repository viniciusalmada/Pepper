#pragma once

#include "Event.hpp"

namespace Pepper
{

  class PEPPER_API WindowResizeEvent : public Event
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

    EVENT_INFO(EvCategory::APPLICATION)
    STATIC_TYPE(EvType::WINDOW_RESIZE)

  private:
    unsigned int width, height;
  };

  class PEPPER_API WindowCloseEvent : public Event
  {
  public:
    WindowCloseEvent() {}

    EVENT_INFO(EvCategory::APPLICATION)
    STATIC_TYPE(EvType::WINDOW_CLOSE)
  };

  class PEPPER_API AppTickEvent : public Event
  {
  public:
    AppTickEvent() {}

    EVENT_INFO(EvCategory::APPLICATION)
    STATIC_TYPE(EvType::APP_TICK)
  };

  class PEPPER_API AppUpdateEvent : public Event
  {
  public:
    AppUpdateEvent() {}

    EVENT_INFO(EvCategory::APPLICATION)
    STATIC_TYPE(EvType::APP_UPDATE)
  };

  class PEPPER_API AppRenderEvent : public Event
  {
  public:
    AppRenderEvent() {}

    EVENT_INFO(EvCategory::APPLICATION)
    STATIC_TYPE(EvType::APP_RENDER)
  };
}