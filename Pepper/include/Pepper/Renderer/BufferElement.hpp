#pragma once

#include "Pepper/Core/Core.hpp"

#include <cstdint>

namespace Pepper
{
  enum class ShaderDataType { None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

  static constexpr uint32_t ShaderDataTypeSize(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Float2:
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Float3:
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Float4:
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    case ShaderDataType::None:
      PP_CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
    }
    PP_CORE_ASSERT(false, "Unreachable code");
    return 0;
  }

  struct BufferElement
  {
    std::string name;
    ShaderDataType type;
    uint32_t size;   // Number of bytes of type
    uint32_t offset; //
    bool normalized;

    BufferElement() {}

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
        name(name),
        type(type),
        size(ShaderDataTypeSize(type)),
        offset(0),
        normalized(normalized)
    {
    }

    constexpr uint32_t GetComponentCount() const
    {
      switch (type)
      {
      case ShaderDataType::Float:
      case ShaderDataType::Int:
      case ShaderDataType::Bool:
        return 1;
      case ShaderDataType::Float2:
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Float3:
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Float4:
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Mat3:
        return 3 * 3;
      case ShaderDataType::Mat4:
        return 4 * 4;
      case ShaderDataType::None:
        PP_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
      }
      PP_CORE_ASSERT(false, "Unreachable code");
      return 0;
    }
  };
}