// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Platform/Windows/WinWindow.hpp"

#include "Pepper/Events/KeyEvent.hpp"
#include "Pepper/Events/MouseEvent.hpp"
#include "Pepper/Events/WindowEvent.hpp"
#include "Pepper/Platform/OpenGL/OpenGLContext.hpp"

namespace Pepper
{
  static bool s_glfw_initialized = false;

  static void GLFWErrorCallback(int error, const char* desc)
  {
    PP_CORE_ERROR("GLFW Error: ({0}: {1})", error, desc);
  }

  Scope<Window> Window::Create(const WindowProps& props)
  {
    return CreateScope<WinWindow>(props);
  }

  class WinWindow::Impl
  {
  public:
    Impl(const WindowProps& props);

    void Init(const WindowProps& props);
    void Shutdown();

    void ConfigResizeCB() const;
    void ConfigCloseCB() const;
    void ConfigKeyCB() const;
    void ConfigMouseButtonCB() const;
    void ConfigMouseScrollCB() const;
    void ConfigMouseMoveCB() const;

    GLFWwindow* window;
    Scope<GraphicsContext> graphics_context;

    struct WindowData : WindowProps
    {
      bool vsync;
      EventCallbackFn eventCallback;
    };
    WindowData data;
  };

  WinWindow::Impl::Impl(const WindowProps& props)
  {
    Init(props);
  }

  WinWindow::WinWindow(const WindowProps& props) : pimp(CreateScope<Impl>(props))
  {
    pimp->graphics_context = CreateScope<OpenGLContext>(*this);
    pimp->graphics_context->Init();

    SetVsync(true);
  }

  WinWindow::~WinWindow()
  {
    pimp->Shutdown();
  }

  void WinWindow::Impl::Init(const WindowProps& props)
  {
    data.title = props.title;
    data.width = props.width;
    data.height = props.height;

    PP_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (!s_glfw_initialized)
    {
      int success = glfwInit();
      PP_CORE_ASSERT(success, "Could not initialize GLFW!");
      if (!success)
        return;
      glfwSetErrorCallback(GLFWErrorCallback);

      s_glfw_initialized = true;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(window, &data);

    ConfigResizeCB();
    ConfigCloseCB();
    ConfigKeyCB();
    ConfigMouseButtonCB();
    ConfigMouseScrollCB();
    ConfigMouseMoveCB();
  }

  void WinWindow::Impl::Shutdown()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void WinWindow::Impl::ConfigResizeCB() const
  {
    auto size_callback = [](GLFWwindow* win, int w, int h)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
      data.width = w;
      data.height = h;

      WindowResizeEvent event{ data.width, data.height };
      PP_CORE_WARN("{0}, {1}", w, h);
      data.eventCallback(event);
    };
    auto iconify_callback = [](GLFWwindow* win, int iconified)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
      if (iconified)
      {
        data.width = 0;
        data.height = 0;
      }
      else
      {
        glfwGetWindowSize(win, (int*)&data.width, (int*)&data.height);
      }
      WindowResizeEvent event{ data.width, data.height };
      PP_CORE_WARN("{0}, {1}", data.width, data.height);
      data.eventCallback(event);
    };
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetWindowIconifyCallback(window, iconify_callback);
  }

  void WinWindow::Impl::ConfigCloseCB() const
  {
    auto close_callback = [](GLFWwindow* win)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      WindowCloseEvent event{};
      data.eventCallback(event);
    };
    glfwSetWindowCloseCallback(window, close_callback);
  }

  void WinWindow::Impl::ConfigKeyCB() const
  {
    auto key_callback = [](GLFWwindow* win, int key, int, int action, int)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      switch (action)
      {
      case GLFW_PRESS:
      {
        KeyPressedEvent event{ PPKey::FromGLFWKey(key), 0 };
        data.eventCallback(event);
        break;
      }
      case GLFW_RELEASE:
      {
        KeyReleaseEvent event{ PPKey::FromGLFWKey(key) };
        data.eventCallback(event);
        break;
      }
      case GLFW_REPEAT:
      {
        KeyPressedEvent event{ PPKey::FromGLFWKey(key), 1 };
        data.eventCallback(event);
        break;
      }
      }
    };
    glfwSetKeyCallback(window, key_callback);

    auto char_callback = [](GLFWwindow* win, unsigned int key)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      KeyTypedEvent event{ PPKey::FromGLFWKey(static_cast<int>(key)) };
      data.eventCallback(event);
    };

    glfwSetCharCallback(window, char_callback);
  }

  void WinWindow::Impl::ConfigMouseButtonCB() const
  {
    auto mouse_callback = [](GLFWwindow* win, int bt, int action, int)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      switch (action)
      {
      case GLFW_PRESS:
      {
        MouseButtonPressedEvent event{ PPMouseBt::FromGLFWMouse(bt) };
        data.eventCallback(event);
        break;
      }
      case GLFW_RELEASE:
      {
        MouseButtonReleasedEvent event{ PPMouseBt::FromGLFWMouse(bt) };
        data.eventCallback(event);
        break;
      }
      }
    };
    glfwSetMouseButtonCallback(window, mouse_callback);
  }

  void WinWindow::Impl::ConfigMouseScrollCB() const
  {
    auto mouse_callback = [](GLFWwindow* win, double xOff, double yOff)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      MouseScrolledEvent event{ static_cast<float>(xOff), static_cast<float>(yOff) };
      data.eventCallback(event);
    };
    glfwSetScrollCallback(window, mouse_callback);
  }

  void WinWindow::Impl::ConfigMouseMoveCB() const
  {
    auto mouse_callback = [](GLFWwindow* win, double xPos, double yPos)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

      MouseMovedEvent event{ static_cast<float>(xPos), static_cast<float>(yPos) };
      data.eventCallback(event);
    };
    glfwSetCursorPosCallback(window, mouse_callback);
  }

  void WinWindow::OnUpdate()
  {
    glfwPollEvents();
    pimp->graphics_context->SwapBuffers();
  }

  unsigned int WinWindow::GetWidth() const
  {
    return pimp->data.width;
  }

  unsigned int WinWindow::GetHeight() const
  {
    return pimp->data.height;
  }

  void WinWindow::SetEventCallback(const EventCallbackFn& callback)
  {
    pimp->data.eventCallback = callback;
  }

  void WinWindow::SetVsync(bool enabled)
  {
    if (enabled)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);

    pimp->data.vsync = enabled;
  }

  bool WinWindow::IsVsync() const
  {
    return pimp->data.vsync;
  }

  std::any WinWindow::GetNativeWindow() const
  {
    return std::make_any<GLFWwindow*>(pimp->window);
  };
}