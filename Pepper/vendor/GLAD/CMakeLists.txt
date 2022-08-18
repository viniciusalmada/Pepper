cmake_minimum_required(VERSION 3.15.0)
project(GLAD)

add_library(GLAD STATIC src/glad.c)

if(WIN32)
  set_property(TARGET GLAD PROPERTY
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif(WIN32)

target_include_directories(GLAD PRIVATE include)