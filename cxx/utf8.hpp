#ifndef __UTF8_HPP__
#define __UTF8_HPP__

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <stdexcept>

namespace cxx
{
  inline std::ostream& operator<<(std::ostream &out, const std::u8string_view text) noexcept
  {
    return out << reinterpret_cast<const char*>(text.data());
  }

  inline std::ostream& operator<<(std::ostream &out, const char8_t *text) noexcept
  {
    return out << reinterpret_cast<const char*>(text);
  }

  inline std::ostream& operator<<(std::ostream &out, const char8_t letter) noexcept
  {
    const char8_t text[2] = { letter, u8'\0' };
    return out << reinterpret_cast<const char*>(text);
  }

  std::istream& getline(std::istream &is, std::u8string &text, const char8_t &delim = u8'\n')
  {
    using traits = std::char_traits<char8_t>;
    using stream = std::istream;

    stream::iostate state = stream::goodbit;
    bool changed = false;
    const stream::sentry ok(is, true);

    if (ok)
    {
      try
      {
        text.erase();
        const traits::int_type meta_delim = traits::to_int_type(delim);
        traits::int_type meta = is.rdbuf()->sgetc();

        for (;; meta = is.rdbuf()->snextc())
        {
          if (traits::eq_int_type(traits::eof(), meta))
          {
            state |= stream::eofbit;
            break;
          }
          else if (traits::eq_int_type(meta, delim))
          {
            changed = true;
            is.rdbuf()->sbumpc();
            break;
          }
          else if (text.max_size() <= text.size())
          {
            state |= stream::failbit;
            break;
          }
          else
          {
            text.push_back(traits::to_char_type(meta));
            changed = true;
          }
        }
      }
      catch (...)
      {
        is.exceptions(stream::goodbit);
        is.clear(stream::badbit);
        // https://en.cppreference.com/w/cpp/error/nested_exception
        // https://en.cppreference.com/w/cpp/error/exception_ptr
        // https://en.cppreference.com/w/cpp/error/rethrow_if_nested
        std::throw_with_nested(stream::failure("ios_base::badbit set"));
      }
    }

    if (!changed) {
      state |= stream::failbit;
    }

    is.setstate(state);
    return is;
  }
} // namespace cxx

#endif //!__UTF8_HPP__