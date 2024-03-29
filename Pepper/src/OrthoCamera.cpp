// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/OrthoCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Pepper
{
  class OrthoCamera::Impl
  {
  public:
    Impl(const CameraLimits& lim);

    void RecalculateViewMatrix();

    glm::mat4 proj_matrix;
    glm::mat4 view_matrix;
    glm::mat4 view_proj_matrix;
    glm::vec3 position;
    float rotation_deg;
  };

  OrthoCamera::Impl::Impl(const CameraLimits& lim) :
      proj_matrix(glm::ortho(lim.left, lim.right, lim.bottom, lim.top, -1.0f, 1.0f)),
      view_matrix(1.0f),
      position({ 0.0f, 0.0f, 0.0f }),
      rotation_deg(0.0f)
  {
    PP_PROFILE_FUNCTION();
    view_proj_matrix = proj_matrix * view_matrix;
  }

  OrthoCamera::OrthoCamera(const CameraLimits& lim) : pimp(CreateScope<Impl>(lim)) {}

  OrthoCamera::~OrthoCamera() = default;

  void OrthoCamera::SetProjection(const CameraLimits&& lim)
  {
    PP_PROFILE_FUNCTION();
    pimp->proj_matrix = glm::ortho(lim.left, lim.right, lim.bottom, lim.top, -1.0f, 1.0f);
    pimp->view_proj_matrix = pimp->proj_matrix * pimp->view_matrix;
  }

  const glm::vec3& OrthoCamera::GetPosition() const
  {
    return pimp->position;
  }

  void OrthoCamera::SetPosition(const glm::vec3& newPosition)
  {
    this->pimp->position = newPosition;
    pimp->RecalculateViewMatrix();
  }

  float OrthoCamera::GetRotation() const
  {
    return pimp->rotation_deg;
  }

  void OrthoCamera::SetRotationDeg(float rotation)
  {
    this->pimp->rotation_deg = rotation;
    pimp->RecalculateViewMatrix();
  }

  const glm::mat4& OrthoCamera::GetProjectionMatrix() const
  {
    return pimp->proj_matrix;
  }

  const glm::mat4& OrthoCamera::GetViewMatrix() const
  {
    return pimp->view_matrix;
  }

  const glm::mat4& OrthoCamera::GetViewProjectionMatrix() const
  {
    return pimp->view_proj_matrix;
  }

  void OrthoCamera::Impl::RecalculateViewMatrix()
  {
    PP_PROFILE_FUNCTION();
    glm::mat4 pos_translated = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotation =
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation_deg), glm::vec3(0, 0, 1));
    glm::mat4 transform = pos_translated * rotation;

    view_matrix = glm::inverse(transform);
    view_proj_matrix = proj_matrix * view_matrix;
  }
}