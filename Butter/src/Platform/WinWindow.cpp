#include "WinWindow.hpp"

#include "Butter/Events/KeyEvent.hpp"
#include "Butter/Events/MouseEvent.hpp"
#include "Butter/Events/WindowEvent.hpp"
#include "ButterPCH.hpp"

static bool s_glfw_initialized = false;

static void GLFWErrorCallback(int error, const char* desc)
{
  BT_CORE_ERROR("GLFW Error: ({0}: {1})", error, desc);
}

Butter::Window* Butter::Window::Create(const WindowProps& props)
{
  return new WinWindow(props);
}

Butter::WinWindow::WinWindow(const WindowProps& props) { Init(props); }

Butter::WinWindow::~WinWindow() { Shutdown(); }

void Butter::WinWindow::Init(const WindowProps& props)
{
  data.title = props.title;
  data.width = props.width;
  data.height = props.height;

  BT_CORE_INFO("Creating window {0} ({1}, {2}",
               props.title,
               props.width,
               props.height);

  if (!s_glfw_initialized)
  {
    // TODO: glfwTerminate on system shutdown
    int success = glfwInit();
    BT_ASSERT(success, "Could not initialize GLFW!");
    glfwSetErrorCallback(GLFWErrorCallback);

    s_glfw_initialized = true;
  }

  window = glfwCreateWindow(props.width,
                            props.height,
                            props.title.c_str(),
                            nullptr,
                            nullptr);
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, &data);
  SetVsync(true);

  ConfigResizeCB();
  ConfigCloseCB();
  ConfigKeyCB();
  ConfigMouseButtonCB();
  ConfigMouseScrollCB();
  ConfigMouseMoveCB();
}

void Butter::WinWindow::Shutdown() { glfwDestroyWindow(window); }

void Butter::WinWindow::ConfigResizeCB() const
{
  auto size_callback = [](GLFWwindow* win, int w, int h)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
    data.width = w;
    data.height = h;

    WindowResizeEvent event{ data.width, data.height };
    data.eventCallback(event);
  };
  glfwSetWindowSizeCallback(window, size_callback);
}

void Butter::WinWindow::ConfigCloseCB() const
{
  auto close_callback = [](GLFWwindow* win)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

    WindowCloseEvent event{};
    data.eventCallback(event);
  };
  glfwSetWindowCloseCallback(window, close_callback);
}

void Butter::WinWindow::ConfigKeyCB() const
{
  auto key_callback = [](GLFWwindow* win, int key, int, int action, int)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

    switch (action)
    {
    case GLFW_PRESS:
    {
      KeyPressedEvent event{ key, 0 };
      data.eventCallback(event);
      break;
    }
    case GLFW_RELEASE:
    {
      KeyReleaseEvent event{ key };
      data.eventCallback(event);
      break;
    }
    case GLFW_REPEAT:
    {
      KeyPressedEvent event{ key, 1 };
      data.eventCallback(event);
      break;
    }
    }
  };
  glfwSetKeyCallback(window, key_callback);
}

void Butter::WinWindow::ConfigMouseButtonCB() const
{
  auto mouse_callback = [](GLFWwindow* win, int bt, int action, int)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

    switch (action)
    {
    case GLFW_PRESS:
    {
      MouseButtonPressedEvent event{ bt };
      data.eventCallback(event);
      break;
    }
    case GLFW_RELEASE:
    {
      MouseButtonReleasedEvent event{ bt };
      data.eventCallback(event);
      break;
    }
    }
  };
  glfwSetMouseButtonCallback(window, mouse_callback);
}

void Butter::WinWindow::ConfigMouseScrollCB() const
{
  auto mouse_callback = [](GLFWwindow* win, double xOff, double yOff)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

    MouseScrolledEvent event{ static_cast<float>(xOff),
                              static_cast<float>(yOff) };
    data.eventCallback(event);
  };
  glfwSetScrollCallback(window, mouse_callback);
}

void Butter::WinWindow::ConfigMouseMoveCB() const
{
  auto mouse_callback = [](GLFWwindow* win, double xPos, double yPos)
  {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

    MouseMovedEvent event{ static_cast<float>(xPos), static_cast<float>(yPos) };
    data.eventCallback(event);
  };
  glfwSetCursorPosCallback(window, mouse_callback);
}

void Butter::WinWindow::OnUpdate()
{
  glfwPollEvents();
  glfwSwapBuffers(window);
}

void Butter::WinWindow::SetVsync(bool enabled)
{
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  data.vsync = enabled;
}

bool Butter::WinWindow::IsVsync() const { return data.vsync; }