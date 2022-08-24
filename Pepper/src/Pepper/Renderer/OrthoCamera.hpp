#pragma once

#include <glm/glm.hpp>

namespace Pepper
{
  class OrthoCamera
  {
  public:
    OrthoCamera(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const;
    void SetPosition(const glm::vec3& position);

    float GetRotation() const;
    void SetRotationDeg(float rotation);

    const glm::mat4& GetProjectionMatrix() const;
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetViewProjectionMatrix() const;

  private:
    void RecalculateViewMatrix();

  private:
    glm::mat4 proj_matrix;
    glm::mat4 view_matrix;
    glm::mat4 view_proj_matrix;

    glm::vec3 position;
    float rotation_deg;
  };
}