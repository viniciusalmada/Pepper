cmake_minimum_required(VERSION 3.15.0)
project(PepperEngine VERSION 0.1)

# #############################################################################
# Global constants
set(PP_VENDOR_DIR Pepper/vendor)
set(PP_SRC_DIR Pepper/src)

set(CXX_STANDARD cxx_std_20)

# #############################################################################
# GLFW library
add_subdirectory(${PP_VENDOR_DIR}/GLFW)

# #############################################################################
# glm library
add_subdirectory(${PP_VENDOR_DIR}/glm)

if(WIN32)
  # disable warning C4201: nonstandard extension used: nameless struct/union
  target_compile_options(glm INTERFACE /wd4201)
endif(WIN32)

# #############################################################################
# GLAD library
add_subdirectory(${PP_VENDOR_DIR}/GLAD)

# #############################################################################
# Dear ImGui library
add_library(ImGui STATIC ${PP_VENDOR_DIR}/imgui/imgui_demo.cpp
  ${PP_VENDOR_DIR}/imgui/imgui_draw.cpp
  ${PP_VENDOR_DIR}/imgui/imgui_tables.cpp
  ${PP_VENDOR_DIR}/imgui/imgui_widgets.cpp
  ${PP_VENDOR_DIR}/imgui/imgui.cpp
  ${PP_VENDOR_DIR}/imgui/backends/imgui_impl_opengl3.cpp
  ${PP_VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

target_compile_features(ImGui PRIVATE ${CXX_STANDARD})

if(WIN32)
  set_property(TARGET ImGui PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(ImGui PRIVATE ${PP_VENDOR_DIR}/imgui)
target_include_directories(ImGui PRIVATE ${PP_VENDOR_DIR}/GLFW/include)

# #############################################################################
# The Pepper engine
add_library(Pepper STATIC
  ${PP_SRC_DIR}/Pepper/Core/Application.cpp
  ${PP_SRC_DIR}/Pepper/Core/Log.cpp
  ${PP_SRC_DIR}/Pepper/Core/Utils.cpp
  ${PP_SRC_DIR}/Pepper/Events/Event.cpp
  ${PP_SRC_DIR}/Pepper/Input/Input.cpp
  ${PP_SRC_DIR}/Pepper/Layers/ImGui/ImGuiLayer.cpp
  ${PP_SRC_DIR}/Pepper/Layers/Layer.cpp
  ${PP_SRC_DIR}/Pepper/Layers/LayerStack.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLBuffer.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLContext.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLDebugger.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLRendererAPI.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLShader.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLTexture.cpp
  ${PP_SRC_DIR}/Pepper/Platform/OpenGL/OpenGLVertexArray.cpp
  ${PP_SRC_DIR}/Pepper/Platform/Windows/WinInput.cpp
  ${PP_SRC_DIR}/Pepper/Platform/Windows/WinWindow.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/Buffer.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/BufferLayout.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/OrthoCamera.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/RenderCommand.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/Renderer.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/RendererAPI.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/Shader.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/ShaderLibrary.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/Texture.cpp
  ${PP_SRC_DIR}/Pepper/Renderer/VertexArray.cpp
  ${PP_SRC_DIR}/Pepper/OrthoCameraController.cpp
)

target_compile_features(Pepper PRIVATE ${CXX_STANDARD})

if(WIN32)
  target_compile_options(Pepper PRIVATE /MP /W4 $<$<CONFIG:Debug>:/WX>)
elseif(UNIX)
  target_compile_options(Pepper PRIVATE -Wall -Wextra $<$<CONFIG:Debug>:-Werror>)
endif(WIN32)

if(WIN32)
  set_property(TARGET Pepper PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(Pepper PRIVATE
  ${PP_SRC_DIR}
  ${PP_VENDOR_DIR}/spdlog/include
  ${PP_VENDOR_DIR}/GLFW/include
  ${PP_VENDOR_DIR}/GLAD/include
  ${PP_VENDOR_DIR}/imgui
  ${PP_VENDOR_DIR}/glm/
  ${PP_VENDOR_DIR}/stb/
)

target_compile_definitions(Pepper
  PRIVATE GLFW_INCLUDE_NONE # Not include gl
  PRIVATE $<IF:$<CONFIG:Debug>,PP_DEBUG,PP_RELEASE> # Configurations flags
  PRIVATE $<$<CONFIG:Debug>:PP_ENABLE_ASSERTS> # Debug defines
  PRIVATE PP_CORE
)

if(WIN32)
  target_compile_definitions(Pepper PRIVATE PP_PLATFORM_WINDOWS)
elseif(UNIX)
  target_compile_definitions(Pepper PRIVATE PP_PLATFORM_LINUX)
endif(WIN32)

target_link_libraries(Pepper
  PRIVATE GLAD
  PRIVATE glfw
  PRIVATE ImGui
  PRIVATE glm
)

if(WIN32)
  target_link_libraries(Pepper PRIVATE opengl32.lib)
elseif(UNIX)
  target_link_libraries(Pepper PRIVATE GL)
endif(WIN32)

target_precompile_headers(Pepper PRIVATE ${PP_SRC_DIR}/PepperPCH.hpp)

# #############################################################################
# The client application
add_executable(App
  App/src/ClientApp.cpp
)

target_compile_features(App PRIVATE ${CXX_STANDARD})

target_include_directories(App PRIVATE
  App/src
  ${PP_SRC_DIR}
  ${PP_VENDOR_DIR}/spdlog/include
  ${PP_VENDOR_DIR}/glm/
  ${PP_VENDOR_DIR}/imgui
)

if(WIN32)
  set_property(TARGET App PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

# Configurations flags
target_compile_definitions(App
  PRIVATE APP_WORKING_DIR="${PROJECT_SOURCE_DIR}"
  PRIVATE $<IF:$<CONFIG:Debug>,PP_DEBUG,PP_RELEASE>
)

if(WIN32)
  target_compile_definitions(App PRIVATE PP_PLATFORM_WINDOWS)
elseif(UNIX)
  target_compile_definitions(App PRIVATE PP_PLATFORM_LINUX)
endif(WIN32)

target_link_libraries(App Pepper)

target_precompile_headers(App PRIVATE ${PP_SRC_DIR}/PepperPCH.hpp)