#pragma once

#ifdef BT_PLATFORM_WINDOWS
  #ifdef BT_BUILD_DLL
    #define BUTTER_API __declspec(dllexport)
  #else
    #define BUTTER_API __declspec(dllimport)
  #endif
#else
  #error Butter only support Windows!
#endif