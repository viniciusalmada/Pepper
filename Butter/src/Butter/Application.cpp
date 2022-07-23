#include "Application.hpp"

#include "ButterPCH.hpp"

Butter::Application::Application()
{
  window = std::unique_ptr<Window>(Window::Create());
}

Butter::Application::~Application() {}

void Butter::Application::Run()
{
  while (running)
  {
    window->OnUpdate();
  }
}
