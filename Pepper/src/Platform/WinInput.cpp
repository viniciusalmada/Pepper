// clang-format off
#include "PepperPCH.hpp"
// clang-format on
#include "WinInput.hpp"

#include "Pepper/Application.hpp"

#include <GLFW/glfw3.h>

Pepper::Input* Pepper::Input::instance = new WinInput{};

bool Pepper::WinInput::IsKeyPressedImpl(int keyCode)
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  auto state = glfwGetKey(glfw_window, keyCode);

  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Pepper::WinInput::IsMouseButtonImpl(int button)
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  auto state = glfwGetMouseButton(glfw_window, button);

  return state == GLFW_PRESS;
}

float Pepper::WinInput::GetMouseXImpl()
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  double x{};
  glfwGetCursorPos(glfw_window, &x, nullptr);

  return (float)x;
}

float Pepper::WinInput::GetMouseYImpl()
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  double y{};
  glfwGetCursorPos(glfw_window, nullptr, &y);

  return (float)y;
}