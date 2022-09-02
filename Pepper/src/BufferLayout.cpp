// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/BufferLayout.hpp"

namespace Pepper
{
  class BufferLayout::Impl
  {
  public:
    Impl(const std::vector<BufferElement>&);

    Impl& operator=(Impl other);

    void CalculateOffsetAndStride();

    std::vector<BufferElement> elements;
    uint32_t stride;
  };

  BufferLayout::Impl::Impl(const std::vector<BufferElement>& elements) : elements(elements)
  {
    CalculateOffsetAndStride();
  }

  BufferLayout::Impl& BufferLayout::Impl::operator=(BufferLayout::Impl other)
  {
    PP_CORE_INFO("Making copy");
    std::swap(elements, other.elements);
    std::swap(stride, other.stride);
    return *this;
  }

  void BufferLayout::Impl::CalculateOffsetAndStride()
  {
    uint32_t offset = 0;
    stride = 0;

    for (auto& element : elements)
    {
      element.offset = offset;
      offset += element.size;
      stride += element.size;
    }
  }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : pimp(CreateScope<Impl>(elements))
  {
  }

  BufferLayout::~BufferLayout() = default;

  BufferLayout& BufferLayout::operator=(const BufferLayout& other)
  {
    if (this != &other)
    {
      if (this->pimp != other.pimp)
      {
        this->pimp = CreateScope<Impl>(other.pimp->elements);
      }
    }
    return *this;
  }

  uint32_t BufferLayout::GetStride() const
  {
    return pimp->stride;
  }

  const std::vector<BufferElement>& BufferLayout::GetElements() const
  {
    return pimp->elements;
  }

  std::vector<BufferElement>::iterator BufferLayout::begin()
  {
    return pimp->elements.begin();
  }

  std::vector<BufferElement>::const_iterator BufferLayout::end() const
  {
    return pimp->elements.cend();
  }

  bool BufferLayout::IsEmpty() const
  {
    return pimp->elements.empty();
  }

  std::vector<BufferElement>::iterator BufferLayout::end()
  {
    return pimp->elements.end();
  }

  std::vector<BufferElement>::const_iterator BufferLayout::begin() const
  {
    return pimp->elements.cbegin();
  }
}