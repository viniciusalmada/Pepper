//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLBuffer.hpp"

#include <glad/glad.h>

/*****************
 * Vertex Buffer *
 *****************/

Pepper::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    : renderer_id(0)
{
  glCreateBuffers(1, &renderer_id);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Pepper::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
  glDeleteBuffers(1, &renderer_id);
}

void Pepper::OpenGLVertexBuffer::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void Pepper::OpenGLVertexBuffer::Unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const Pepper::BufferLayout& Pepper::OpenGLVertexBuffer::GetLayout() const
{
  return this->layout;
}

void Pepper::OpenGLVertexBuffer::SetLayout(const Pepper::BufferLayout& layout)
{
  this->layout = layout;
}

/****************
 * Index Buffer *
 ****************/

Pepper::OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count(count)
{
  glCreateBuffers(1, &renderer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               count * sizeof(uint32_t),
               indices,
               GL_STATIC_DRAW);
}

Pepper::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
  glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &renderer_id);
}

void Pepper::OpenGLIndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}

void Pepper::OpenGLIndexBuffer::Unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}