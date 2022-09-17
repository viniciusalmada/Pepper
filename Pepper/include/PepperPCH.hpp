#pragma once

#include <algorithm>
#include <any>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// clang-format off
#include "Pepper/Core/Log.hpp"

#ifdef PP_PLATFORM_WINDOWS
  #include <Windows.h>
#endif
// clang-format on