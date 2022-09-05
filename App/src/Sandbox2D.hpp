#pragma once

#include <Pepper.hpp>

class Sandbox2D : public Pepper::Layer
{
public:
  Sandbox2D();

  void OnAttach() override;

  void OnDetach() override;

  void OnUpdate(Pepper::TimeStep) override;

  void OnImGuiRender() override;

  void OnEvent(Pepper::Event&) override;

private:
  Pepper::OrthoCameraController camera_controller;

  Pepper::Ref<Pepper::VertexArray> vertex_array;
  Pepper::Ref<Pepper::Shader> shader;
  Pepper::Ref<Pepper::Texture2D> texture;

  glm::vec3 square_color;
};