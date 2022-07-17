cmake_minimum_required(VERSION 3.15.0)
project(ButterEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_compile_options(/W4 /WX)

##########################################
# The engine itself is a DLL
add_library(Butter SHARED Butter/src/Test.cpp)
target_include_directories(Butter PRIVATE Butter/src)

##########################################
# The client application
add_executable(App App/src/Application.cpp)
target_include_directories(App PRIVATE App/src)

target_link_libraries(App Butter)