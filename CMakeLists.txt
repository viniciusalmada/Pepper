cmake_minimum_required(VERSION 3.15.0)
project(ButterEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_compile_options(/W4 /WX)

##########################################
# The engine itself is a DLL
add_library(Butter SHARED Butter/src/Butter/Application.cpp
                          Butter/src/Butter/EntryPoint.hpp)
target_include_directories(Butter PRIVATE Butter/src/Butter)

target_compile_definitions(Butter PRIVATE BT_PLATFORM_WINDOWS)
target_compile_definitions(Butter PRIVATE BT_BUILD_DLL)

##########################################
# The client application
add_executable(App App/src/ClientApp.cpp)
target_include_directories(App PRIVATE App/src Butter/src)

target_compile_definitions(App PRIVATE BT_PLATFORM_WINDOWS)

target_link_libraries(App Butter)