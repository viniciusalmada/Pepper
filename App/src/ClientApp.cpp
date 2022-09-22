// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

#include "GameLayer.hpp"

#include <Pepper/Core/EntryPoint.hpp>

ClientApp::ClientApp()
{
  PushLayer(Pepper::CreateRef<GameLayer>());
}

ClientApp::~ClientApp() {}

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  return CreateScope<ClientApp>();
}