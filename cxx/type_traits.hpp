#ifndef __CXX_TYPE_TRAITS_HPP__
#define __CXX_TYPE_TRAITS_HPP__

#include <utility>
#include <string>
#include <string_view>

namespace cxx
{
  namespace detail
  {
    template<size_t size>
    struct utf_traits
    {
      using char_t      = wchar_t;
      using string_t    = std::wstring;
      using view_t      = std::wstring_view;
      using streampos_t = std::wstreampos;
    };

    template<>
    struct utf_traits<sizeof(char16_t)>
    {
      using char_t      = char16_t;
      using string_t    = std::u16string;
      using view_t      = std::u16string_view;
      using streampos_t = std::u16streampos;
    };

    template<>
    struct utf_traits<sizeof(char32_t)>
    {
      using char_t      = char32_t;
      using string_t    = std::u32string;
      using view_t      = std::u32string_view;
      using streampos_t = std::u32streampos;
    };

    using utf_t = detail::utf_traits<sizeof(wchar_t)>;
  } // namespace detail

  using uchar        = detail::utf_t::char_t;
  using ustring      = detail::utf_t::string_t;
  using ustring_view = detail::utf_t::view_t;
  using ustreampos   = detail::utf_t::streampos_t;

} // namespace cxx

#endif //!__CXX_TYPE_TRAITS_HPP__