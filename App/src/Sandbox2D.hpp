#pragma once

#include <Pepper.hpp>
#include <list>

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
  void DrawRuler() const;

  Pepper::OrthoCameraController camera_controller;

  Pepper::Ref<Pepper::VertexArray> vertex_array;
  Pepper::Ref<Pepper::Shader> shader;
  Pepper::Ref<Pepper::Texture2D> texture;
  
  glm::vec3 square_color;

  struct ProfileResult
  {
    const std::string name;
    float time;
  };

  std::list<ProfileResult> profile;
  
};