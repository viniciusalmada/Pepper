// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Layers/Layer.hpp"

namespace Pepper
{
  class Layer::Impl
  {
  public:
    std::string debugName;
  };

  Layer::Layer(const std::string& name) : pimp(new Impl{})
  {
    pimp->debugName = name;
  }

  Layer::~Layer() = default;

  void Layer::OnAttach() {}

  void Layer::OnDetach() {}

  void Layer::OnUpdate(TimeStep) {}

  void Layer::OnImGuiRender() {}

  void Layer::OnEvent(Event&) {}

  const std::string& Layer::GetName() const
  {
    return pimp->debugName;
  }
}
