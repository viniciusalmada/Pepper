// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "OpenGLShader.hpp"

#include "Pepper/Core/Utils.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Pepper::OpenGLShader::OpenGLShader(const std::filesystem::path& filepath)
{
  std::string shader_raw_src = std::move(ReadFile(filepath));
  auto shaders_src = SplitShaderSrc(shader_raw_src);
  CreateProgram(shaders_src[ShaderType::VERTEX], shaders_src[ShaderType::FRAGMENT]);
}

Pepper::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
  CreateProgram(vertexSrc, fragmentSrc);
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

void Pepper::OpenGLShader::UploadUniformInt(const std::string& name, const int& value)
{
  bool is_bound = CheckIsBound();
  PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
  if (!is_bound)
    return;

  int location = glGetUniformLocation(renderer_id, name.c_str());
  glUniform1i(location, value);
}

std::tuple<uint32_t, bool> Pepper::OpenGLShader::Compile(const std::string& src, Pepper::ShaderType type)
{
  uint32_t shader_type = GetGLShaderType(type);

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

    std::string shader_name = GetGLShaderName(type);
    PP_CORE_ERROR("Failure at compiling the {0} shader", shader_name);
    PP_CORE_ERROR("{0}", info_log.data());

    glDeleteShader(shader);
  }
  return std::tuple<unsigned int, bool>{ shader, success == 1 };
}

uint32_t Pepper::OpenGLShader::GetGLShaderType(Pepper::ShaderType type)
{
  switch (type)
  {
  case ShaderType::VERTEX:
    return GL_VERTEX_SHADER;
  case ShaderType::FRAGMENT:
    return GL_FRAGMENT_SHADER;
  }
  PP_CORE_ASSERT(false, "Unreachable code!");
  return 0;
}

std::string Pepper::OpenGLShader::GetGLShaderName(Pepper::ShaderType type)
{
  switch (type)
  {
  case ShaderType::VERTEX:
    return "Vertex";
  case ShaderType::FRAGMENT:
    return "Fragment";
  }
  PP_CORE_ASSERT(false, "Unreachable code!");
  return {};
}

std::string Pepper::OpenGLShader::ReadFile(const std::filesystem::path& filepath)
{
  std::ifstream in{ filepath, std::ios::in | std::ios::binary };
  PP_CORE_ASSERT(in, "Could not open shader file {0}", filepath);

  std::string shader_raw_src;

  in.seekg(0, std::ios::end);
  shader_raw_src.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(shader_raw_src.data(), shader_raw_src.size());
  in.close();

  PP_CORE_ASSERT(!shader_raw_src.empty(), "Shader raw source is empty!")

  return shader_raw_src;
}

std::unordered_map<Pepper::ShaderType, std::string> Pepper::OpenGLShader::SplitShaderSrc(const std::string& rawSrc)
{
  std::unordered_map<std::string, std::string> shaders_src;
  const std::string TOKEN{ "//##" };
  const std::string NEW_LINE = Pepper::Utils::GetNewLine();

  uint64_t curr_position = 0;
  while (curr_position < rawSrc.size())
  {
    uint64_t first_token_pos = rawSrc.find(TOKEN, curr_position);
    uint64_t second_token_pos = rawSrc.find(TOKEN, first_token_pos + TOKEN.size());
    if (second_token_pos == std::string::npos)
      second_token_pos = rawSrc.size();

    uint64_t token_type_start = first_token_pos + TOKEN.size();
    uint64_t token_type_end = rawSrc.find(NEW_LINE, first_token_pos);
    uint64_t token_type_len = token_type_end - token_type_start;
    std::string token_type = rawSrc.substr(token_type_start, token_type_len);

    uint64_t code_start = token_type_end + NEW_LINE.size();
    uint64_t code_end;
    if (second_token_pos == rawSrc.size()) // last shader code
      code_end = second_token_pos;
    else
      code_end = second_token_pos - NEW_LINE.size();
    uint64_t code_len = code_end - code_start;
    shaders_src[token_type] = rawSrc.substr(code_start, code_len);
    curr_position = second_token_pos;
  }

  std::unordered_map<ShaderType, std::string> shaders_src_by_type;

  for (auto [token_type, code] : shaders_src)
  {
    if (token_type == "vertex")
    {
      shaders_src_by_type[ShaderType::VERTEX] = code;
    }
    else if (token_type == "fragment")
    {
      shaders_src_by_type[ShaderType::FRAGMENT] = code;
    }
  }

  PP_CORE_ASSERT(shaders_src_by_type.size() == shaders_src.size(), "Error on spliting the shaders sources!");
  return shaders_src_by_type;
}

void Pepper::OpenGLShader::CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
{
  auto [vertex_shader, vertex_ok] = Compile(vertexSrc, ShaderType::VERTEX);
  if (!vertex_ok)
    return;

  auto [fragment_shader, fragment_ok] = Compile(fragmentSrc, ShaderType::FRAGMENT);
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

bool Pepper::OpenGLShader::CheckIsBound() const
{
  uint32_t curr_program = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, (int*)&curr_program);
  if (curr_program == 0)
    return false;

  return curr_program == renderer_id;
}