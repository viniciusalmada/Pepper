#pragma once

#include <string>
namespace Pepper::Utils
{
  std::string GetNewLine();

  class Timer
  {
  public:
    Timer(const std::string& name);
    ~Timer();

    void Stop();

  private:
    DECLARE_PIMPL
  };
}