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
  std::shared_ptr<Pepper::Shader> shader;
  std::shared_ptr<Pepper::Shader> flat_color_shader;

  std::shared_ptr<Pepper::VertexArray> triangle_VAO;
  std::shared_ptr<Pepper::VertexArray> square_VAO;

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