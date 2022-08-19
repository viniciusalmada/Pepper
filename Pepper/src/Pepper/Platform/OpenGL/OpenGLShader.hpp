#pragma once

#include "Pepper/Renderer/Shader.hpp"

#include <glm/glm.hpp>

namespace Pepper
{
  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader();

    void Bind() const override;

    void Unbind() const override;

    void UploadUniformInt(const std::string& name, const int& value);

    void UploadUniformFloat(const std::string& name, const float& value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

  private:
    bool CheckIsBound() const;

    uint32_t renderer_id;
  };
}