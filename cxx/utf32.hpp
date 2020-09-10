#ifndef __UTF32_HPP__
#define __UTF32_HPP__

#include "utf8.hpp"

namespace cxx
{
  std::ostream& operator<<(std::ostream &out, const std::u32string_view u32text) noexcept
  {
    using codecvt_t = std::codecvt<char32_t, char8_t, std::mbstate_t>;
    const auto &facet = std::use_facet<codecvt_t>(out.getloc());

    std::u8string u8text(u32text.size() * facet.max_length(), u8'\0');

    std::mbstate_t mbstate{ };

    std::u32string_view::const_pointer from_next = nullptr;
    std::u8string::pointer to_next = nullptr;

    codecvt_t::result result =
        facet.out(mbstate, u32text.begin(), u32text.end(), from_next,
                  u8text.data(), u8text.data() + u8text.size(), to_next);

    if (result == codecvt_t::result::ok)
    {
      size_t utf8size = to_next - u8text.data();
      out << std::u8string_view(u8text.data(), utf8size);
    }
    else
    {
      out.setstate(std::ios::failbit);
    }

    return out;
  }

  inline std::ostream& operator<<(std::ostream &out, const char32_t *text) noexcept
  {
    return out << std::u32string_view{ text };
  }

  inline std::ostream& operator<<(std::ostream &out, const char32_t letter) noexcept
  {
    const char32_t text[2] = { letter, U'\0' };
    return out << std::u32string_view(text, 2);
  }
} // namespace cxx

#endif //!__UTF32_HPP__