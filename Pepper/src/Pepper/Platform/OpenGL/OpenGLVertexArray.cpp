//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

namespace Pepper
{
  static unsigned int ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    default:
      PP_CORE_ASSERT(false, "Unknown ShaderDataType!");
      return 0;
    }
  }

  OpenGLVertexArray::OpenGLVertexArray() : vertex_buffers({}), index_buffer({})
  {
    glCreateVertexArrays(1, &renderer_id);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    glDeleteVertexArrays(1, &renderer_id);
  }

  void OpenGLVertexArray::Bind() const
  {
    glBindVertexArray(renderer_id);
    for (auto& vbo : vertex_buffers)
      vbo->Bind();
    if (index_buffer)
      index_buffer->Bind();
  }

  void OpenGLVertexArray::Unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
  {
    const auto& layout = buffer->GetLayout();
    PP_CORE_ASSERT(!layout.IsEmpty(), "Layout is empty!");

    uint32_t index = 0;
    for (const auto& element : layout)
    {
      glEnableVertexAttribArray(index);
      uint64_t offset = element.offset;
      glVertexAttribPointer(index,
                            element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.type),
                            element.normalized,
                            layout.GetStride(),
                            (void*)offset);
      index++;
    }

    vertex_buffers.insert(buffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
  {
    index_buffer = buffer;
  }

  const std::set<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
  {
    return this->vertex_buffers;
  }

  const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
  {
    return this->index_buffer;
  }
}
