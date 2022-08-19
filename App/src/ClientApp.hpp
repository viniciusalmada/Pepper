#pragma once

#include <Pepper.hpp>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer();

  void OnImGuiRender() override;

  void OnUpdate(Pepper::Timestep timestep) override;

  void OnEvent(Pepper::Event&) override;

private:
  Pepper::Ref<Pepper::Shader> shader;
  Pepper::Ref<Pepper::Shader> flat_color_shader;

  Pepper::Ref<Pepper::VertexArray> triangle_VAO;
  Pepper::Ref<Pepper::VertexArray> square_VAO;

  Pepper::OrthoCamera camera;
  const float CAMERA_MOVE_SPEED = 5.0f;
  const float CAMERA_ROTATION_SPEED = 180.0f;
  const float SQUARE_MOVE_SPEED = 1.0f;

  glm::vec3 square_position;
  glm::vec3 square_color;
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp();
  ~ClientApp();
};