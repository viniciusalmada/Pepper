#pragma once

#include "Core.hpp"
#include "Window.hpp"

namespace Butter
{
  class BUTTER_API Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();

  private:
#pragma warning(push)
#pragma warning(disable : 4251) // needs to have dll-interface
    std::unique_ptr<Window> window;
#pragma warning(pop)
    bool running = true;
  };

  // To be defined in client
  Application* CreateApplication();
}