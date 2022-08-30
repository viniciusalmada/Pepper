#pragma once

#include "BufferElement.hpp"

namespace Pepper
{
  class BufferLayout
  {
  public:
    BufferLayout(const std::initializer_list<BufferElement>&);
    ~BufferLayout();

    BufferLayout& operator=(const BufferLayout&);

    uint32_t GetStride() const;

    const std::vector<BufferElement>& GetElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

    bool IsEmpty() const;

  private:
    PIMPL()
  };
}