#pragma once

#include "Butter/Window.hpp"

#include <GLFW/glfw3.h>

namespace Butter
{
  class WinWindow : public Window
  {
  public:
    WinWindow(const WindowProps& props);
    virtual ~WinWindow();

    void OnUpdate() override;

    unsigned int GetWidth() const override { return data.width; }
    unsigned int GetHeight() const override { return data.height; }

    void SetEventCallback(const EventCallbackFn& callback) override
    {
      data.eventCallback = callback;
    }

    void SetVsync(bool enabled) override;
    bool IsVsync() const override;

  private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

  private:
    GLFWwindow* window;

    struct WindowData : WindowProps
    {
      bool vsync;
      EventCallbackFn eventCallback;
    };
    WindowData data;
  };
}