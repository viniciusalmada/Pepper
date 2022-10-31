#pragma once

#include "Core.hpp"
#include "Pepper/Events/Event.hpp"

namespace Pepper
{
  struct WindowProps
  {
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string& title = "Pepper engine",
                unsigned int w = 1280,
                unsigned int h = 720) :
        title(title),
        width(w),
        height(h)
    {
#if defined(PROFILING)
      this->title += " - Profiling running";
#endif
    }
  };

  class Window
  {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVsync(bool enabled) = 0;
    virtual bool IsVsync() const = 0;

    virtual std::any GetNativeWindow() const = 0;

    static Scope<Window> Create(const WindowProps& props = WindowProps());
  };
}