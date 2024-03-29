// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Core/Application.hpp"

#include "Pepper/Core/Window.hpp"
#include "Pepper/Events/EventDispatcher.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Input/Input.hpp"
#include "Pepper/Layers/ImGui/ImGuiLayer.hpp"
#include "Pepper/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Pepper
{
  class Application::Impl
  {
  public:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    Scope<Window> window;
    LayerStack layer_stack;
    Ref<ImGuiLayer> imGuiLayer;

    bool running = true;
    bool minimized = false;

    float last_frame_time;

    static Application* app_instance;
  };

  Application* Application::Impl::app_instance = nullptr;

  Application::Application(std::string&& title, uint32_t width, uint32_t height) :
      pimp(CreateScope<Impl>())
  {
    PP_PROFILE_FUNCTION();
    PP_CORE_ASSERT(!pimp->app_instance, "Application already defined!");
    pimp->app_instance = this;

    pimp->window = std::unique_ptr<Window>(Window::Create({ title, width, height }));
    pimp->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    Renderer::Init();

    pimp->imGuiLayer = CreateRef<ImGuiLayer>();
    PushOverlay(pimp->imGuiLayer);

    pimp->last_frame_time = 0;
  }

  Application::~Application()
  {
    PP_PROFILE_FUNCTION();
    Renderer::Shutdown();
  }

  void Application::PushLayer(Ref<Layer> layer)
  {
    PP_PROFILE_FUNCTION();
    pimp->layer_stack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Ref<Layer> overlay)
  {
    PP_PROFILE_FUNCTION();
    pimp->layer_stack.PushOverlay(overlay);
    overlay->OnAttach();
  }

  Window& Application::GetWindow()
  {
    return *pimp->window;
  }

  Application& Application::Get()
  {
    return *Impl::app_instance;
  }

  void Application::OnEvent(Event& e)
  {
    PP_PROFILE_FUNCTION();
    EventDispatcher dispatcher{ e };
    dispatcher.Dispatch<WindowCloseEvent>(
      std::bind(&Application::Impl::OnWindowClose, pimp.get(), std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
      std::bind(&Application::Impl::OnWindowResize, pimp.get(), std::placeholders::_1));

    // From top to bottom of stack check event handled
    for (auto iter = pimp->layer_stack.end(); iter != pimp->layer_stack.begin();)
    {
      (*--iter)->OnEvent(e);
      if (e.IsHandled())
        break;
    }
  }

  void Application::Run()
  {
    PP_PROFILE_FUNCTION();
    while (pimp->running)
    {
      PP_PROFILE_SCOPE("RunLoopStep");
      float time_sec = static_cast<float>(glfwGetTime()); // Platform::GetTime
      TimeStep timeStep{ time_sec - pimp->last_frame_time };
      pimp->last_frame_time = time_sec;

      if (!pimp->minimized)
      {
        {
          PP_PROFILE_SCOPE("LayerStack Update");
          for (Ref<Layer> layer : pimp->layer_stack)
            layer->OnUpdate(timeStep);
        }
        pimp->imGuiLayer->Begin();
        {
          PP_PROFILE_SCOPE("LayerStack ImGuiRender");
          for (Ref<Layer> layer : pimp->layer_stack)
            layer->OnImGuiRender();
        }
        pimp->imGuiLayer->End();
      }

      pimp->window->OnUpdate();
    }
  }

  bool Application::Impl::OnWindowClose(WindowCloseEvent&)
  {
    running = false;
    return true;
  }

  bool Application::Impl::OnWindowResize(WindowResizeEvent& e)
  {
    PP_PROFILE_FUNCTION();
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
