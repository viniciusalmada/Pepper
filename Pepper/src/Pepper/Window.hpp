#pragma once
#include "PepperPCH.hpp"
#include "Core.hpp"
#include "Events/Event.hpp"

namespace Pepper
{
  struct WindowProps
  {
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string& title = "Pepper engine",
                unsigned int w = 1280,
                unsigned int h = 720)
        : title(title), width(w), height(h)
    {
    }
  };

  class PEPPER_API Window
  {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVsync(bool enabled) = 0;
    virtual bool IsVsync() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
  };
}