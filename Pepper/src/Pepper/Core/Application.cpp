// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Application.hpp"

#include "Pepper/Input/Input.hpp"
#include "Pepper/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Pepper
{

  Application* Application::app_instance = nullptr;

  Application::Application()
  {
    PP_CORE_ASSERT(!app_instance, "Application already defined!");
    app_instance = this;

    window = std::unique_ptr<Window>(Window::Create());
    window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();

    imGuiLayer = new ImGuiLayer();
    PushOverlay(imGuiLayer);

    last_frame_time = 0;
  }

  void Application::PushLayer(Layer* layer)
  {
    layer_stack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* overlay)
  {
    layer_stack.PushOverlay(overlay);
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher{ e };
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

    // From top to bottom of stack check event handled
    for (auto iter = layer_stack.end(); iter != layer_stack.begin();)
    {
      (*--iter)->OnEvent(e);
      if (e.IsHandled())
        break;
    }
  }

  void Application::Run()
  {
    while (running)
    {
      float time_sec = static_cast<float>(glfwGetTime()); // Platform::GetTime
      Timestep timestep{ time_sec - last_frame_time };
      last_frame_time = time_sec;

      if (!minimized)
      {
        for (Layer* layer : layer_stack)
          layer->OnUpdate(timestep);

      }
      imGuiLayer->Begin();
      for (Layer* layer : layer_stack)
        layer->OnImGuiRender();
      imGuiLayer->End();
      
      window->OnUpdate();
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent&)
  {
    running = false;
    return true;
  }

  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
      minimized = true;
      return false;
    }
    minimized = false;
    return false;
  }
}
