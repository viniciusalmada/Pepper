// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Application.hpp"

#include "Pepper/Input/Input.hpp"
#include "Pepper/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

// #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Pepper
{

  struct Application::Impl
  {
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    Scope<Window> window;
    ImGuiLayer* imGuiLayer;
    bool running = true;
    bool minimized = false;
    LayerStack layer_stack;
    float last_frame_time;

    static Application* app_instance;
  };

  Application* Application::Impl::app_instance = nullptr;

  Application::Application() : impl(new Impl())
  {
    PP_CORE_ASSERT(!impl->app_instance, "Application already defined!");
    impl->app_instance = this;

    impl->window = std::unique_ptr<Window>(Window::Create());
    impl->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    Renderer::Init();

    impl->imGuiLayer = new ImGuiLayer();
    PushOverlay(impl->imGuiLayer);

    impl->last_frame_time = 0;
  }

  void Application::PushLayer(Layer* layer)
  {
    impl->layer_stack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* overlay)
  {
    impl->layer_stack.PushOverlay(overlay);
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher{ e };
    dispatcher.Dispatch<WindowCloseEvent>(
      std::bind(&Application::Impl::OnWindowClose, impl.get(), std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
      std::bind(&Application::Impl::OnWindowResize, impl.get(), std::placeholders::_1));

    // From top to bottom of stack check event handled
    for (auto iter = impl->layer_stack.end(); iter != impl->layer_stack.begin();)
    {
      (*--iter)->OnEvent(e);
      if (e.IsHandled())
        break;
    }
  }

  void Application::Run()
  {
    while (impl->running)
    {
      float time_sec = static_cast<float>(glfwGetTime()); // Platform::GetTime
      Timestep timestep{ time_sec - impl->last_frame_time };
      impl->last_frame_time = time_sec;

      if (!impl->minimized)
      {
        for (Layer* layer : impl->layer_stack)
          layer->OnUpdate(timestep);
      }
      impl->imGuiLayer->Begin();
      for (Layer* layer : impl->layer_stack)
        layer->OnImGuiRender();
      impl->imGuiLayer->End();

      impl->window->OnUpdate();
    }
  }

  bool Application::Impl::OnWindowClose(WindowCloseEvent&)
  {
    running = false;
    return true;
  }

  bool Application::Impl::OnWindowResize(WindowResizeEvent& e)
  {
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
      minimized = true;
      return false;
    }
    minimized = false;
    Renderer::OnViewportResize(e.GetWidth(), e.GetHeight());

    return false;
  }
}
