#include "Application.hpp"

#include <PepperPCH.hpp>
#include <glad/glad.h>

#include "Pepper/Input.hpp"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Pepper::Application* Pepper::Application::app_instance = nullptr;

Pepper::Application::Application()
{
  PP_CORE_ASSERT(!app_instance, "Application already defined!");
  app_instance = this;

  window = std::unique_ptr<Window>(Window::Create());
  window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Pepper::Application::~Application() {}

void Pepper::Application::PushLayer(Layer* layer)
{
  layer_stack.PushLayer(layer);
  layer->OnAttach();
}

void Pepper::Application::PushOverlay(Layer* overlay)
{
  layer_stack.PushOverlay(overlay);
  overlay->OnAttach();
}

void Pepper::Application::OnEvent(Event& e)
{
  EventDispatcher dispatcher{ e };
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

  // From top to bottom of stack check event handled
  for (auto iter = layer_stack.end(); iter != layer_stack.begin();)
  {
    (*--iter)->OnEvent(e);
    if (e.IsHandled())
      break;
  }
}

void Pepper::Application::Run()
{
  while (running)
  {
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Layer* layer : layer_stack)
      layer->OnUpdate();

    window->OnUpdate();
  }
}

bool Pepper::Application::OnWindowClose(WindowCloseEvent&)
{
  running = false;
  return true;
}
