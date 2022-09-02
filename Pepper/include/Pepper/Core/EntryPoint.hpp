#pragma once

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Pepper::Scope<Pepper::Application> Pepper::CreateApplication();

int main()
{
  Pepper::Log::Init();
  PP_WARN("Initialized Log!");
  int a = 5;
  PP_INFO("App running! {0}", a);

  auto app = Pepper::CreateApplication();
  app->Run();
}