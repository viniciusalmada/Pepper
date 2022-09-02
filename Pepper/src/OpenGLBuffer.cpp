//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "Pepper/Platform/OpenGL/OpenGLBuffer.hpp"

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

namespace Pepper
{
  /*****************
   * Vertex Buffer *
   *****************/

  class OpenGLVertexBuffer::Impl
  {
  public:
    Impl(const std::vector<float>& vertices, uint32_t parent);

    uint32_t renderer_id;
    uint32_t parent_id;
    BufferLayout layout;
  };

  OpenGLVertexBuffer::Impl::Impl(const std::vector<float>& vertices, uint32_t parent) :
      renderer_id(0),
      parent_id(parent),
      layout({})

  {
    if (!CheckValidVAO(parent))
      return;

    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
    uint32_t size = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vertices, uint32_t parent) :
      pimp(CreateScope<Impl>(vertices, parent))
  {
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    glDeleteBuffers(1, &pimp->renderer_id);
  }

  void OpenGLVertexBuffer::Bind() const
  {
    if (!CheckValidVAO(pimp->parent_id))
      return;

    glBindBuffer(GL_ARRAY_BUFFER, pimp->renderer_id);
  }

  void OpenGLVertexBuffer::Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  const BufferLayout& OpenGLVertexBuffer::GetLayout() const
  {
    return this->pimp->layout;
  }

  void OpenGLVertexBuffer::SetLayout(const BufferLayout& newLayout)
  {
    this->pimp->layout = newLayout;
  }

  /****************
   * Index Buffer *
   ****************/

  class OpenGLIndexBuffer::Impl
  {
  public:
    Impl(const std::vector<uint32_t>& indices, uint32_t parent);

    uint32_t renderer_id;
    uint32_t count;
    uint32_t parent_id;
  };

  OpenGLIndexBuffer::Impl::Impl(const std::vector<uint32_t>& indices, uint32_t parent) :
      count(indices.size()),
      parent_id(parent)
  {
    if (!CheckValidVAO(parent_id))
      return;

    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& indices, uint32_t parent) :
      pimp(CreateScope<Impl>(indices, parent))
  {
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &pimp->renderer_id);
  }

  void OpenGLIndexBuffer::Bind() const
  {
    if (!CheckValidVAO(pimp->parent_id))
      return;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pimp->renderer_id);
  }

  void OpenGLIndexBuffer::Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLIndexBuffer::GetCount() const
  {
    return pimp->count;
  }
}