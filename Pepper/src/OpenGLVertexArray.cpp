//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "Pepper/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Pepper/Renderer/BufferElement.hpp"

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

  class OpenGLVertexArray::Impl
  {
  public:
    uint32_t renderer_id;
    uint32_t vertex_attrib_index = 0;
    std::set<Ref<VertexBuffer>> vertex_buffers;
    Ref<IndexBuffer> index_buffer;
  };

  OpenGLVertexArray::OpenGLVertexArray() : pimp(new Impl{})
  {
    glCreateVertexArrays(1, &pimp->renderer_id);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    glDeleteVertexArrays(1, &pimp->renderer_id);
  }

  void OpenGLVertexArray::Bind() const
  {
    glBindVertexArray(pimp->renderer_id);
    for (auto& vbo : pimp->vertex_buffers)
      vbo->Bind();
    if (pimp->index_buffer)
      pimp->index_buffer->Bind();
  }

  void OpenGLVertexArray::Unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
  {
    const auto& layout = buffer->GetLayout();
    PP_CORE_ASSERT(!layout.IsEmpty(), "Layout is empty!");

    for (const auto& element : layout)
    {
      glEnableVertexAttribArray(pimp->vertex_attrib_index);
      uint64_t offset = element.offset;
      glVertexAttribPointer(pimp->vertex_attrib_index,
                            element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.type),
                            element.normalized,
                            layout.GetStride(),
                            (void*)offset);
      pimp->vertex_attrib_index++;
    }

    pimp->vertex_buffers.insert(buffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
  {
    pimp->index_buffer = buffer;
  }

  const std::set<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
  {
    return this->pimp->vertex_buffers;
  }

  const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
  {
    return this->pimp->index_buffer;
  }

  uint32_t OpenGLVertexArray::GetRendererID() const
  {
    return pimp->renderer_id;
  };
}
