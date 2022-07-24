#pragma once

#include "Core.hpp"
#include "Events/WindowEvent.hpp"
#include "LayerStack.hpp"
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

    void PushLayer(Layer* layer);
    
    void PushOverlay(Layer* overlay);

  private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> window;
    bool running = true;
    LayerStack layer_stack;
  };

  // To be defined in client
  Application* CreateApplication();
}