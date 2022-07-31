// clang-format off
#include "PepperPCH.hpp"
// clang-format on
#include "ImGuiLayer.hpp"

#include "Pepper/Application.hpp"
#include "Pepper/Core.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui.h>

Pepper::ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

Pepper::ImGuiLayer::~ImGuiLayer() {}

void Pepper::ImGuiLayer::OnAttach()
{
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  // TEMPORARY: should eventually use Hazel key codes
  io.KeyMap[ImGuiKey_Tab] = PP_KEY_TAB.keyCode;
  io.KeyMap[ImGuiKey_LeftArrow] = PP_KEY_LEFT.keyCode;
  io.KeyMap[ImGuiKey_RightArrow] = PP_KEY_RIGHT.keyCode;
  io.KeyMap[ImGuiKey_UpArrow] = PP_KEY_UP.keyCode;
  io.KeyMap[ImGuiKey_DownArrow] = PP_KEY_DOWN.keyCode;
  io.KeyMap[ImGuiKey_PageUp] = PP_KEY_PAGE_UP.keyCode;
  io.KeyMap[ImGuiKey_PageDown] = PP_KEY_PAGE_DOWN.keyCode;
  io.KeyMap[ImGuiKey_Home] = PP_KEY_HOME.keyCode;
  io.KeyMap[ImGuiKey_End] = PP_KEY_END.keyCode;
  io.KeyMap[ImGuiKey_Insert] = PP_KEY_INSERT.keyCode;
  io.KeyMap[ImGuiKey_Delete] = PP_KEY_DELETE.keyCode;
  io.KeyMap[ImGuiKey_Backspace] = PP_KEY_BACKSPACE.keyCode;
  io.KeyMap[ImGuiKey_Space] = PP_KEY_SPACE.keyCode;
  io.KeyMap[ImGuiKey_Enter] = PP_KEY_ENTER.keyCode;
  io.KeyMap[ImGuiKey_Escape] = PP_KEY_ESCAPE.keyCode;
  io.KeyMap[ImGuiKey_A] = PP_KEY_A.keyCode;
  io.KeyMap[ImGuiKey_C] = PP_KEY_C.keyCode;
  io.KeyMap[ImGuiKey_V] = PP_KEY_V.keyCode;
  io.KeyMap[ImGuiKey_X] = PP_KEY_X.keyCode;
  io.KeyMap[ImGuiKey_Y] = PP_KEY_Y.keyCode;
  io.KeyMap[ImGuiKey_Z] = PP_KEY_Z.keyCode;

  ImGui_ImplOpenGL3_Init("#version 410");
}

void Pepper::ImGuiLayer::OnDetach() {}

void Pepper::ImGuiLayer::OnUpdate()
{
  ImGuiIO& io = ImGui::GetIO();
  Application& app = Application::Get();
  io.DisplaySize =
    ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

  float glfw_time = (float)glfwGetTime();
  io.DeltaTime = time > 0.0f ? (glfw_time - time) : (1.0f / 60.0f);
  time = glfw_time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Pepper::ImGuiLayer::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseButtonPressedEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));

  dispatcher.Dispatch<MouseButtonReleasedEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));

  dispatcher.Dispatch<MouseMovedEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));

  dispatcher.Dispatch<MouseScrolledEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

  dispatcher.Dispatch<KeyPressedEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));

  dispatcher.Dispatch<KeyReleaseEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));

  dispatcher.Dispatch<KeyTypedEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

  dispatcher.Dispatch<WindowResizeEvent>(
    PP_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
}

bool Pepper::ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton().button] = true;

  return false;
}

bool Pepper::ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton().button] = false;

  return false;
}

bool Pepper::ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e.GetX(), e.GetY());

  return false;
}

bool Pepper::ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheel += e.GetYOffset();
  io.MouseWheelH += e.GetXOffset();

  return false;
}

bool Pepper::ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode().keyCode] = true;

#define KEY_DOWN(l, r) io.KeysDown[l.keyCode] || io.KeysDown[r.keyCode]
  io.KeyCtrl = KEY_DOWN(PP_KEY_LEFT_CONTROL, PP_KEY_RIGHT_CONTROL);
  io.KeyAlt = KEY_DOWN(PP_KEY_LEFT_ALT, PP_KEY_RIGHT_ALT);
  io.KeyShift = KEY_DOWN(PP_KEY_LEFT_SHIFT, PP_KEY_RIGHT_SHIFT);
  io.KeySuper = KEY_DOWN(PP_KEY_LEFT_SUPER, PP_KEY_RIGHT_SUPER);

  return false;
}

bool Pepper::ImGuiLayer::OnKeyReleasedEvent(KeyReleaseEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode().keyCode] = false;

  return false;
}

bool Pepper::ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  int c = e.GetKeyCode().keyCode;
  if (c > 0 && c < 0x10000)
    io.AddInputCharacter((unsigned short)c);

  return false;
}

bool Pepper::ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  glViewport(0, 0, e.GetWidth(), e.GetHeight());

  return false;
}