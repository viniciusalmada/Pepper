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
    
#pragma warning(push)
#pragma warning(disable : 4251) // needs to have dll-interface
    std::unique_ptr<Window> window;
#pragma warning(pop)
    bool running = true;
  };

  // To be defined in client
  Application* CreateApplication();
}