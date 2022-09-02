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

    GLFWwindow* native_window;
  };

  OpenGLContext::Impl::Impl(Window& windowsHandle) :
      native_window(std::any_cast<GLFWwindow*>(windowsHandle.GetNativeWindow()))
  {
    PP_CORE_ASSERT(native_window, "Window handle is null!");
  }

  OpenGLContext::OpenGLContext(Window& windowsHandle) : pimp(new Impl{ windowsHandle }) {}

  OpenGLContext::~OpenGLContext() = default;

  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent(pimp->native_window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PP_CORE_ASSERT(status, "Failed to initialize Glad!");

    PP_CORE_INFO("OpenGL Vendor: {0}", (char*)glGetString(GL_VENDOR));
    PP_CORE_INFO("OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER));
    PP_CORE_INFO("OpenGL Version: {0}", (char*)glGetString(GL_VERSION));
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(pimp->native_window);
  }
}