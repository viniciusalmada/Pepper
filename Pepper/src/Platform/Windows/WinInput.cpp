// clang-format off
#include "PepperPCH.hpp"
// clang-format on
#include "WinInput.hpp"

#include "Pepper/Core/Application.hpp"

#include <GLFW/glfw3.h>

Pepper::Input* Pepper::Input::instance = new WinInput{};

bool Pepper::WinInput::IsKeyPressedImpl(PPKey keyCode)
{
  GLFWwindow* glfw_window = GetGLFWWindow();

  int state = glfwGetKey(glfw_window, PPKey::ToGLFWKey(keyCode));

  return state == GLFW_PRESS;
}

bool Pepper::WinInput::IsMouseButtonImpl(PPMouseBt button)
{
  GLFWwindow* glfw_window = GetGLFWWindow();

  int state = glfwGetMouseButton(glfw_window, PPMouseBt::ToGLFWMouse(button));

  return state == GLFW_PRESS;
}

std::pair<float, float> Pepper::WinInput::GetMouseXYImpl()
{
  GLFWwindow* glfw_window = GetGLFWWindow();
  double x{}, y{};
  glfwGetCursorPos(glfw_window, &x, &y);

  return { static_cast<float>(x), static_cast<float>(y) };
}

GLFWwindow* Pepper::WinInput::GetGLFWWindow()
{
  Window& window = Application::Get().GetWindow();
  GLFWwindow* glfw_window = std::any_cast<GLFWwindow*>(window.GetNativeWindow());
  return glfw_window;
}