// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Platform/OpenGL/OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pepper
{
  class OpenGLContext::Impl
  {
  public:
    Impl(Window& windowsHandle);

    GLFWwindow* window_handle;
  };

  OpenGLContext::Impl::Impl(Window& windowsHandle) :
      window_handle(std::any_cast<GLFWwindow*>(windowsHandle.GetNativeWindow()))
  {
    PP_CORE_ASSERT(window_handle, "Window handle is null!");
  }

  OpenGLContext::OpenGLContext(Window& windowsHandle) : pimp(new Impl{ windowsHandle }) {}

  OpenGLContext::~OpenGLContext() = default;

  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent(pimp->window_handle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PP_CORE_ASSERT(status, "Failed to initialize Glad!");

    PP_CORE_INFO("OpenGL Vendor: {0}", (char*)glGetString(GL_VENDOR));
    PP_CORE_INFO("OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER));
    PP_CORE_INFO("OpenGL Version: {0}", (char*)glGetString(GL_VERSION));
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(pimp->window_handle);
  }
}