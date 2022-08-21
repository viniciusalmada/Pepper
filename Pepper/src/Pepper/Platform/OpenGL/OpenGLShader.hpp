#pragma once

#include "Pepper/Renderer/Shader.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Pepper
{
  enum class ShaderType { VERTEX, FRAGMENT };

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::filesystem::path& filepath);
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
    static std::tuple<uint32_t, bool> Compile(const std::string& src, ShaderType type);

    static uint32_t GetGLShaderType(ShaderType type);

    static std::string GetGLShaderName(ShaderType type);

    static std::string ReadFile(const std::filesystem::path& filepath);

    static std::unordered_map<ShaderType, std::string> SplitShaderSrc(const std::string& rawSrc);

    void CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);

    bool CheckIsBound() const;

  private:
    uint32_t renderer_id;
  };
}