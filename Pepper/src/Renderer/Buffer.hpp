#pragma once

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
    default:
      PP_CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
    }
  }

  struct BufferElement
  {
    std::string name;
    ShaderDataType type;
    uint32_t size;   // Number of bytes of type
    uint32_t offset; //
    bool normalized;

    BufferElement() {}

    BufferElement(ShaderDataType type,
                  const std::string& name,
                  bool normalized = false)
        : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0),
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
      default:
        PP_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
      }
    }
  };

  class BufferLayout
  {
  public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : elements(elements)
    {
      CalculateOffsetAndStride();
    }

    uint32_t GetStride() const { return stride; }

    const std::vector<BufferElement>& GetElements() const { return elements; }

    auto begin() { return elements.begin(); }
    auto end() { return elements.end(); }
    auto begin() const { return elements.cbegin(); }
    auto end() const { return elements.cend(); }

  private:
    void CalculateOffsetAndStride()
    {
      uint32_t offset = 0;
      stride = 0;

      for (auto& element : elements)
      {
        element.offset = offset;
        offset += element.size;
        stride += element.size;
      }
    }

    std::vector<BufferElement> elements;
    uint32_t stride;
  };

  class Buffer
  {
  public:
    virtual ~Buffer() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;
  };

  class VertexBuffer : public Buffer
  {
  public:
    virtual const BufferLayout& GetLayout() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;

    static VertexBuffer* Create(float* vertices, uint32_t size);
  };

  class IndexBuffer : public Buffer
  {
  public:
    virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
  };
}