#pragma once

#include <algorithm>
#include <any>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
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
#include "Pepper/Profiling/Profiler.hpp"

#ifdef PP_PLATFORM_WINDOWS
  #include <Windows.h>
#endif
// clang-format on