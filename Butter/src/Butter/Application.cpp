#include "Application.hpp"

#include "ButterPCH.hpp"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Butter::Application::Application()
{
  window = std::unique_ptr<Window>(Window::Create());
  window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Butter::Application::~Application() {}

void Butter::Application::OnEvent(Event& e)
{
  EventDispatcher dispatcher{ e };
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
  
  BT_CORE_TRACE("{0}", e);
}

void Butter::Application::Run()
{
  while (running)
  {
    window->OnUpdate();
  }
}

bool Butter::Application::OnWindowClose(WindowCloseEvent& e)
{
  running = false;
  return true;
}
