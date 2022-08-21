//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

static unsigned int ShaderDataTypeToOpenGLBaseType(Pepper::ShaderDataType type)
{
  switch (type)
  {
  case Pepper::ShaderDataType::Float:
  case Pepper::ShaderDataType::Float2:
  case Pepper::ShaderDataType::Float3:
  case Pepper::ShaderDataType::Float4:
  case Pepper::ShaderDataType::Mat3:
  case Pepper::ShaderDataType::Mat4:
    return GL_FLOAT;
  case Pepper::ShaderDataType::Int:
  case Pepper::ShaderDataType::Int2:
  case Pepper::ShaderDataType::Int3:
  case Pepper::ShaderDataType::Int4:
    return GL_INT;
  case Pepper::ShaderDataType::Bool:
    return GL_BOOL;
  default:
    PP_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }
}

Pepper::OpenGLVertexArray::OpenGLVertexArray() : vertex_buffers({}), index_buffer({})
{
  glCreateVertexArrays(1, &renderer_id);
}

Pepper::OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &renderer_id); }

void Pepper::OpenGLVertexArray::Bind() const
{
  glBindVertexArray(renderer_id);
  for (auto& vbo : vertex_buffers)
    vbo->Bind();
  if (index_buffer)
    index_buffer->Bind();
}

void Pepper::OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void Pepper::OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
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

void Pepper::OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer) { index_buffer = buffer; }

const std::set<Pepper::Ref<Pepper::VertexBuffer>>& Pepper::OpenGLVertexArray::GetVertexBuffers() const
{
  return this->vertex_buffers;
}

const Pepper::Ref<Pepper::IndexBuffer>& Pepper::OpenGLVertexArray::GetIndexBuffer() const { return this->index_buffer; }