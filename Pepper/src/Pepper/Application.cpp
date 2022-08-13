#include "Application.hpp"

#include "Pepper/Input.hpp"

#include <PepperPCH.hpp>
#include <glad/glad.h>

const std::string vertex_src = R"glsl(
  #version 330 core

  layout(location = 0) in vec3 in_position;
  layout(location = 1) in vec3 in_color;

  out vec3 v_color;

  void main()
  {
    gl_Position = vec4(in_position, 1.0);
    v_color = in_color;
  }
)glsl";

const std::string fragment_src = R"glsl(
  #version 330 core

  in vec3 v_color;
  out vec4 out_color;

  void main()
  {
    out_color = vec4(v_color, 1.0);
  }
)glsl";

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

  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  // -1.0 < x < 1.0
  // -1.0 < y < 1.0
  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, +0.0f, 0.0f, 1.0f, 1.0f,
    +0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 0.0f,
    +0.0f, +0.5f, +0.0f, 1.0f, 0.0f, 1.0f,
  };
  // clang-format on

  vertex_buffer = std::unique_ptr<VertexBuffer>(
    VertexBuffer::Create(vertices, sizeof(vertices)));

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        false,
                        6 * sizeof(float),
                        (void*)(3 * sizeof(float)));

  uint32_t indices[] = { 0, 1, 2 };
  index_buffer = std::unique_ptr<IndexBuffer>(
    IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

  shader = std::make_unique<Shader>(vertex_src, fragment_src);
}

void Pepper::Application::PushLayer(Layer* layer)
{
  layer_stack.PushLayer(layer);
}

void Pepper::Application::PushOverlay(Layer* overlay)
{
  layer_stack.PushOverlay(overlay);
}

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
    glBindVertexArray(vertex_array);
    glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
