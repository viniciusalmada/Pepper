#pragma once

#ifdef BT_PLATFORM_WINDOWS

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Butter::Application* Butter::CreateApplication();

int main()
{
  auto* app = Butter::CreateApplication();
  app->Run();
  delete app;
}

#endif