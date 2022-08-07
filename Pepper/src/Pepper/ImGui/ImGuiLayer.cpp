// clang-format off
#include "PepperPCH.hpp"
// clang-format on
#include "ImGuiLayer.hpp"

#include "Pepper/Application.hpp"
#include "Pepper/Core.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

Pepper::ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

Pepper::ImGuiLayer::~ImGuiLayer() {}

void Pepper::ImGuiLayer::OnAttach()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  Application& app = Application::Get();
  GLFWwindow* glfw_win =
    std::any_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

  ImGui_ImplGlfw_InitForOpenGL(glfw_win, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

void Pepper::ImGuiLayer::OnDetach()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Pepper::ImGuiLayer::Begin() const
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
void Pepper::ImGuiLayer::End() const
{
  ImGuiIO& io = ImGui::GetIO();
  Application& app = Application::Get();
  io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()),
                          static_cast<float>(app.GetWindow().GetHeight()));

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_context);
  }
}

void Pepper::ImGuiLayer::OnImGuiRender()
{
  static bool show_demo = true;
  ImGui::ShowDemoWindow(&show_demo);
}