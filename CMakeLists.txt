cmake_minimum_required(VERSION 3.15.0)
project(PepperEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_compile_options(/W0)
add_compile_options("$<$<CONFIG:Debug>:/MDd>")
add_compile_options("$<$<CONFIG:Release>:/MD>")
add_compile_options("$<$<CONFIG:MinSizeRel>:/MD>")

##########################################
# Global constants
set(VENDOR_DIR Pepper/vendor)
set(SOURCE_DIR Pepper/src)

##########################################
# GLFW library
add_subdirectory(${VENDOR_DIR}/GLFW)

##########################################
# GLAD library
add_subdirectory(${VENDOR_DIR}/GLAD)

##########################################
# Dear ImGui library
add_library(ImGui STATIC ${VENDOR_DIR}/imgui/imgui_demo.cpp
                         ${VENDOR_DIR}/imgui/imgui_draw.cpp
                         ${VENDOR_DIR}/imgui/imgui_tables.cpp
                         ${VENDOR_DIR}/imgui/imgui_widgets.cpp
                         ${VENDOR_DIR}/imgui/imgui.cpp)

target_include_directories(ImGui PRIVATE ${VENDOR_DIR}/imgui)


##########################################
# The engine itself is a DLL
add_library(Pepper SHARED ${SOURCE_DIR}/Pepper/Application.cpp
                          ${SOURCE_DIR}/Pepper/Log.cpp
                          ${SOURCE_DIR}/Pepper/Events/Event.cpp
                          ${SOURCE_DIR}/Platform/WinWindow.cpp
                          ${SOURCE_DIR}/Pepper/Layer.cpp
                          ${SOURCE_DIR}/Pepper/LayerStack.cpp
                          ${SOURCE_DIR}/Pepper/ImGui/ImGuiLayer.cpp
                          ${SOURCE_DIR}/Platform/OpenGL/ImGuiOpenGLRenderer.cpp)

target_include_directories(Pepper PRIVATE ${SOURCE_DIR})
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/spdlog/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/GLFW/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/GLAD/include)
target_include_directories(Pepper PRIVATE ${VENDOR_DIR}/imGui)

target_compile_definitions(Pepper PRIVATE PP_PLATFORM_WINDOWS)
target_compile_definitions(Pepper PRIVATE PP_BUILD_DLL)
target_compile_definitions(Pepper PRIVATE GLFW_INCLUDE_NONE) # not include gl

# Debug defines
target_compile_definitions(Pepper PRIVATE $<$<CONFIG:Debug>:PP_ENABLE_ASSERTS>)

target_link_libraries(Pepper GLAD glfw ImGui opengl32.lib)

target_precompile_headers(Pepper PRIVATE ${SOURCE_DIR}/PepperPCH.hpp)

##########################################
# The client application
add_executable(App App/src/ClientApp.cpp)

target_include_directories(App PRIVATE App/src)
target_include_directories(App PRIVATE ${SOURCE_DIR})
target_include_directories(App PRIVATE ${VENDOR_DIR}/spdlog/include)

target_compile_definitions(App PRIVATE PP_PLATFORM_WINDOWS)

target_link_libraries(App Pepper)

target_precompile_headers(App PRIVATE ${SOURCE_DIR}/PepperPCH.hpp)