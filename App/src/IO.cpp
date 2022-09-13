#include "IO.hpp"

namespace IO
{
  std::filesystem::path GetAssets()
  {
    std::filesystem::path assets_path;
#if defined APP_WORKING_DIR
    const std::string working_dir = APP_WORKING_DIR;
    assets_path = std::filesystem::path{ working_dir } / "App" / "assets";
#else
    assets_path = std::filesystem::current_path().parent_path() / "App" / "assets";
#endif

    return assets_path;
  }
}