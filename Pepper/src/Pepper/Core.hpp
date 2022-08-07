#pragma once

#ifdef PP_PLATFORM_WINDOWS
  #ifdef PP_DYNAMIC_LINK
    #ifdef PP_BUILD_DLL
      #define PEPPER_API __declspec(dllexport)
    #else
      #define PEPPER_API __declspec(dllimport)
    #endif
  #else
    #define PEPPER_API
  #endif
#else
  #error Pepper only support Windows!
#endif

#ifdef PP_ENABLE_ASSERTS
  #define PP_ASSERT(x, ...)                                                    \
    {                                                                          \
      if (!x)                                                                  \
      {                                                                        \
        PP_ERROR("Assertion failed: {0}", __VA_ARGS__);                        \
        __debugbreak();                                                        \
      }                                                                        \
    }
  #define PP_CORE_ASSERT(x, ...)                                               \
    {                                                                          \
      if (!x)                                                                  \
      {                                                                        \
        PP_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);                   \
        __debugbreak();                                                        \
      }                                                                        \
    }
#else
  #define PP_ASSERT(x, ...)
  #define PP_CORE_ASSERT(x, ...)
#endif

#define FIND(cont, x) std::find(cont.begin(), cont.end(), x)

#define PP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)