#include "WinWindow.hpp"

#include "ButterPCH.hpp"

static bool s_glfw_initialized = false;

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
}

void Butter::WinWindow::Shutdown() { glfwDestroyWindow(window); }

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