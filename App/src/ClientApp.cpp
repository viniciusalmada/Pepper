// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

const std::string vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;
  layout(location = 1) in vec4 in_color;

  uniform mat4 u_view_projection;

  out vec4 v_color;

  void main()
  {
    gl_Position = u_view_projection * vec4(in_position, 1.0);
    v_color = in_color;
  }
)glsl" };

const std::string fragment_src{ R"glsl(
  #version 330 core

  in vec4 v_color;
  out vec4 out_color;

  void main()
  {
    out_color = v_color;
  }
)glsl" };

const std::string blue_vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;

  uniform mat4 u_view_projection;

  void main()
  {
    gl_Position = u_view_projection * vec4(in_position, 1.0);
  }
)glsl" };

const std::string blue_fragment_src{ R"glsl(
  #version 330 core

  out vec4 out_color;

  void main()
  {
    out_color = vec4(0.1, 0.1, 0.8, 1.0);
  }
)glsl" };

ExampleLayer::ExampleLayer() : Pepper::Layer("Example")
{
  triangle_VAO = Pepper::VertexArray::Create();
  triangle_VAO->Bind();
  {
    // -1.0 < x < 1.0
    // -1.0 < y < 1.0
    // clang-format off
      float vertices[] = {
        //  x,     y,     z,    r,    g,    b,    a
        -0.5f, -0.5f, +0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        +0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        +0.0f, +0.5f, +0.0f, 1.0f, 0.0f, 1.0f, 1.0f
      };
    // clang-format on

    auto vbo = Pepper::VertexBuffer::Create(vertices, sizeof(vertices), triangle_VAO->GetRendererID());
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" },
                                    { Pepper::ShaderDataType::Float4, "in_color" } };

    vbo->SetLayout(layout);
    triangle_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2 };
    auto ibo = Pepper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), triangle_VAO->GetRendererID());
    triangle_VAO->SetIndexBuffer(ibo);
  }

  square_VAO = Pepper::VertexArray::Create();
  square_VAO->Bind();
  {
    // -1.0 < x < 1.0
    // -1.0 < y < 1.0
    // clang-format off
      float vertices[] = {
        //  x,     y,     z
        +0.5f, +0.5f, +0.0f,
        +0.9f, +0.5f, +0.0f,
        +0.9f, +0.9f, +0.0f,
        +0.5f, +0.9f, +0.0f,
      };
    // clang-format on

    auto vbo = Pepper::VertexBuffer::Create(vertices, sizeof(vertices), square_VAO->GetRendererID());
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" } };
    vbo->SetLayout(layout);
    square_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    auto ibo = Pepper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), square_VAO->GetRendererID());
    square_VAO->SetIndexBuffer(ibo);
  }
  shader = std::make_shared<Pepper::Shader>(vertex_src, fragment_src);
  blue_shader = std::make_shared<Pepper::Shader>(blue_vertex_src, blue_fragment_src);
}

void ExampleLayer::OnImGuiRender()
{
  // ImGui::Begin("Pepper");
  // ImGui::Text("Hello, Pepper!");
  // ImGui::End();
}

void ExampleLayer::OnUpdate()
{
  // if (Pepper::Input::IsKeyPressed(PP_KEY_TAB))
  // PP_INFO("Tab pressed");

  Pepper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Pepper::RenderCommand::Clear();

  Pepper::Renderer::BeginScene();

  shader->Bind();
  Pepper::Renderer::Submit(triangle_VAO);

  blue_shader->Bind();
  Pepper::Renderer::Submit(square_VAO);

  Pepper::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Pepper::Event&) {}

ClientApp::ClientApp() { PushLayer(new ExampleLayer{}); }

ClientApp::~ClientApp() {}

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }