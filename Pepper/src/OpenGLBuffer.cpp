//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "Pepper/Platform/OpenGL/OpenGLBuffer.hpp"

#include <glad/glad.h>

static bool CheckValidVAO(uint32_t vaoID)
{
  // Check valid parent VAO
  bool is_valid_vao = glIsVertexArray(vaoID);
  PP_CORE_ASSERT(is_valid_vao, "Invalid Vertex Array Object");
  if (!is_valid_vao)
    return false;

  // Check passed VAO is bound
  uint32_t current_VAO = 0;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (int*)(&current_VAO));

  bool is_any_vao_bound = current_VAO != 0;
  PP_CORE_ASSERT(is_any_vao_bound, "No Vertex Array Object is bound!");
  if (!is_any_vao_bound)
    return false;

  bool is_vao_bound = current_VAO == vaoID;
  PP_CORE_ASSERT(is_vao_bound, "The passed VAO is not bound!");
  if (!is_vao_bound)
    return false;

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
    PP_PROFILE_FUNCTION();
    if (!CheckValidVAO(parent))
      return;

    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
    uint32_t size = (uint32_t)(vertices.size() * sizeof(float));
    glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vertices, uint32_t parent) :
      pimp(CreateScope<Impl>(vertices, parent))
  {
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    PP_PROFILE_FUNCTION();
    glDeleteBuffers(1, &pimp->renderer_id);
  }

  void OpenGLVertexBuffer::Bind() const
  {
    PP_PROFILE_FUNCTION();
    if (!CheckValidVAO(pimp->parent_id))
      return;

    glBindBuffer(GL_ARRAY_BUFFER, pimp->renderer_id);
  }

  void OpenGLVertexBuffer::Unbind() const
  {
    PP_PROFILE_FUNCTION();
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
      count((uint32_t)indices.size()),
      parent_id(parent)
  {
    PP_PROFILE_FUNCTION();
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
    PP_PROFILE_FUNCTION();
    glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &pimp->renderer_id);
  }

  void OpenGLIndexBuffer::Bind() const
  {
    PP_PROFILE_FUNCTION();
    if (!CheckValidVAO(pimp->parent_id))
      return;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pimp->renderer_id);
  }

  void OpenGLIndexBuffer::Unbind() const
  {
    PP_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLIndexBuffer::GetCount() const
  {
    return pimp->count;
  }
}