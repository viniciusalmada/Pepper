#pragma once

// clang-format off
#include <algorithm>
#include <any>
#include <cstdint>
#include <fstream>
#include <functional>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Pepper/Core/Log.hpp"

#ifdef PP_PLATFORM_WINDOWS
  #include <Windows.h>
#endif
// clang-format on