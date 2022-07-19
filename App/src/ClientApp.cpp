#include <ButterPCH.hpp>
#include <Butter.hpp>

class ClientApp : public Butter::Application
{
public:
  ClientApp() {}
  ~ClientApp() {}
};

Butter::Application* Butter::CreateApplication() { return new ClientApp(); }