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

  OpenGLContext::OpenGLContext(Window& windowsHandle) : pimp(CreateScope<Impl>(windowsHandle)) {}

  OpenGLContext::~OpenGLContext() = default;

  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent(pimp->native_window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PP_CORE_ASSERT(status, "Failed to initialize Glad!");

    PP_CORE_INFO("OpenGL Vendor: {0}", (char*)glGetString(GL_VENDOR));
    PP_CORE_INFO("OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER));
    PP_CORE_INFO("OpenGL Version: {0}", (char*)glGetString(GL_VERSION));

#ifdef PP_ENABLE_ASSERTS
    int versionMajor;
    int versionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    PP_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                   "Pepper requires at least OpenGL version 4.5!");
#endif
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(pimp->native_window);
  }
}