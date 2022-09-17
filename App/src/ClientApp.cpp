// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

#include "ExampleLayer.hpp"
#include "Sandbox2D.hpp"

#include <Pepper/Core/EntryPoint.hpp>

ClientApp::ClientApp()
{
  // PushLayer(Pepper::CreateRef<ExampleLayer>());
  PP_PROFILE_FUNCTION();
  PushLayer(Pepper::CreateRef<Sandbox2D>());
}

ClientApp::~ClientApp() {}

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  return CreateScope<ClientApp>();
}