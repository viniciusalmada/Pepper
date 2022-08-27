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
    void OnEvent(Event& e);

    const OrthoCamera& GetCamera() const { return camera; }

  private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

    CameraLimits GetLimits() const;
    OrthoCamera BuildCamera() const;

  private:
    float aspect_ratio;
    bool rotation;
    float zoom_level = 1.0f;
    OrthoCamera camera;

    // TODO: calculate move speed based on zoom level
    float camera_move_speed = 5.0f;
    static constexpr float CAMERA_ROTATION_SPEED = 180.0f;
  };
}