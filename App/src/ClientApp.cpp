// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

#include "GameLayer.hpp"

#include <Pepper/Core/EntryPoint.hpp>

ClientApp::ClientApp()
{
  PP_PROFILE_FUNCTION()
  PushLayer(Pepper::CreateRef<GameLayer>());
}

ClientApp::~ClientApp() = default;

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  PP_PROFILE_FUNCTION()
  return CreateScope<ClientApp>();
}
