// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Sandbox2D.hpp"

#include "IO.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

Sandbox2D::Sandbox2D() :
    Layer("Sandbox 2D"),
    camera_controller(1280.0f / 720.0f, true),
    square_color({ 0.2f, 0.3f, 0.8f })
{
}

void Sandbox2D::OnAttach() {}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Pepper::TimeStep ts)
{
  camera_controller.OnUpdate(ts);

  // Pepper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  // Pepper::RenderCommand::Clear();

  Pepper::Renderer2D::BeginScene(camera_controller.GetCamera());

  Pepper::Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 0.2, 0.2 }, { 1.0f, 0.2f, 0.3f, 1.0f });

  Pepper::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(square_color));
  ImGui::End();
}

void Sandbox2D::OnEvent(Pepper::Event& e)
{
  camera_controller.OnEvent(e);
}
