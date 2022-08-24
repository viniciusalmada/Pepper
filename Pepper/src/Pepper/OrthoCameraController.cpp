// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "OrthoCameraController.hpp"

#include "Pepper/Input/Input.hpp"
#include "Pepper/Input/KeyCodes.hpp"

namespace Pepper
{
  OrthoCameraController::OrthoCameraController(float aspectRatio, bool enableRotation)
      : aspect_ratio(aspectRatio), rotation(enableRotation), camera(BuildCamera())
  {
  }

  void OrthoCameraController::OnUpdate(Timestep ts)
  {
    glm::vec3 pos = camera.GetPosition();
    if (Input::IsKeyPressed(PP_KEY_W))
      pos.y -= CAMERA_MOVE_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_A))
      pos.x += CAMERA_MOVE_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_S))
      pos.y += CAMERA_MOVE_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_D))
      pos.x -= CAMERA_MOVE_SPEED * ts;
    camera.SetPosition(pos);

    float rot_deg = camera.GetRotation();
    if (Input::IsKeyPressed(PP_KEY_Q))
      rot_deg += CAMERA_ROTATION_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_E))
      rot_deg -= CAMERA_ROTATION_SPEED * ts;
    camera.SetRotationDeg(rot_deg);
  }

  void OrthoCameraController::OnEvent(Event& e) const
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(PP_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(PP_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
  }

  bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e) const {}

  bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e) const {}

  OrthoCamera OrthoCameraController::BuildCamera() const
  {
    float left = -aspect_ratio * zoom_level;
    float right = aspect_ratio * zoom_level;
    float bottom = -zoom_level;
    float top = zoom_level;
    return { left, right, bottom, top };
  }

} // namespace Pepper
