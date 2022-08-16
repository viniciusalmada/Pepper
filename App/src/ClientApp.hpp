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
};


class ClientApp : public Pepper::Application
{
public:
  ClientApp();
  ~ClientApp();
};