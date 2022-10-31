#pragma once

#include "Pepper/Core/Core.hpp"

#include <cstdint>

namespace Pepper
{
  enum class ShaderDataType {
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
  };

  struct BufferElement
  {
    std::string name;
    ShaderDataType type;
    uint32_t size;   // Number of bytes of type
    uint32_t offset; //
    bool normalized;

    BufferElement() {}

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

    [[nodiscard]] uint32_t GetComponentCount() const;
  };
}