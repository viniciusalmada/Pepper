#pragma once

#include "Pepper/Window.hpp"
#include "Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Pepper
{
  class WinWindow : public Window
  {
  public:
    WinWindow(const WindowProps& props);
    ~WinWindow() override;

    void OnUpdate() override;

    unsigned int GetWidth() const override { return data.width; }
    unsigned int GetHeight() const override { return data.height; }

    void SetEventCallback(const EventCallbackFn& callback) override
    {
      data.eventCallback = callback;
    }

    void SetVsync(bool enabled) override;
    bool IsVsync() const override;

    std::any GetNativeWindow() const override
    {
      return std::make_any<GLFWwindow*>(window);
    };

  private:
    void Init(const WindowProps& props);
    void Shutdown();

    void ConfigResizeCB() const;
    void ConfigCloseCB() const;
    void ConfigKeyCB() const;
    void ConfigMouseButtonCB() const;
    void ConfigMouseScrollCB() const;
    void ConfigMouseMoveCB() const;

  private:
    GLFWwindow* window;
    GraphicsContext* graphics_context;

    struct WindowData : WindowProps
    {
      bool vsync;
      EventCallbackFn eventCallback;
    };
    WindowData data;
  };
}