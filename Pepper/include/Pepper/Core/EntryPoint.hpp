#pragma once

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Pepper::Scope<Pepper::Application> Pepper::CreateApplication();

int main()
{
  Pepper::Log::Init();
  auto app = Pepper::CreateApplication();

  app->Run();

  auto* app_ptr = app.release();
  delete app_ptr;
}