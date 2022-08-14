//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLBuffer.hpp"

#include <glad/glad.h>

static bool CheckValidVAO(uint32_t vaoID)
{
  // Check valid parent VAO
  PP_CORE_ASSERT(glIsVertexArray(vaoID), "Invalid Vertex Array Object");

  // Check passed VAO is bound
  uint32_t current_VAO = 0;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (int*)(&current_VAO));

  PP_CORE_ASSERT(current_VAO != 0, "No Vertex Array Object is bound!");

  PP_CORE_ASSERT(current_VAO == vaoID, "The passed VAO is not bound!");

  return true;
}

/*****************
 * Vertex Buffer *
 *****************/

Pepper::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, uint32_t parent)
    : renderer_id(0), parent_id(parent)
{
  if (!CheckValidVAO(parent))
    return;

  glCreateBuffers(1, &renderer_id);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Pepper::OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &renderer_id); }

void Pepper::OpenGLVertexBuffer::Bind() const
{
  if (!CheckValidVAO(parent_id))
    return;

  glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void Pepper::OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

const Pepper::BufferLayout& Pepper::OpenGLVertexBuffer::GetLayout() const { return this->layout; }

void Pepper::OpenGLVertexBuffer::SetLayout(const Pepper::BufferLayout& layout) { this->layout = layout; }

/****************
 * Index Buffer *
 ****************/

Pepper::OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, uint32_t parent)
    : count(count), parent_id(parent)
{
  if (!CheckValidVAO(parent_id))
    return;

  glCreateBuffers(1, &renderer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

Pepper::OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &renderer_id); }

void Pepper::OpenGLIndexBuffer::Bind() const
{
  if (!CheckValidVAO(parent_id))
    return;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}

void Pepper::OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }