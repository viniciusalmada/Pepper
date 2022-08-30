#pragma once

#include "Pepper/Core/Window.hpp"
#include "Pepper/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Pepper
{
  class WinWindow : public Window
  {
  public:
    WinWindow(const WindowProps& props);
    ~WinWindow() override;

    void OnUpdate() override;

    unsigned int GetWidth() const override;
    unsigned int GetHeight() const override;

    void SetEventCallback(const EventCallbackFn& callback) override;

    void SetVsync(bool enabled) override;
    bool IsVsync() const override;

    std::any GetNativeWindow() const override;

  private:
    DECLARE_PIMPL
  };
}