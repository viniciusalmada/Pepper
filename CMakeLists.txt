cmake_minimum_required(VERSION 3.15.0)
project(PepperEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_compile_options(/W4)

##########################################
# GLFW library
add_subdirectory(Pepper/vendor/GLFW)

##########################################
# The engine itself is a DLL
add_library(Pepper SHARED Pepper/src/Pepper/Application.cpp
                          Pepper/src/Pepper/Log.cpp
                          Pepper/src/Pepper/Events/Event.cpp
                          Pepper/src/Platform/WinWindow.cpp)

target_include_directories(Pepper PRIVATE Pepper/src)
target_include_directories(Pepper PRIVATE Pepper/vendor/spdlog/include)
target_include_directories(Pepper PRIVATE Pepper/vendor/GLFW/include)

target_compile_definitions(Pepper PRIVATE BT_PLATFORM_WINDOWS)
target_compile_definitions(Pepper PRIVATE BT_BUILD_DLL)

# Debug defines
target_compile_definitions(Pepper PRIVATE $<$<CONFIG:Debug>:BT_ENABLE_ASSERTS>)

target_link_libraries(Pepper glfw opengl32.lib)

target_precompile_headers(Pepper PRIVATE Pepper/src/PepperPCH.hpp)

##########################################
# The client application
add_executable(App App/src/ClientApp.cpp)

target_include_directories(App PRIVATE App/src)
target_include_directories(App PRIVATE Pepper/src)
target_include_directories(App PRIVATE Pepper/vendor/spdlog/include)

target_compile_definitions(App PRIVATE BT_PLATFORM_WINDOWS)

target_link_libraries(App Pepper)

target_precompile_headers(App PRIVATE Pepper/src/PepperPCH.hpp)