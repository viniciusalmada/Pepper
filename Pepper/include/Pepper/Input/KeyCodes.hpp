#pragma once

struct PPKey
{
  int keyCode;

  friend std::ostream& operator<<(std::ostream& os, PPKey ppKey) { return os << ppKey.keyCode; }

  static PPKey FromGLFWKey(int glfwKeyCode) { return PPKey{ glfwKeyCode }; }

  static int ToGLFWKey(PPKey ppKey) { return ppKey.keyCode; }
};

// From glfw3.h, but with constexpr
constexpr PPKey PP_KEY_SPACE{ 32 };
constexpr PPKey PP_KEY_APOSTROPHE{ 39 }; /* ' */
constexpr PPKey PP_KEY_COMMA{ 44 };      /* , */
constexpr PPKey PP_KEY_MINUS{ 45 };      /* - */
constexpr PPKey PP_KEY_PERIOD{ 46 };     /* . */
constexpr PPKey PP_KEY_SLASH{ 47 };      /* / */
constexpr PPKey PP_KEY_0{ 48 };
constexpr PPKey PP_KEY_1{ 49 };
constexpr PPKey PP_KEY_2{ 50 };
constexpr PPKey PP_KEY_3{ 51 };
constexpr PPKey PP_KEY_4{ 52 };
constexpr PPKey PP_KEY_5{ 53 };
constexpr PPKey PP_KEY_6{ 54 };
constexpr PPKey PP_KEY_7{ 55 };
constexpr PPKey PP_KEY_8{ 56 };
constexpr PPKey PP_KEY_9{ 57 };
constexpr PPKey PP_KEY_SEMICOLON{ 59 }; /* ; */
constexpr PPKey PP_KEY_EQUAL{ 61 };     /* = */
constexpr PPKey PP_KEY_A{ 65 };
constexpr PPKey PP_KEY_B{ 66 };
constexpr PPKey PP_KEY_C{ 67 };
constexpr PPKey PP_KEY_D{ 68 };
constexpr PPKey PP_KEY_E{ 69 };
constexpr PPKey PP_KEY_F{ 70 };
constexpr PPKey PP_KEY_G{ 71 };
constexpr PPKey PP_KEY_H{ 72 };
constexpr PPKey PP_KEY_I{ 73 };
constexpr PPKey PP_KEY_J{ 74 };
constexpr PPKey PP_KEY_K{ 75 };
constexpr PPKey PP_KEY_L{ 76 };
constexpr PPKey PP_KEY_M{ 77 };
constexpr PPKey PP_KEY_N{ 78 };
constexpr PPKey PP_KEY_O{ 79 };
constexpr PPKey PP_KEY_P{ 80 };
constexpr PPKey PP_KEY_Q{ 81 };
constexpr PPKey PP_KEY_R{ 82 };
constexpr PPKey PP_KEY_S{ 83 };
constexpr PPKey PP_KEY_T{ 84 };
constexpr PPKey PP_KEY_U{ 85 };
constexpr PPKey PP_KEY_V{ 86 };
constexpr PPKey PP_KEY_W{ 87 };
constexpr PPKey PP_KEY_X{ 88 };
constexpr PPKey PP_KEY_Y{ 89 };
constexpr PPKey PP_KEY_Z{ 90 };
constexpr PPKey PP_KEY_LEFT_BRACKET{ 91 };  /* [ */
constexpr PPKey PP_KEY_BACKSLASH{ 92 };     /* \ */
constexpr PPKey PP_KEY_RIGHT_BRACKET{ 93 }; /* ] */
constexpr PPKey PP_KEY_GRAVE_ACCENT{ 96 };  /* ` */
constexpr PPKey PP_KEY_WORLD_1{ 161 };      /* non-US #1 */
constexpr PPKey PP_KEY_WORLD_2{ 162 };      /* non-US #2 */
constexpr PPKey PP_KEY_ESCAPE{ 256 };
constexpr PPKey PP_KEY_ENTER{ 257 };
constexpr PPKey PP_KEY_TAB{ 258 };
constexpr PPKey PP_KEY_BACKSPACE{ 259 };
constexpr PPKey PP_KEY_INSERT{ 260 };
constexpr PPKey PP_KEY_DELETE{ 261 };
constexpr PPKey PP_KEY_RIGHT{ 262 };
constexpr PPKey PP_KEY_LEFT{ 263 };
constexpr PPKey PP_KEY_DOWN{ 264 };
constexpr PPKey PP_KEY_UP{ 265 };
constexpr PPKey PP_KEY_PAGE_UP{ 266 };
constexpr PPKey PP_KEY_PAGE_DOWN{ 267 };
constexpr PPKey PP_KEY_HOME{ 268 };
constexpr PPKey PP_KEY_END{ 269 };
constexpr PPKey PP_KEY_CAPS_LOCK{ 280 };
constexpr PPKey PP_KEY_SCROLL_LOCK{ 281 };
constexpr PPKey PP_KEY_NUM_LOCK{ 282 };
constexpr PPKey PP_KEY_PRINT_SCREEN{ 283 };
constexpr PPKey PP_KEY_PAUSE{ 284 };
constexpr PPKey PP_KEY_F1{ 290 };
constexpr PPKey PP_KEY_F2{ 291 };
constexpr PPKey PP_KEY_F3{ 292 };
constexpr PPKey PP_KEY_F4{ 293 };
constexpr PPKey PP_KEY_F5{ 294 };
constexpr PPKey PP_KEY_F6{ 295 };
constexpr PPKey PP_KEY_F7{ 296 };
constexpr PPKey PP_KEY_F8{ 297 };
constexpr PPKey PP_KEY_F9{ 298 };
constexpr PPKey PP_KEY_F10{ 299 };
constexpr PPKey PP_KEY_F11{ 300 };
constexpr PPKey PP_KEY_F12{ 301 };
constexpr PPKey PP_KEY_F13{ 302 };
constexpr PPKey PP_KEY_F14{ 303 };
constexpr PPKey PP_KEY_F15{ 304 };
constexpr PPKey PP_KEY_F16{ 305 };
constexpr PPKey PP_KEY_F17{ 306 };
constexpr PPKey PP_KEY_F18{ 307 };
constexpr PPKey PP_KEY_F19{ 308 };
constexpr PPKey PP_KEY_F20{ 309 };
constexpr PPKey PP_KEY_F21{ 310 };
constexpr PPKey PP_KEY_F22{ 311 };
constexpr PPKey PP_KEY_F23{ 312 };
constexpr PPKey PP_KEY_F24{ 313 };
constexpr PPKey PP_KEY_F25{ 314 };
constexpr PPKey PP_KEY_KP_0{ 320 };
constexpr PPKey PP_KEY_KP_1{ 321 };
constexpr PPKey PP_KEY_KP_2{ 322 };
constexpr PPKey PP_KEY_KP_3{ 323 };
constexpr PPKey PP_KEY_KP_4{ 324 };
constexpr PPKey PP_KEY_KP_5{ 325 };
constexpr PPKey PP_KEY_KP_6{ 326 };
constexpr PPKey PP_KEY_KP_7{ 327 };
constexpr PPKey PP_KEY_KP_8{ 328 };
constexpr PPKey PP_KEY_KP_9{ 329 };
constexpr PPKey PP_KEY_KP_DECIMAL{ 330 };
constexpr PPKey PP_KEY_KP_DIVIDE{ 331 };
constexpr PPKey PP_KEY_KP_MULTIPLY{ 332 };
constexpr PPKey PP_KEY_KP_SUBTRACT{ 333 };
constexpr PPKey PP_KEY_KP_ADD{ 334 };
constexpr PPKey PP_KEY_KP_ENTER{ 335 };
constexpr PPKey PP_KEY_KP_EQUAL{ 336 };
constexpr PPKey PP_KEY_LEFT_SHIFT{ 340 };
constexpr PPKey PP_KEY_LEFT_CONTROL{ 341 };
constexpr PPKey PP_KEY_LEFT_ALT{ 342 };
constexpr PPKey PP_KEY_LEFT_SUPER{ 343 };
constexpr PPKey PP_KEY_RIGHT_SHIFT{ 344 };
constexpr PPKey PP_KEY_RIGHT_CONTROL{ 345 };
constexpr PPKey PP_KEY_RIGHT_ALT{ 346 };
constexpr PPKey PP_KEY_RIGHT_SUPER{ 347 };
constexpr PPKey PP_KEY_MENU{ 348 };