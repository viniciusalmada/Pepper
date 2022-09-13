#pragma once

#include <Pepper.hpp>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer();

  void OnImGuiRender() override;

  void OnUpdate(Pepper::TimeStep) override;

  void OnEvent(Pepper::Event&) override;

private:
  // Pepper::Ref<Pepper::Shader> shader;
  // Pepper::Ref<Pepper::Shader> flat_color_shader;
  // Pepper::Ref<Pepper::Shader> texture_shader;
  Pepper::ShaderLibrary shader_library;

  Pepper::Ref<Pepper::VertexArray> triangle_VAO;
  Pepper::Ref<Pepper::VertexArray> square_VAO;

  Pepper::Ref<Pepper::Texture2D> texture;
  Pepper::Ref<Pepper::Texture2D> pepper_texture;

  Pepper::OrthoCameraController camera_controller;

  const float SQUARE_MOVE_SPEED = 1.0f;

  glm::vec3 square_position;
  glm::vec4 square_color;
};