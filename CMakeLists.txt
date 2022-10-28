cmake_minimum_required(VERSION 3.20)
project(PepperEngine VERSION 0.1)

# #############################################################################
# Global constants
set(PP_VENDOR_DIR Pepper/vendor)
set(PP_SRC_DIR Pepper/src)
set(PP_INCLUDE_DIR Pepper/include)

set(CXX_STANDARD cxx_std_20)

# #############################################################################
# Configuration types
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release Profiling)
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Reset the configurations to what we need"
    FORCE)
endif()

set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_PROFILING "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILING "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

# #############################################################################
# GLFW library
add_subdirectory(${PP_VENDOR_DIR}/GLFW)

if(WIN32)
  set_property(TARGET glfw PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

# #############################################################################
# glm library
add_subdirectory(${PP_VENDOR_DIR}/glm)

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
  ${PP_SRC_DIR}/Application.cpp
  ${PP_SRC_DIR}/Buffer.cpp
  ${PP_SRC_DIR}/BufferLayout.cpp
  ${PP_SRC_DIR}/Event.cpp
  ${PP_SRC_DIR}/ImGuiLayer.cpp
  ${PP_SRC_DIR}/Input.cpp
  ${PP_SRC_DIR}/Layer.cpp
  ${PP_SRC_DIR}/LayerStack.cpp
  ${PP_SRC_DIR}/Log.cpp
  ${PP_SRC_DIR}/OpenGLBuffer.cpp
  ${PP_SRC_DIR}/OpenGLContext.cpp
  ${PP_SRC_DIR}/OpenGLDebugger.cpp
  ${PP_SRC_DIR}/OpenGLRendererAPI.cpp
  ${PP_SRC_DIR}/OpenGLShader.cpp
  ${PP_SRC_DIR}/OpenGLTexture.cpp
  ${PP_SRC_DIR}/OpenGLVertexArray.cpp
  ${PP_SRC_DIR}/OrthoCamera.cpp
  ${PP_SRC_DIR}/OrthoCameraController.cpp
  ${PP_SRC_DIR}/Profiler.cpp
  ${PP_SRC_DIR}/RenderCommand.cpp
  ${PP_SRC_DIR}/Renderer.cpp
  ${PP_SRC_DIR}/Renderer2D.cpp
  ${PP_SRC_DIR}/RendererAPI.cpp
  ${PP_SRC_DIR}/Shader.cpp
  ${PP_SRC_DIR}/ShaderLibrary.cpp
  ${PP_SRC_DIR}/Texture.cpp
  ${PP_SRC_DIR}/Timer.cpp
  ${PP_SRC_DIR}/Utils.cpp
  ${PP_SRC_DIR}/VertexArray.cpp
  ${PP_SRC_DIR}/WinInput.cpp
  ${PP_SRC_DIR}/WinWindow.cpp
  )

target_compile_features(Pepper PRIVATE ${CXX_STANDARD})

if(WIN32)
  target_compile_options(Pepper PRIVATE /MP /W4 /WX /w44061 /w44062 /w44191 /w44254 /w44263 /w44265 /w44287 /w44266 /w44289 /w44296 /w44339 /w44350 /w44355)
elseif(UNIX)
  target_compile_options(Pepper PRIVATE -flto=auto -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -fimplicit-constexpr -fstrict-enums -Wnon-virtual-dtor -Wno-pessimizing-move -Wno-redundant-move -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wmismatched-new-delete)
endif(WIN32)

if(WIN32)
  set_property(TARGET Pepper PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(Pepper PRIVATE ${PP_INCLUDE_DIR})
target_include_directories(Pepper SYSTEM PUBLIC
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
  PRIVATE $<$<CONFIG:Profiling>:PROFILING>
  )

if(WIN32)
  target_compile_definitions(Pepper PRIVATE PP_PLATFORM_WINDOWS)
elseif(UNIX)
  target_compile_definitions(Pepper PRIVATE PP_PLATFORM_LINUX)
endif(WIN32)

target_link_libraries(Pepper
  PUBLIC GLAD
  PUBLIC glfw
  PUBLIC ImGui
  PUBLIC glm
  )

if(WIN32)
  target_link_libraries(Pepper PRIVATE opengl32.lib)
elseif(UNIX)
  target_link_libraries(Pepper PRIVATE GL)
endif(WIN32)

target_precompile_headers(Pepper PRIVATE ${PP_INCLUDE_DIR}/PepperPCH.hpp)

# #############################################################################
# The client application
add_executable(App
  App/src/ClientApp.cpp
  App/src/Color.cpp
  App/src/GameLayer.cpp
  App/src/Level.cpp
  App/src/Planet.cpp
  App/src/Player.cpp
  App/src/Random.cpp
  App/src/Utilities.cpp
  App/src/ParticleSystem.cpp)

if(WIN32)
  target_compile_options(App PRIVATE /MP /W4 /WX /w44061 /w44062 /w44191 /w44254 /w44263 /w44265 /w44287 /w44266 /w44289 /w44296 /w44339 /w44350 /w44355)
elseif(UNIX)
  target_compile_options(App PRIVATE -flto=auto -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -fimplicit-constexpr -fstrict-enums -Wnon-virtual-dtor -Wno-pessimizing-move -Wno-redundant-move -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wmismatched-new-delete)
endif(WIN32)

target_compile_features(App PRIVATE ${CXX_STANDARD})

target_include_directories(App PRIVATE App/src)
target_include_directories(App PRIVATE SYSTEM
  ${PP_INCLUDE_DIR}
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
  PRIVATE $<$<CONFIG:Debug>:PP_ENABLE_ASSERTS> # Debug defines
  PRIVATE $<$<CONFIG:Profiling>:PROFILING>
  )

if(WIN32)
  target_compile_definitions(App PRIVATE PP_PLATFORM_WINDOWS)
elseif(UNIX)
  target_compile_definitions(App PRIVATE PP_PLATFORM_LINUX)
endif(WIN32)

target_link_libraries(App Pepper)

target_precompile_headers(App PRIVATE ${PP_INCLUDE_DIR}/PepperPCH.hpp)