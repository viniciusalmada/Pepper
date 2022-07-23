#pragma once

#include "Core.hpp"
#include "Events/WindowEvent.hpp"
#include "Window.hpp"

namespace Pepper
{
  class PEPPER_API Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

  private:
    bool OnWindowClose(WindowCloseEvent& e);
    
    std::unique_ptr<Window> window;
    bool running = true;
  };

  // To be defined in client
  Application* CreateApplication();
}