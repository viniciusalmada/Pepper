// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer2D.hpp"

#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/VertexArray.hpp"

// TODO: Remove
#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"

namespace Pepper
{
  struct RendererData
  {
    Ref<VertexArray> quad_vertex_array;
    Ref<Shader> flat_color_shader;
  };

  static Scope<RendererData> data;

  void Renderer2D::Init()
  {
    data = CreateScope<RendererData>();
    data->quad_vertex_array = VertexArray::Create();
    data->quad_vertex_array->Bind();

    // clang-format off
    std::vector<float> square_vertices = { 
      -0.5f, -0.5f, 0.0f, 
      +0.5f, -0.5f, 0.0f, 
      +0.5f, +0.5f, 0.0f, 
      -0.5f, +0.5f, 0.0f 
    };
    // clang-format on
    Ref<VertexBuffer> square_vb = VertexBuffer::Create(square_vertices, data->quad_vertex_array->GetRendererID());
    square_vb->SetLayout({ { ShaderDataType::Float3, "in_position" } });
    data->quad_vertex_array->AddVertexBuffer(square_vb);

    std::vector<uint32_t> square_indices = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> square_ib = IndexBuffer::Create(square_indices, data->quad_vertex_array->GetRendererID());
    data->quad_vertex_array->SetIndexBuffer(square_ib);

    data->flat_color_shader = Shader::Create(R"(assets/shaders/FlatColor.glsl)");
  }

  void Renderer2D::Shutdown()
  {
    data.reset();
  }

  void Renderer2D::BeginScene(const OrthoCamera& camera)
  {
    Ref<OpenGLShader> gl_shader = std::dynamic_pointer_cast<OpenGLShader>(data->flat_color_shader);
    gl_shader->Bind();
    gl_shader->UploadUniformMat4("u_view_projection", camera.GetViewProjectionMatrix());
    gl_shader->UploadUniformMat4("u_transform", glm::mat4(1.0f));
  }

  void Renderer2D::EndScene() {}

  void Renderer2D::DrawQuad(const glm::vec2& /* position */, const glm::vec2&/*  size */, const glm::vec4& color)
  {
    Ref<OpenGLShader> gl_shader = std::dynamic_pointer_cast<OpenGLShader>(data->flat_color_shader);
    gl_shader->Bind();
    gl_shader->UploadUniformFloat4("u_color", color);

    data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(data->quad_vertex_array);
  }

}