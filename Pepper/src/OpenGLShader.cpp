// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"

#include "Pepper/Core/Utils.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pepper
{
  class OpenGLShader::Impl
  {
  public:
    Impl(const std::filesystem::path& filepath);
    Impl(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

    static std::tuple<uint32_t, bool> Compile(const std::string& src, ShaderType type);

    static uint32_t GetGLShaderType(ShaderType type);
    static std::string GetGLShaderName(ShaderType type);

    static std::string ReadFile(const std::filesystem::path& filepath);
    static std::unordered_map<ShaderType, std::string> SplitShaderSrc(const std::string& rawSrc);

    bool AssertShaderIsBound() const;

    void CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
    bool CheckIsBound() const;

    uint32_t RetrieveUniformLocation(const std::string& name);

    void UploadUniformBool(const std::string& name, bool value);

    void UploadUniformInt(const std::string& name, const int& value);

    void UploadUniformFloat(const std::string& name, const float& value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    std::unordered_map<std::string, uint32_t> uniform_locations;
    uint32_t renderer_id;
    std::string name;
  };

  OpenGLShader::Impl::Impl(const std::filesystem::path& filepath) : name(filepath.stem().string())
  {
    std::string shader_raw_src = std::move(ReadFile(filepath));
    auto shaders_src = SplitShaderSrc(shader_raw_src);
    CreateProgram(shaders_src[ShaderType::VERTEX], shaders_src[ShaderType::FRAGMENT]);
  }

  OpenGLShader::Impl::Impl(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
      name(name)
  {
    CreateProgram(vertexSrc, fragmentSrc);
  }

  OpenGLShader::OpenGLShader(const std::filesystem::path& filepath) : pimp(CreateScope<Impl>(filepath)) {}

  OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
      pimp(CreateScope<Impl>(name, vertexSrc, fragmentSrc))
  {
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(pimp->renderer_id);
  }

  void OpenGLShader::Bind() const
  {
    glUseProgram(pimp->renderer_id);
  }

  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

  const std::string& OpenGLShader::GetName() const
  {
    return this->pimp->name;
  }

  void OpenGLShader::SetBoolean(const std::string& name, bool value)
  {
    this->pimp->UploadUniformBool(name, value);
  }

  void OpenGLShader::SetInt(const std::string& name, const int& number)
  {
    this->pimp->UploadUniformInt(name, number);
  }

  void OpenGLShader::SetMat4(const std::string& uniformName, const glm::mat4& matrix)
  {
    this->pimp->UploadUniformMat4(uniformName, matrix);
  }

  void OpenGLShader::SetFloat4(const std::string& uniformName, const glm::vec4& vec)
  {
    this->pimp->UploadUniformFloat4(uniformName, vec);
  }

  void OpenGLShader::Impl::UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::Impl::UploadUniformMat3(const std::string& uniformName, const glm::mat3& matrix)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::Impl::UploadUniformFloat4(const std::string& uniformName, const glm::vec4& vec)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
  }

  void OpenGLShader::Impl::UploadUniformFloat3(const std::string& uniformName, const glm::vec3& vec)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform3f(location, vec.x, vec.y, vec.z);
  }

  void OpenGLShader::Impl::UploadUniformFloat2(const std::string& uniformName, const glm::vec2& value)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform2f(location, value.x, value.y);
  }

  void OpenGLShader::Impl::UploadUniformFloat(const std::string& uniformName, const float& value)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform1f(location, value);
  }

  void OpenGLShader::Impl::UploadUniformInt(const std::string& uniformName, const int& value)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform1i(location, value);
  }

  void OpenGLShader::Impl::UploadUniformBool(const std::string& uniformName, bool value)
  {
    if (!AssertShaderIsBound())
      return;

    int location = RetrieveUniformLocation(uniformName);
    glUniform1i(location, value);
  }

  std::tuple<uint32_t, bool> OpenGLShader::Impl::Compile(const std::string& src, ShaderType type)
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

  uint32_t OpenGLShader::Impl::GetGLShaderType(ShaderType type)
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

  std::string OpenGLShader::Impl::GetGLShaderName(ShaderType type)
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

  std::string OpenGLShader::Impl::ReadFile(const std::filesystem::path& filepath)
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

  std::unordered_map<ShaderType, std::string> OpenGLShader::Impl::SplitShaderSrc(const std::string& rawSrc)
  {
    std::unordered_map<std::string, std::string> shaders_src;
    const std::string TOKEN{ "//##" };
    const std::string NEW_LINE = Utils::GetNewLine();

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

  bool OpenGLShader::Impl::AssertShaderIsBound() const
  {
    bool is_bound = CheckIsBound();
    PP_CORE_ASSERT(is_bound, "The shader is not bound to upload this uniform!");
    return is_bound;
  }

  void OpenGLShader::Impl::CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
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

  bool OpenGLShader::Impl::CheckIsBound() const
  {
    uint32_t curr_program = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, (int*)&curr_program);
    if (curr_program == 0)
      return false;

    return curr_program == renderer_id;
  }

  uint32_t OpenGLShader::Impl::RetrieveUniformLocation(const std::string& uniformName)
  {
    if (uniform_locations.contains(uniformName))
      return uniform_locations.at(uniformName);

    int location = glGetUniformLocation(renderer_id, uniformName.c_str());
    PP_CORE_ASSERT(location != -1, "Invalid uniform name to this shader!");

    uniform_locations[uniformName] = location;
    return location;
  }

}
