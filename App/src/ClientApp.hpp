#pragma once

#include <Pepper.hpp>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer();

  void OnImGuiRender() override;

  void OnUpdate() override;

  void OnEvent(Pepper::Event&) override;

private:
  std::shared_ptr<Pepper::Shader> shader;
  std::shared_ptr<Pepper::Shader> blue_shader;

  std::shared_ptr<Pepper::VertexArray> triangle_VAO;
  std::shared_ptr<Pepper::VertexArray> square_VAO;

  Pepper::OrthoCamera camera;
  const float CAMERA_MOVE_SPEED = 0.01f;
  const float CAMERA_ROTATION_SPEED = 2.5f;
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp();
  ~ClientApp();
};