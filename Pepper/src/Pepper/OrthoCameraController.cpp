// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "OrthoCameraController.hpp"

#include "Pepper/Input/Input.hpp"
#include "Pepper/Input/KeyCodes.hpp"

namespace Pepper
{
  OrthoCameraController::OrthoCameraController(float aspectRatio, bool enableRotation) :
      aspect_ratio(aspectRatio),
      rotation(enableRotation),
      camera(BuildCamera())
  {
  }

  void OrthoCameraController::OnUpdate(Timestep ts)
  {
    glm::vec3 pos = camera.GetPosition();
    if (Input::IsKeyPressed(PP_KEY_W))
      pos.y -= camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_A))
      pos.x += camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_S))
      pos.y += camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_D))
      pos.x -= camera_move_speed * ts;
    camera.SetPosition(pos);

    float rot_deg = camera.GetRotation();
    if (Input::IsKeyPressed(PP_KEY_Q))
      rot_deg += CAMERA_ROTATION_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_E))
      rot_deg -= CAMERA_ROTATION_SPEED * ts;
    camera.SetRotationDeg(rot_deg);
  }

  void OrthoCameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(PP_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(PP_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
  }

  bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
  {

    zoom_level -= e.GetYOffset() / 5.0f;
    zoom_level = std::max(zoom_level, MIN_ZOOM_LEVEL);
    zoom_level = std::min(zoom_level, MAX_ZOOM_LEVEL);

    camera.SetProjection(GetLimits());
    return false;
  }

  bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
  {
    aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();
    camera.SetProjection(GetLimits());
    return false;
  }

  CameraLimits OrthoCameraController::GetLimits() const
  {
    float left = -aspect_ratio * zoom_level;
    float right = aspect_ratio * zoom_level;
    float bottom = -zoom_level;
    float top = zoom_level;
    return { left, right, bottom, top };
  }

  OrthoCamera OrthoCameraController::BuildCamera() const
  {
    return OrthoCamera{ GetLimits() };
  }

} // namespace Pepper
