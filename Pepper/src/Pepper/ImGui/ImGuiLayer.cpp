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
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

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
  io.MouseDown[e.GetMouseButton()] = true;

  return false;
}

bool Pepper::ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = false;

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
  io.KeysDown[e.GetKeyCode()] = true;

#define KEY_DOWN(l, r) io.KeysDown[l] || io.KeysDown[r]
  io.KeyCtrl = KEY_DOWN(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL);
  io.KeyAlt = KEY_DOWN(GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT);
  io.KeyShift = KEY_DOWN(GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT);
  io.KeySuper = KEY_DOWN(GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SUPER);

  return false;
}

bool Pepper::ImGuiLayer::OnKeyReleasedEvent(KeyReleaseEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode()] = false;

  return false;
}

bool Pepper::ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
{
  ImGuiIO& io = ImGui::GetIO();
  int c = e.GetKeyCode();
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