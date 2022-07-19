#pragma once

#ifdef BT_PLATFORM_WINDOWS

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Butter::Application* Butter::CreateApplication();

int main()
{
  Butter::Log::Init();
  BT_CORE_WARN("Initialized Log!");
  int a = 5;
  BT_INFO("App running! {0}", a);

  auto* app = Butter::CreateApplication();
  app->Run();
  delete app;
}

#endif