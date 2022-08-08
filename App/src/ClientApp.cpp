// clang-format off
#include <Pepper.hpp>
#include <PepperPCH.hpp>
// clang-format on

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>   // glm::vec3
#include <glm/vec4.hpp>   // glm::vec4
#include <imgui.h>
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
  glm::mat4 Projection =
    glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 View =
    glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
  View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return Projection * View * Model;
}

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer() : Layer("Example") { auto a = camera(5.0f, { 1.0f, 2.0f }); }

  void OnImGuiRender() override
  {
    ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
  }

  void OnUpdate() override
  {
    if (Pepper::Input::IsKeyPressed(PP_KEY_TAB))
      PP_INFO("Tab pressed");
  }

  void OnEvent(Pepper::Event&) override
  { 
  }
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp() { PushLayer(new ExampleLayer{}); }
  ~ClientApp() {}
};

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }