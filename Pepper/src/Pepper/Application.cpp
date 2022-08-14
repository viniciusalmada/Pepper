#include "Application.hpp"

#include "Pepper/Input.hpp"

#include <GLFW/glfw3.h>
#include <PepperPCH.hpp>
#include <glad/glad.h>

const std::string vertex_src{ R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;
  layout(location = 1) in vec4 in_color;

  out vec4 v_color;

  void main()
  {
    gl_Position = vec4(in_position, 1.0);
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

  void main()
  {
    gl_Position = vec4(in_position, 1.0);
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

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Pepper::Application* Pepper::Application::app_instance = nullptr;

Pepper::Application::Application()
{
  PP_CORE_ASSERT(!app_instance, "Application already defined!");
  app_instance = this;

  window = std::unique_ptr<Window>(Window::Create());
  window->SetEventCallback(BIND_EVENT_FN(OnEvent));

  imGuiLayer = new ImGuiLayer();
  PushOverlay(imGuiLayer);

  triangle_VAO = VertexArray::Create();
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

    auto vbo = VertexBuffer::Create(vertices, sizeof(vertices), triangle_VAO->GetRendererID());
    BufferLayout layout = { { ShaderDataType::Float3, "in_position" }, { ShaderDataType::Float4, "in_color" } };

    vbo->SetLayout(layout);
    triangle_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2 };
    auto ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), triangle_VAO->GetRendererID());
    triangle_VAO->SetIndexBuffer(ibo);
  }

  square_VAO = VertexArray::Create();
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

    auto vbo = VertexBuffer::Create(vertices, sizeof(vertices), square_VAO->GetRendererID());
    BufferLayout layout = { { ShaderDataType::Float3, "in_position" } };
    vbo->SetLayout(layout);
    square_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    auto ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), square_VAO->GetRendererID());
    square_VAO->SetIndexBuffer(ibo);
  }
  shader = std::make_unique<Shader>(vertex_src, fragment_src);
  blue_shader = std::make_unique<Shader>(blue_vertex_src, blue_fragment_src);

  // vertex_buffer->Unbind();
  // vertex_buffer2->Unbind();
  // index_buffer->Unbind();
  // index_buffer2->Unbind();

  triangle_VAO->Unbind();
  square_VAO->Unbind();
}

void Pepper::Application::PushLayer(Layer* layer) { layer_stack.PushLayer(layer); }

void Pepper::Application::PushOverlay(Layer* overlay) { layer_stack.PushOverlay(overlay); }

void Pepper::Application::OnEvent(Event& e)
{
  EventDispatcher dispatcher{ e };
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

  // From top to bottom of stack check event handled
  for (auto iter = layer_stack.end(); iter != layer_stack.begin();)
  {
    (*--iter)->OnEvent(e);
    if (e.IsHandled())
      break;
  }
}

void Pepper::Application::Run()
{
  while (running)
  {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Bind();
    triangle_VAO->Bind();
    glDrawElements(GL_TRIANGLES, triangle_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    blue_shader->Bind();
    square_VAO->Bind();
    glDrawElements(GL_TRIANGLES, square_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    for (Layer* layer : layer_stack)
      layer->OnUpdate();

    imGuiLayer->Begin();
    for (Layer* layer : layer_stack)
      layer->OnImGuiRender();
    imGuiLayer->End();

    window->OnUpdate();
  }
}

bool Pepper::Application::OnWindowClose(WindowCloseEvent&)
{
  running = false;
  return true;
}
