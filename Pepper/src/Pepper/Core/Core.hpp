#pragma once

#ifdef PP_PLATFORM_WINDOWS
  #define DEBUG_BREAK __debugbreak();
#else
  #define DEBUG_BREAK __builtin_trap();
#endif

#ifdef PP_ENABLE_ASSERTS
  #define PP_ASSERT(x, ...)                                                                                            \
    {                                                                                                                  \
      if (!(x))                                                                                                        \
      {                                                                                                                \
        PP_ERROR("Assertion failed: {0}", __VA_ARGS__);                                                                \
        DEBUG_BREAK                                                                                                    \
      }                                                                                                                \
    }
  #define PP_CORE_ASSERT(x, ...)                                                                                       \
    {                                                                                                                  \
      if (!(x))                                                                                                        \
      {                                                                                                                \
        PP_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);                                                           \
        DEBUG_BREAK                                                                                                    \
      }                                                                                                                \
    }
#else
  #define PP_ASSERT(x, ...)
  #define PP_CORE_ASSERT(x, ...)
#endif

#define FIND(cont, x) std::find(cont.begin(), cont.end(), x)

#define PP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)