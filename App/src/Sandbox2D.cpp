// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Sandbox2D.hpp"

#include "IO.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox 2D"), camera_controller(1280.0f / 720.0f, true), square_color({0.2f, 0.3f, 0.8f})
{
}

void Sandbox2D::OnAttach()
{
  vertex_array = Pepper::VertexArray::Create();

  // clang-format off
  std::vector<float> square_vertices = {
    -0.5f, -0.5f, -0.0f,
     0.5f, -0.5f, -0.0f,
     0.5f,  0.5f, -0.0f,
    -0.5f,  0.5f, -0.0f,
  };
  // clang-format on

  auto square_vb = Pepper::VertexBuffer::Create(square_vertices, vertex_array->GetRendererID());
  square_vb->SetLayout({ { Pepper::ShaderDataType::Float3, "in_position" } });
  vertex_array->AddVertexBuffer(square_vb);

  std::vector<uint32_t> square_indices = { 0, 1, 2, 2, 3, 0 };
  auto square_ib = Pepper::IndexBuffer::Create(square_indices, vertex_array->GetRendererID());
  vertex_array->SetIndexBuffer(square_ib);

  shader = Pepper::Shader::Create(IO::GetAssets() / "shaders/FlatColor");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Pepper::TimeStep) {}

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
