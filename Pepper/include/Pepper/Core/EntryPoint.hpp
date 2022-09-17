#pragma once

// Expected to be defined in client.
// With this, the main function does not needed to be defined in client
// application
extern Pepper::Scope<Pepper::Application> Pepper::CreateApplication();

int main()
{
  Pepper::Log::Init();
  PP_PROFILE_BEGIN_SESSION("PPProfile-Startup.json");
  auto app = Pepper::CreateApplication();
  PP_PROFILE_END_SESSION();

  PP_PROFILE_BEGIN_SESSION("PPProfile-Runtime.json");
  app->Run();
  PP_PROFILE_END_SESSION();

  PP_PROFILE_BEGIN_SESSION("PPProfile-Shutdown.json");
  auto* app_ptr = app.release();
  delete app_ptr;
  PP_PROFILE_END_SESSION();
  
}