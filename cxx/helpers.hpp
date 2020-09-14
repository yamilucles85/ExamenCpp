#ifndef __HELPERS_HPP__
#define __HELPERS_HPP__

#include <cstdint>
#include <cinttypes>
#include <cstddef>

namespace cxx
{
  template <typename ValueT>
  decltype(auto) discard_value() noexcept
  {
    static ValueT value;
    return (value);
  }
} // namespace cxx

#endif //!__HELPERS_HPP__