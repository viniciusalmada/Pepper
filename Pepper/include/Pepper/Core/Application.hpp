#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Events/Event.hpp"
#include "Pepper/Layers/LayerStack.hpp"
#include "Window.hpp"

namespace Pepper
{
  class Application
  {
  public:
    explicit Application(std::string&& title, uint32_t width = 1280, uint32_t height = 720);
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);

    Window& GetWindow();

    static Application& Get();

  private:
    DECLARE_PIMPL
  };

  // To be defined in client
  Scope<Application> CreateApplication();
}