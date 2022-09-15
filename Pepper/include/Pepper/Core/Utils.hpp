#pragma once

#include <functional>
#include <string>

namespace Pepper::Utils
{
  std::string GetNewLine();

  using TimerFun = std::function<void(const std::string&, float)>;

  class Timer
  {
  public:
    Timer(const std::string& name, TimerFun&& fun);
    ~Timer();

    void Stop();

  private:
    DECLARE_PIMPL
  };
}