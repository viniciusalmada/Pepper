// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "ExampleLayer.hpp"

#include "IO.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

ExampleLayer::ExampleLayer() :
    Pepper::Layer("Example"),
    camera_controller(1280.f / 720.f, true),
    square_position(0.0f),
    square_color({ 0.2, 0.4, 0.7, 1.0f }),
    triangle_VAO(Pepper::VertexArray::Create()),
    square_VAO(Pepper::VertexArray::Create()),
    shader_library()
{
  triangle_VAO->Bind();
  {
    // -1.0 < x < 1.0
    // -1.0 < y < 1.0
    // clang-format off
      std::vector<float> vertices = {
        //  x,     y,     z,    r,    g,    b,    a
        -0.5f, -0.5f, +0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        +0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        +0.0f, +0.5f, +0.0f, 1.0f, 0.0f, 1.0f, 1.0f
      };
    // clang-format on

    auto vbo = Pepper::VertexBuffer::Create(vertices, triangle_VAO->GetRendererID());
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" },
                                    { Pepper::ShaderDataType::Float4, "in_color" } };

    vbo->SetLayout(layout);
    triangle_VAO->AddVertexBuffer(vbo);

    std::vector<uint32_t> indices = { 0, 1, 2 };
    auto ibo = Pepper::IndexBuffer::Create(indices, triangle_VAO->GetRendererID());
    triangle_VAO->SetIndexBuffer(ibo);
  }

  square_VAO->Bind();
  {
    // -1.0 < x < 1.0
    // -1.0 < y < 1.0
    // clang-format off
      std::vector<float> vertices = {
        //  x,     y,     z,    s,    t
        -1.6f, -0.9f, +0.0f, 0.0f, 0.0f, // bot-left
        -1.1f, -0.9f, +0.0f, 1.0f, 0.0f, // bot-right
        -1.1f, -0.4f, +0.0f, 1.0f, 1.0f, // top-right
        -1.6f, -0.4f, +0.0f, 0.0f, 1.0f, // top-left
      };
    // clang-format on

    auto vbo = Pepper::VertexBuffer::Create(vertices, square_VAO->GetRendererID());
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" },
                                    { Pepper::ShaderDataType::Float2, "in_tex_coord" } };
    vbo->SetLayout(layout);
    square_VAO->AddVertexBuffer(vbo);

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    auto ibo = Pepper::IndexBuffer::Create(indices, square_VAO->GetRendererID());
    square_VAO->SetIndexBuffer(ibo);
  };

  shader_library.Load(IO::GetAssets() / "shaders/Simple.glsl");
  shader_library.Load(IO::GetAssets() / "shaders/FlatColor.glsl");
  shader_library.Load(IO::GetAssets() / "shaders/Texture.glsl");

  texture = Pepper::Texture2D::Create(IO::GetAssets() / "textures/checkerboard.png");
  pepper_texture = Pepper::Texture2D::Create(IO::GetAssets() / "textures/black-pepper.png");

  shader_library.Get("Texture")->Bind();
  shader_library.Get("Texture")->SetInt("u_texture", 0);
}

void ExampleLayer::OnImGuiRender()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(square_color));
  ImGui::End();
}

void ExampleLayer::OnUpdate(Pepper::TimeStep ts)
{
  camera_controller.OnUpdate(ts);

  Pepper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Pepper::RenderCommand::Clear();

  // TRIANGLE
  // Pepper::Renderer::BeginScene(camera);
  // Pepper::Renderer::Submit(shader, triangle_VAO);
  // Pepper::Renderer::EndScene();

  // Pepper::Material* material = new Pepper::Material(shader);
  // material->Set("u_Color", redColor);
  // squareMesh->SetMaterial(material)

  Pepper::Renderer::BeginScene(camera_controller.GetCamera());
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  Pepper::Ref<Pepper::Shader> flat_color_shader = shader_library.Get("FlatColor");
  Pepper::Ref<Pepper::Shader> texture_shader = shader_library.Get("Texture");

  flat_color_shader->Bind();
  flat_color_shader->SetFloat4("u_color", square_color);
  for (int i = 0; i < 30; i++)
  {
    for (int j = 0; j < 20; j++)
    {
      glm::vec3 new_pos(i * 0.11f + (-1.44), j * 0.110f + (-0.81), 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, new_pos) * scale;
      Pepper::Renderer::Submit(flat_color_shader, square_VAO, transform);
    }
  }

  glm::mat4 new_scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
  glm::vec3 center_pos(1.6 * 2.5, 0.9 * 2.2, 0.0);
  glm::mat4 new_transf = glm::translate(glm::mat4{ 1.0f }, center_pos) * new_scale;
  texture->Bind();
  Pepper::Renderer::Submit(texture_shader, square_VAO, new_transf);
  pepper_texture->Bind();
  Pepper::Renderer::Submit(texture_shader, square_VAO, new_transf);
  Pepper::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Pepper::Event& e)
{
  camera_controller.OnEvent(e);
}
