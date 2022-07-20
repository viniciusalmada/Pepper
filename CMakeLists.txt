cmake_minimum_required(VERSION 3.15.0)
project(ButterEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_compile_options(/W4)

##########################################
# GLFW library
add_subdirectory(Butter/vendor/GLFW)

##########################################
# The engine itself is a DLL
add_library(Butter SHARED Butter/src/Butter/Application.cpp
                          Butter/src/Butter/Log.cpp)

target_include_directories(Butter PRIVATE Butter/src)
target_include_directories(Butter PRIVATE Butter/src/Butter)
target_include_directories(Butter PRIVATE Butter/vendor/spdlog/include)
target_include_directories(Butter PRIVATE Butter/vendor/GLFW/include)

target_compile_definitions(Butter PRIVATE BT_PLATFORM_WINDOWS)
target_compile_definitions(Butter PRIVATE BT_BUILD_DLL)

target_link_libraries(Butter glfw opengl32.lib)

target_precompile_headers(Butter PRIVATE Butter/src/ButterPCH.hpp)

##########################################
# The client application
add_executable(App App/src/ClientApp.cpp)

target_include_directories(App PRIVATE App/src)
target_include_directories(App PRIVATE Butter/src)
target_include_directories(App PRIVATE Butter/vendor/spdlog/include)

target_compile_definitions(App PRIVATE BT_PLATFORM_WINDOWS)

target_link_libraries(App Butter)

target_precompile_headers(App PRIVATE Butter/src/ButterPCH.hpp)