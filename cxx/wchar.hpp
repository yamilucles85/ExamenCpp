#ifndef __WCHAR_HPP__
#define __WCHAR_HPP__

#include "utf16.hpp"
#include "utf32.hpp"
#include <utility>

namespace cxx
{
  std::ostream& operator<<(std::ostream &out, const std::wstring_view text) noexcept
  {
    #if defined( __GNUC__ )
      out << std::u32string_view(reinterpret_cast<const char32_t*>(text.data()), text.size());
    #elif defined( _MSC_VER )
      out << std::u16string_view(reinterpret_cast<const char16_t*>(text.data()), text.size());
    #else
      if constexpr (sizeof(wchar_t) == sizeof(char32_t))
      {
        out << std::u32string_view(reinterpret_cast<const char32_t*>(text.data()), text.size());
      }
      else if constexpr (sizeof(wchar_t) == sizeof(char16_t))
      {
        out << std::u16string_view(reinterpret_cast<const char16_t*>(text.data()), text.size());
      }
      else
      {
        out.setstate(std::ios::failbit);
      }
    #endif

    return out;
  }

  inline std::ostream& operator<<(std::ostream &out, const wchar_t *text) noexcept
  {
    return out << std::wstring_view{ text };
  }

  inline std::ostream& operator<<(std::ostream &out, const wchar_t letter) noexcept
  {
    const wchar_t text[2] = { letter, L'\0' };
    return out << std::wstring_view(text, 2);
  }
} // namespace cxx

#endif //!__WCHAR_HPP__