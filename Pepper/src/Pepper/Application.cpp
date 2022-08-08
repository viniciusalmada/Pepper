#include "Application.hpp"

#include "Pepper/Input.hpp"

#include <PepperPCH.hpp>
#include <glad/glad.h>

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

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

  // -1.0 < x < 1.0
  // -1.0 < y < 1.0
  // clang-format off
  float vertices[9] = {
    -0.5f, -0.5f, +0.0f,
    +0.5f, -0.5f, +0.0f,
    +0.0f, +0.5f, +0.0f,
  };
  // clang-format on
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);

  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  unsigned int indices[3] = { 0, 1, 2 };

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

Pepper::Application::~Application() {}

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

    glBindVertexArray(vertex_array);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
