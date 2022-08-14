#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
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

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> blue_shader;

    std::shared_ptr<VertexArray> triangle_VAO;
    std::shared_ptr<VertexArray> square_VAO;

  private:
    static Application* app_instance;
  };

  // To be defined in client
  Application* CreateApplication();
}