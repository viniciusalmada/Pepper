#pragma once

#include "Pepper/Core/Timestep.hpp"
#include "Pepper/Events/MouseEvent.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Renderer/OrthoCamera.hpp"

namespace Pepper
{
  class OrthoCameraController
  {
  public:
    OrthoCameraController(float aspectRatio, bool enableRotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e) const;

  private:
    bool OnMouseScrolled(MouseScrolledEvent& e) const;
    bool OnWindowResized(WindowResizeEvent& e) const;

    OrthoCamera BuildCamera() const;

  private:
    float aspect_ratio;
    bool rotation;
    float zoom_level = 1.0f;
    OrthoCamera camera;

    // TODO: calculate move speed based on zoom level
    static const float CAMERA_MOVE_SPEED = 5.0f;
    static const float CAMERA_ROTATION_SPEED = 180.0f;
  };
}