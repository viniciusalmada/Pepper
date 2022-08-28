#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Core/Timestep.hpp"
#include "Pepper/Events/Event.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Layers/ImGui/ImGuiLayer.hpp"
#include "Pepper/Layers/LayerStack.hpp"
#include "Window.hpp"

namespace Pepper
{
  class Application
  {
  public:
    Application();
    virtual ~Application() = default;

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow() { return *window; }

    static Application& Get() { return *app_instance; }

  private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    Scope<Window> window;
    ImGuiLayer* imGuiLayer;
    bool running = true;
    bool minimized = false;
    LayerStack layer_stack;
    float last_frame_time;

  private:
    static Application* app_instance;
  };

  // To be defined in client
  Application* CreateApplication();
}