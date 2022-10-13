// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "VoronoiApp.hpp"

#include "DiagramLayer.hpp"

#include <Pepper/Core/EntryPoint.hpp>

VoronoiApp::VoronoiApp()
{
  PushLayer(Pepper::CreateRef<DiagramLayer>());
}

VoronoiApp::~VoronoiApp() noexcept {}

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  return Pepper::CreateScope<VoronoiApp>();
}
