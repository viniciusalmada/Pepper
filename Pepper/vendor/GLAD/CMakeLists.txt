cmake_minimum_required(VERSION 3.15.0)
project(GLAD)

add_library(GLAD STATIC src/glad.c)

set_property(TARGET GLAD PROPERTY
  MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)

target_include_directories(GLAD PRIVATE include)