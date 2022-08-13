#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Renderer/Shader.hpp"
#include "Window.hpp"

namespace Pepper
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

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

    unsigned int vertex_array, vertex_buffer, index_buffer;
    std::unique_ptr<Shader> shader;

  private:
    static Application* app_instance;
  };

  // To be defined in client
  Application* CreateApplication();
}