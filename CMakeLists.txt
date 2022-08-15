cmake_minimum_required(VERSION 3.15.0)
project(PepperEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# #############################################################################
# Global constants
set(VENDOR_DIR Pepper/vendor)
set(SOURCE_DIR Pepper/src)

# #############################################################################
# GLFW library
add_subdirectory(${VENDOR_DIR}/GLFW)

if(WIN32)
  set_property(TARGET glfw PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

# #############################################################################
# GLAD library
add_subdirectory(${VENDOR_DIR}/GLAD)

# #############################################################################
# Dear ImGui library
add_library(ImGui STATIC ${VENDOR_DIR}/imgui/imgui_demo.cpp
  ${VENDOR_DIR}/imgui/imgui_draw.cpp
  ${VENDOR_DIR}/imgui/imgui_tables.cpp
  ${VENDOR_DIR}/imgui/imgui_widgets.cpp
  ${VENDOR_DIR}/imgui/imgui.cpp
  ${VENDOR_DIR}/imgui/backends/imgui_impl_opengl3.cpp
  ${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

if(WIN32)
  set_property(TARGET ImGui PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(ImGui PRIVATE ${VENDOR_DIR}/imgui)
target_include_directories(ImGui PRIVATE ${VENDOR_DIR}/GLFW/include)

# #############################################################################
# The engine itself is a static library
add_library(Pepper STATIC
  ${SOURCE_DIR}/Pepper/Application.cpp
  ${SOURCE_DIR}/Pepper/Events/Event.cpp
  ${SOURCE_DIR}/Pepper/ImGui/ImGuiLayer.cpp
  ${SOURCE_DIR}/Pepper/Input.cpp
  ${SOURCE_DIR}/Pepper/Layer.cpp
  ${SOURCE_DIR}/Pepper/LayerStack.cpp
  ${SOURCE_DIR}/Pepper/Log.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLContext.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLRendererAPI.cpp
  ${SOURCE_DIR}/Platform/OpenGL/OpenGLVertexArray.cpp
  ${SOURCE_DIR}/Platform/Windows/WinInput.cpp
  ${SOURCE_DIR}/Platform/Windows/WinWindow.cpp
  ${SOURCE_DIR}/Renderer/Buffer.cpp
  ${SOURCE_DIR}/Renderer/RenderCommand.cpp
  ${SOURCE_DIR}/Renderer/Renderer.cpp
  ${SOURCE_DIR}/Renderer/RendererAPI.cpp
  ${SOURCE_DIR}/Renderer/Shader.cpp
  ${SOURCE_DIR}/Renderer/VertexArray.cpp
)

if(WIN32)
  target_compile_options(Pepper PRIVATE /W4 $<$<CONFIG:Debug>:/WX>)
elseif(UNIX)
  target_compile_options(Pepper PRIVATE -Wall -Wextra $<$<CONFIG:Debug>:-Werror>)
endif(WIN32)

if(WIN32)
  set_property(TARGET Pepper PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(Pepper PRIVATE ${SOURCE_DIR})
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/spdlog/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/GLFW/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/GLAD/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/imgui)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/glm/)

target_compile_definitions(Pepper PRIVATE GLFW_INCLUDE_NONE) # not include gl

if(WIN32)
  target_compile_definitions(Pepper PRIVATE PP_PLATFORM_WINDOWS)
endif(WIN32)

# Configurations flags
target_compile_definitions(Pepper PRIVATE $<IF:$<CONFIG:Debug>,PP_DEBUG,PP_RELEASE>)

# Debug defines
target_compile_definitions(Pepper PRIVATE $<$<CONFIG:Debug>:PP_ENABLE_ASSERTS>)

target_link_libraries(Pepper
  PRIVATE GLAD
  PRIVATE glfw
  PRIVATE ImGui
)

if(WIN32)
  target_link_libraries(Pepper PRIVATE opengl32.lib)
elseif(UNIX)
  target_link_libraries(Pepper PRIVATE GL)
endif(WIN32)

target_precompile_headers(Pepper PRIVATE ${SOURCE_DIR}/PepperPCH.hpp)

# #############################################################################
# The client application
add_executable(App
  App/src/ClientApp.cpp
)

target_include_directories(App PRIVATE App/src)
target_include_directories(App PRIVATE ${SOURCE_DIR})
target_include_directories(App PRIVATE ${VENDOR_DIR}/spdlog/include)
target_include_directories(App PRIVATE ${VENDOR_DIR}/glm/)
target_include_directories(App PRIVATE ${VENDOR_DIR}/imgui)

if(WIN32)
  set_property(TARGET App PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

# Configurations flags
target_compile_definitions(App PRIVATE $<IF:$<CONFIG:Debug>,PP_DEBUG,PP_RELEASE>)

if(WIN32)
  target_compile_definitions(App PRIVATE PP_PLATFORM_WINDOWS)
endif(WIN32)

target_link_libraries(App Pepper)

target_precompile_headers(App PRIVATE ${SOURCE_DIR}/PepperPCH.hpp)