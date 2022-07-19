#include "ButterPCH.hpp"
#include "Application.hpp"

#include "Events/WindowEvent.hpp"
#include "Log.hpp"

Butter::Application::Application() {}

Butter::Application::~Application() {}

void Butter::Application::Run()
{
  WindowResizeEvent e(1280, 720);
  if (e.IsInCategory(EvCategory::APPLICATION))
  {
    BT_TRACE(e);
  }
  if (!e.IsInCategory(EvCategory::INPUT))
  {
    BT_TRACE(e);
  }
  while (true)
  {
  }
}
