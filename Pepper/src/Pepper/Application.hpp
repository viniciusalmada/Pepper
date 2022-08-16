#pragma once

#include "Core.hpp"
#include "Core/Timestep.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "Layers/ImGui/ImGuiLayer.hpp"
#include "Layers/LayerStack.hpp"
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

    std::unique_ptr<Window> window;
    ImGuiLayer* imGuiLayer;
    bool running = true;
    LayerStack layer_stack;
    float last_frame_time;

  private:
    static Application* app_instance;
  };

  // To be defined in client
  Application* CreateApplication();
}