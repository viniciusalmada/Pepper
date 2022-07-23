#include "Application.hpp"

#include "PepperPCH.hpp"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Pepper::Application::Application()
{
  window = std::unique_ptr<Window>(Window::Create());
  window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Pepper::Application::~Application() {}

void Pepper::Application::OnEvent(Event& e)
{
  EventDispatcher dispatcher{ e };
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
  
  BT_CORE_TRACE("{0}", e);
}

void Pepper::Application::Run()
{
  while (running)
  {
    window->OnUpdate();
  }
}

bool Pepper::Application::OnWindowClose(WindowCloseEvent& e)
{
  running = false;
  return true;
}
