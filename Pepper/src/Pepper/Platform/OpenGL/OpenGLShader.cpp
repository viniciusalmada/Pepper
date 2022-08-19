// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "OpenGLShader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Pepper::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
  auto compile_shader = [](const std::string& src, bool isVertex)
  {
    auto shader_type = isVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    unsigned int shader = glCreateShader(shader_type);
    const char* chars = src.c_str();
    glShaderSource(shader, 1, &chars, 0);

    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      int max_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

      std::vector<char> info_log;
      info_log.reserve(max_length);
      glGetShaderInfoLog(shader, max_length, &max_length, info_log.data());

      std::string shader_name = isVertex ? "vertex" : "fragment";
      PP_CORE_ERROR("Failure at compiling the {0} shader", shader_name);
      PP_CORE_ERROR("{0}", info_log.data());

      glDeleteShader(shader);
    }
    return std::tuple<unsigned int, bool>{ shader, success == 1 };
  };

  auto [vertex_shader, vertex_ok] = compile_shader(vertexSrc, true);
  if (!vertex_ok)
    return;

  auto [fragment_shader, fragment_ok] = compile_shader(fragmentSrc, false);
  if (!fragment_ok)
  {
    glDeleteShader(vertex_shader);
    return;
  }

  renderer_id = glCreateProgram();

  glAttachShader(renderer_id, vertex_shader);
  glAttachShader(renderer_id, fragment_shader);

  glLinkProgram(renderer_id);

  int is_linked = 0;
  glGetProgramiv(renderer_id, GL_LINK_STATUS, &is_linked);
  if (!is_linked)
  {
    int max_length = 0;
    glGetProgramiv(renderer_id, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<char> info_log;
    info_log.reserve(max_length);
    glGetProgramInfoLog(renderer_id, max_length, &max_length, info_log.data());

    PP_CORE_ERROR("Failure at linking program!");
    PP_CORE_ERROR("{0}", info_log.data());

    glDeleteProgram(renderer_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return;
  }

  glDetachShader(renderer_id, vertex_shader);
  glDetachShader(renderer_id, fragment_shader);
}

Pepper::OpenGLShader::~OpenGLShader() { glDeleteProgram(renderer_id); }

void Pepper::OpenGLShader::Bind() const { glUseProgram(renderer_id); }

void Pepper::OpenGLShader::Unbind() const { glUseProgram(0); }

void Pepper::OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Pepper::OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Pepper::OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Pepper::OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniform3f(location, vec.x, vec.y, vec.z);
}

void Pepper::OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void Pepper::OpenGLShader::UploadUniformFloat(const std::string& name, const float& value)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniform1f(location, value);
}

bool Pepper::OpenGLShader::CheckIsBound() const
{
  uint32_t curr_program = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, (int*)&curr_program);
  if (curr_program == 0)
    return false;

  return curr_program == renderer_id;
}