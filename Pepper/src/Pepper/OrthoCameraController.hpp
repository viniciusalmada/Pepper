#pragma once

#include "Pepper/Core/TimeStep.hpp"
#include "Pepper/Events/MouseEvent.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Renderer/OrthoCamera.hpp"

namespace Pepper
{
  class OrthoCameraController
  {
  public:
    OrthoCameraController(float aspectRatio, bool enableRotation = false);
    ~OrthoCameraController();

    void OnUpdate(TimeStep ts);
    void OnEvent(Event& e);

    const OrthoCamera& GetCamera() const;

    float GetZoomLevel();
    void SetZoomLevel(float level);

  private:
    DECLARE_PIMPL
    
  };
}