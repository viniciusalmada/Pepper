// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Platform/OpenGL/OpenGLDebugger.hpp"

#include <glad/glad.h>

namespace Pepper
{
  void OpenGLDebuggerFunc(GLenum /* source */,
                          GLenum /* type */,
                          unsigned int id,
                          GLenum /* severity */,
                          GLsizei /* length */,
                          const char* message,
                          const void* /* userParam */)
  {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
      return;

    PP_CORE_WARN("OpenGL Error:");
    PP_CORE_WARN("  (0x{0:04x}): {1}", id, message);

    PP_CORE_ASSERT(false, "");
  }
}