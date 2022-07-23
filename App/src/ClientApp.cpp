#include <PepperPCH.hpp>
#include <Pepper.hpp>

class ClientApp : public Pepper::Application
{
public:
  ClientApp() {}
  ~ClientApp() {}
};

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }