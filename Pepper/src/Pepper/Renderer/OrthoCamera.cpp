// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "OrthoCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Pepper
{
  OrthoCamera::OrthoCamera(const CameraLimits&& lim)
      : proj_matrix(glm::ortho(lim.left, lim.right, lim.bottom, lim.top, - 1.0f, 1.0f)), view_matrix(1.0f),
        position({ 0.0f, 0.0f, 0.0f }), rotation_deg(0.0f)
  {
    view_proj_matrix = proj_matrix * view_matrix;
  }

  void OrthoCamera::SetProjection(const CameraLimits&& lim)
  {
    proj_matrix = glm::ortho(lim.left, lim.right, lim.bottom, lim.top,-1.0f, 1.0f);
    view_proj_matrix = proj_matrix * view_matrix;
  }

  const glm::vec3& OrthoCamera::GetPosition() const { return position; }

  void OrthoCamera::SetPosition(const glm::vec3& newPosition)
  {
    this->position = newPosition;
    RecalculateViewMatrix();
  }

  float OrthoCamera::GetRotation() const { return rotation_deg; }

  void OrthoCamera::SetRotationDeg(float rotation)
  {
    this->rotation_deg = rotation;
    RecalculateViewMatrix();
  }

  const glm::mat4& OrthoCamera::GetProjectionMatrix() const { return proj_matrix; }

  const glm::mat4& OrthoCamera::GetViewMatrix() const { return view_matrix; }

  const glm::mat4& OrthoCamera::GetViewProjectionMatrix() const { return view_proj_matrix; }

  void OrthoCamera::RecalculateViewMatrix()
  {
    glm::mat4 pos_translated = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_deg), glm::vec3(0, 0, 1));
    glm::mat4 transform = pos_translated * rotation;

    view_matrix = glm::inverse(transform);
    view_proj_matrix = proj_matrix * view_matrix;
  }
}