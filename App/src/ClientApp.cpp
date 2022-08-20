// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "ClientApp.hpp"

#include <Pepper/Platform/OpenGL/OpenGLShader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

const std::string vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;
  layout(location = 1) in vec4 in_color;

  uniform mat4 u_view_projection;
  uniform mat4 u_transform;

  out vec4 v_color;

  void main()
  {
    gl_Position = u_view_projection * u_transform *  vec4(in_position, 1.0);
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

const std::string flat_color_vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;

  uniform mat4 u_view_projection;
  uniform mat4 u_transform ;

  void main()
  {
    gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
  }
)glsl" };

const std::string flat_color_fragment_src{ R"glsl(
  #version 330 core

  out vec4 out_color;

  uniform vec3 u_color;

  void main()
  {
    out_color = vec4(u_color, 1.0);
  }
)glsl" };

const std::string texture_vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;
  layout(location = 1) in vec2 in_tex_coords;

  uniform mat4 u_view_projection;
  uniform mat4 u_transform ;

  out vec2 v_tex_coords;

  void main()
  {
    gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
    v_tex_coords = in_tex_coords;
  }
)glsl" };

const std::string texture_fragment_src{ R"glsl(
  #version 330 core

  in vec2 v_tex_coords;

  out vec4 out_color;

  uniform sampler2D u_texture;

  void main()
  {
    out_color = texture(u_texture, v_tex_coords);
  }
)glsl" };

ExampleLayer::ExampleLayer()
    : Pepper::Layer("Example"), camera({ -1.6f, 1.6f, -0.9f, 0.9f }), square_position(0.0f),
      square_color({ 0.2, 0.4, 0.7 }), triangle_VAO(Pepper::Ref<Pepper::VertexArray>{ Pepper::VertexArray::Create() }),
      square_VAO(Pepper::Ref<Pepper::VertexArray>{ Pepper::VertexArray::Create() })
{
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

    auto vbo = Pepper::Ref<Pepper::VertexBuffer>{
      Pepper::VertexBuffer::Create(vertices, sizeof(vertices), triangle_VAO->GetRendererID())
    };
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" },
                                    { Pepper::ShaderDataType::Float4, "in_color" } };

    vbo->SetLayout(layout);
    triangle_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2 };
    auto ibo = Pepper::Ref<Pepper::IndexBuffer>{
      Pepper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), triangle_VAO->GetRendererID())
    };
    triangle_VAO->SetIndexBuffer(ibo);
  }

  square_VAO->Bind();
  {
    // -1.0 < x < 1.0
    // -1.0 < y < 1.0
    // clang-format off
      float vertices[] = {
        //  x,     y,     z,    s,    t
        -1.6f, -0.9f, +0.0f, 0.0f, 0.0f, // bot-left
        -1.1f, -0.9f, +0.0f, 1.0f, 0.0f, // bot-right
        -1.1f, -0.4f, +0.0f, 1.0f, 1.0f, // top-right
        -1.6f, -0.4f, +0.0f, 0.0f, 1.0f, // top-left
      };
    // clang-format on

    auto vbo = Pepper::Ref<Pepper::VertexBuffer>{
      Pepper::VertexBuffer::Create(vertices, sizeof(vertices), square_VAO->GetRendererID())
    };
    Pepper::BufferLayout layout = { { Pepper::ShaderDataType::Float3, "in_position" },
                                    { Pepper::ShaderDataType::Float2, "in_tex_coord" } };
    vbo->SetLayout(layout);
    square_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    auto ibo = Pepper::Ref<Pepper::IndexBuffer>{
      Pepper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), square_VAO->GetRendererID())
    };
    square_VAO->SetIndexBuffer(ibo);
  }

  shader = Pepper::Ref<Pepper::Shader>{ Pepper::Shader::Create(vertex_src, fragment_src) };
  flat_color_shader =
    Pepper::Ref<Pepper::Shader>{ Pepper::Shader::Create(flat_color_vertex_src, flat_color_fragment_src) };
  texture_shader = Pepper::Ref<Pepper::Shader>{ Pepper::Shader::Create(texture_vertex_src, texture_fragment_src) };

  
  std::filesystem::current_path(std::filesystem::current_path().parent_path() / "App");
  texture = Pepper::Texture2D::Create("assets/checkerboard.png");
  texture->Bind();

  std::dynamic_pointer_cast<Pepper::OpenGLShader>(texture_shader)->Bind();
  std::dynamic_pointer_cast<Pepper::OpenGLShader>(texture_shader)->UploadUniformInt("u_texture", 0);

  camera.SetPosition({ 0.0f, 0.0f, 0.0f });
  // camera.SetRotationDeg(45.0f);
}

void ExampleLayer::OnImGuiRender()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(square_color));
  ImGui::End();
}

void ExampleLayer::OnUpdate(Pepper::Timestep ts)
{
  // PP_TRACE("dT: {0}ms ({1}ms)", timestep.GetMilliSeconds(), timestep.GetSeconds());
  glm::vec3 pos = camera.GetPosition();
  if (Pepper::Input::IsKeyPressed(PP_KEY_W))
    pos.y -= CAMERA_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_A))
    pos.x += CAMERA_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_S))
    pos.y += CAMERA_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_D))
    pos.x -= CAMERA_MOVE_SPEED * ts;
  camera.SetPosition(pos);

  float rot_deg = camera.GetRotation();
  if (Pepper::Input::IsKeyPressed(PP_KEY_UP))
    rot_deg += CAMERA_ROTATION_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_DOWN))
    rot_deg -= CAMERA_ROTATION_SPEED * ts;
  camera.SetRotationDeg(rot_deg);

  if (Pepper::Input::IsKeyPressed(PP_KEY_H))
    square_position.x -= SQUARE_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_J))
    square_position.y -= SQUARE_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_K))
    square_position.y += SQUARE_MOVE_SPEED * ts;
  else if (Pepper::Input::IsKeyPressed(PP_KEY_L))
    square_position.x += SQUARE_MOVE_SPEED * ts;

  Pepper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Pepper::RenderCommand::Clear();

  // TRIANGLE
  // Pepper::Renderer::BeginScene(camera);
  // Pepper::Renderer::Submit(shader, triangle_VAO);
  // Pepper::Renderer::EndScene();

  // Pepper::Material* material = new Pepper::Material(shader);
  // material->Set("u_Color", redColor);
  // squareMesh->SetMaterial(material)

  Pepper::Renderer::BeginScene(camera);
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  std::dynamic_pointer_cast<Pepper::OpenGLShader>(flat_color_shader)->Bind();
  std::dynamic_pointer_cast<Pepper::OpenGLShader>(flat_color_shader)->UploadUniformFloat3("u_color", square_color);
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
  Pepper::Renderer::Submit(texture_shader, square_VAO, new_transf);
  Pepper::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Pepper::Event&) {}

ClientApp::ClientApp() { PushLayer(new ExampleLayer{}); }

ClientApp::~ClientApp() {}

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }