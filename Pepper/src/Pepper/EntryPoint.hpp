#pragma once

#ifdef BT_PLATFORM_WINDOWS

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Pepper::Application* Pepper::CreateApplication();

int main()
{
  Pepper::Log::Init();
  BT_CORE_WARN("Initialized Log!");
  int a = 5;
  BT_INFO("App running! {0}", a);

  auto* app = Pepper::CreateApplication();
  app->Run();
  delete app;
}

#endif