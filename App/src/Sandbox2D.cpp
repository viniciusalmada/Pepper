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
  PP_PROFILE_FUNCTION();
}

void Sandbox2D::OnAttach()
{
  PP_PROFILE_FUNCTION();

  texture = Pepper::Texture2D::Create(R"(assets/textures/checkerboard.png)");
}

void Sandbox2D::OnDetach()
{
  PP_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION();
  camera_controller.OnUpdate(ts);

  {
    PP_PROFILE_SCOPE("Clear");
    Pepper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Pepper::RenderCommand::Clear();
  }

  {
    PP_PROFILE_SCOPE("Render");
    Pepper::Renderer2D::BeginScene(camera_controller.GetCamera());

    Pepper::Renderer2D::DrawQuad({ 0.0, 0.0 }, { 0.75, 0.75 }, { 0.0f, 1.2f, 0.3f, 1.0f });
    Pepper::Renderer2D::DrawQuad({ -1.0, -1.0, 0.1 }, { 0.25, 0.25 }, { 1.0f, 0.2f, 0.3f, 1.0f });
    Pepper::Renderer2D::DrawQuad({ .0, .0, 0.2 }, { 0.25, 0.25 }, { 1.0f, 0.2f, 0.3f, 1.0f });
    Pepper::Renderer2D::DrawQuad({ -1.0, -1.0, 0.3 }, { 0.10, 0.10 }, { 0.0f, 0.2f, 0.3f, 1.0f });

    //    Pepper::Renderer2D::DrawQuad({ 0., 0., -0.9 }, { 10., 10. }, texture, 10.0f);
    //    Pepper::Renderer2D::DrawQuad({ 0.f, 0.f }, { 0.2, 0.2 }, { 0.0f, 0.2f, 0.3f, 1.0f });
    //
    //    Pepper::Renderer2D::DrawRotatedQuad({ 0.f, 0.f, 0.5f },
    //                                        { 0.5, 0.5 },
    //                                        45.f,
    //                                        texture,
    //                                        20.f,
    //                                        { 0.0f, 0.0f, 1.0f, 1.0f });
    //    Pepper::Renderer2D::DrawRotatedQuad({ 0.f, 0.f, 0.51f }, { 0.4, 0.4 }, 45.f, { 1.0f, 0.2f,
    //    0.3f, 1.0f });

    // DrawRuler();

    Pepper::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender()
{
  PP_PROFILE_FUNCTION();
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(square_color));
  ImGui::End();
}

void Sandbox2D::OnEvent(Pepper::Event& e)
{
  camera_controller.OnEvent(e);
}

void Sandbox2D::DrawRuler() const
{
  // Draw y axis
  for (const int i : std::ranges::iota_view{ -25, 25 })
    Pepper::Renderer2D::DrawQuad({ 0.0, -0.1 * i }, { 0.01, 0.01 }, { 1.0f, 1.0f, 1.0f, 1.0f });

  // // Draw x axis
  for (const int i : std::ranges::iota_view{ -25, 25 })
    Pepper::Renderer2D::DrawQuad({ -0.1 * i, 0.0 }, { 0.01, 0.01 }, { 1.0f, 1.0f, 1.0f, 1.0f });
}
