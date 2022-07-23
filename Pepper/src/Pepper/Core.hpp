#pragma once

#ifdef BT_PLATFORM_WINDOWS
  #ifdef BT_BUILD_DLL
    #define PEPPER_API __declspec(dllexport)
  #else
    #define PEPPER_API __declspec(dllimport)
  #endif
#else
  #error Pepper only support Windows!
#endif

#ifdef BT_ENABLE_ASSERTS
  #define BT_ASSERT(x, ...)                                                    \
    {                                                                          \
      if (!x)                                                                  \
      {                                                                        \
        BT_ERROR("Assertion failed: {0}", __VA_ARGS__);                        \
        __debugbreak();                                                        \
      }                                                                        \
    }
  #define BT_CORE_ASSERT(x, ...)                                               \
    {                                                                          \
      if (!x)                                                                  \
      {                                                                        \
        BT_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);                   \
        __debugbreak();                                                        \
      }                                                                        \
    }
#else
  #define BT_ASSERT(x, ...)
  #define BT_CORE_ASSERT(x, ...)
#endif