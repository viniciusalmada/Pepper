// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/OrthoCameraController.hpp"

#include "Pepper/Events/EventDispatcher.hpp"
#include "Pepper/Input/Input.hpp"
#include "Pepper/Input/KeyCodes.hpp"

namespace Pepper
{
  static constexpr float CAMERA_ROTATION_SPEED = 180.0f;
  static constexpr float MAX_ZOOM_LEVEL = 4.0f;
  static constexpr float MIN_ZOOM_LEVEL = 0.25f;

  class OrthoCameraController::Impl
  {
  public:
    Impl(float aspectRatio, bool enableRotation);

    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

    CameraLimits GetLimits() const;
    OrthoCamera BuildCamera() const;

    float aspect_ratio;
    bool rotation;
    float zoom_level = 1.0f;
    OrthoCamera camera;
    float camera_move_speed = 5.0f;
  };

  OrthoCameraController::Impl::Impl(float aspectRatio, bool enableRotation) :
      aspect_ratio(aspectRatio),
      rotation(enableRotation),
      camera(BuildCamera())
  {
  }

  OrthoCameraController::OrthoCameraController(float aspectRatio, bool enableRotation) :
      pimp(new Impl{ aspectRatio, enableRotation })
  {
  }

  OrthoCameraController::~OrthoCameraController() = default;

  void OrthoCameraController::OnUpdate(TimeStep ts)
  {
    glm::vec3 pos = pimp->camera.GetPosition();
    if (Input::IsKeyPressed(PP_KEY_W))
      pos.y -= pimp->camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_A))
      pos.x += pimp->camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_S))
      pos.y += pimp->camera_move_speed * ts;
    else if (Input::IsKeyPressed(PP_KEY_D))
      pos.x -= pimp->camera_move_speed * ts;
    pimp->camera.SetPosition(pos);

    float rot_deg = pimp->camera.GetRotation();
    if (Input::IsKeyPressed(PP_KEY_Q))
      rot_deg += CAMERA_ROTATION_SPEED * ts;
    else if (Input::IsKeyPressed(PP_KEY_E))
      rot_deg -= CAMERA_ROTATION_SPEED * ts;
    pimp->camera.SetRotationDeg(rot_deg);
  }

  void OrthoCameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
      std::bind(&OrthoCameraController::Impl::OnMouseScrolled, pimp.get(), std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
      std::bind(&OrthoCameraController::Impl::OnWindowResized, pimp.get(), std::placeholders::_1));
  }

  const OrthoCamera& OrthoCameraController::GetCamera() const
  {
    return pimp->camera;
  }

  float OrthoCameraController::GetZoomLevel()
  {
    return pimp->zoom_level;
  }

  void OrthoCameraController::SetZoomLevel(float level)
  {
    pimp->zoom_level = level;
  }

  bool OrthoCameraController::Impl::OnMouseScrolled(MouseScrolledEvent& e)
  {

    zoom_level -= e.GetYOffset() / 5.0f;
    zoom_level = std::max(zoom_level, MIN_ZOOM_LEVEL);
    zoom_level = std::min(zoom_level, MAX_ZOOM_LEVEL);

    camera.SetProjection(GetLimits());
    return false;
  }

  bool OrthoCameraController::Impl::OnWindowResized(WindowResizeEvent& e)
  {
    aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();
    camera.SetProjection(GetLimits());
    return false;
  }

  CameraLimits OrthoCameraController::Impl::GetLimits() const
  {
    float left = -aspect_ratio * zoom_level;
    float right = aspect_ratio * zoom_level;
    float bottom = -zoom_level;
    float top = zoom_level;
    return { left, right, bottom, top };
  }

  OrthoCamera OrthoCameraController::Impl::BuildCamera() const
  {
    return OrthoCamera{ GetLimits() };
  }

} // namespace Pepper