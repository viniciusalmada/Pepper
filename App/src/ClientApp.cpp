// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

#include "ExampleLayer.hpp"

#include <Pepper/Core/EntryPoint.hpp>

ClientApp::ClientApp()
{
  PushLayer(Pepper::CreateRef<ExampleLayer>());
}

ClientApp::~ClientApp() {}

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  return CreateScope<ClientApp>();
}