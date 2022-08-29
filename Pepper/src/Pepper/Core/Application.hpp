#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Core/TimeStep.hpp"
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
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow();

    static Application& Get();

    PIMPL()
  };

  // To be defined in client
  Application* CreateApplication();
}