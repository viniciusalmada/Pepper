// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Pepper::OpenGLContext::OpenGLContext(GLFWwindow* windowsHandle)
    : window_handle(windowsHandle)
{
  PP_CORE_ASSERT(windowsHandle, "Window handle is null!")
}

void Pepper::OpenGLContext::Init()
{
  glfwMakeContextCurrent(window_handle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  PP_CORE_ASSERT(status, "Failed to initialize Glad!");

  PP_CORE_INFO("OpenGL Vendor: {0}", (char*)glGetString(GL_VENDOR));
  PP_CORE_INFO("OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER));
  PP_CORE_INFO("OpenGL Version: {0}", (char*)glGetString(GL_VERSION));
}

void Pepper::OpenGLContext::SwapBuffers() { glfwSwapBuffers(window_handle); }