#ifndef __CXX_STDIO_SYNC_FILEBUF_HPP__
#define __CXX_STDIO_SYNC_FILEBUF_HPP__

#include "type_traits.hpp"

#include <cstdio>
#include <streambuf>
#include <utility>
#include <cuchar>

/*
https://code.woboq.org/gcc/libstdc++-v3/include/ext/stdio_sync_filebuf.h.html
https://code.woboq.org/gcc/libstdc++-v3/include/ext/stdio_filebuf.h.html
https://code.woboq.org/gcc/libstdc++-v3/include/std/fstream.html
https://code.woboq.org/gcc/libstdc++-v3/include/bits/fstream.tcc.html
https://code.woboq.org/llvm/libcxx/include/__std_stream.html
https://code.woboq.org/llvm/libcxx/include/__locale.html
https://code.woboq.org/llvm/libcxx/src/locale.cpp.html
https://code.woboq.org/gcc/libstdc++-v3/include/std/sstream.html
https://code.woboq.org/gcc/libstdc++-v3/include/bits/sstream.tcc.html
*/

namespace cxx
{
  template <typename TChar, typename TTraits = std::char_traits<TChar>>
  class stdio_sync_filebuf : public std::basic_streambuf<TChar, TTraits>
  {
  public:
    using char_type      = TChar;
    using traits_type    = TTraits;
    using int_type       = typename traits_type::int_type;
    using pos_type       = typename traits_type::pos_type;
    using off_type       = typename traits_type::off_type;
    using streambuf_type = std::basic_streambuf<TChar, TTraits>;

  private:
    // Underlying stdio FILE
    FILE *m_file;

    // Last character gotten. This is used when pbackfail is
    // called from basic_streambuf::sungetc()
    int_type m_unget_buf;

  public:
    explicit stdio_sync_filebuf(FILE *file) noexcept
        : m_file{ file }
        , m_unget_buf{ traits_type::eof() }
    {
    }

    stdio_sync_filebuf(stdio_sync_filebuf &&filebuf) noexcept
        : streambuf_type{ std::move(filebuf) }
        , m_file{ filebuf.m_file }
        , m_unget_buf{ filebuf.m_unget_buf }
    {
      filebuf.m_file = nullptr;
      filebuf.m_unget_buf = traits_type::eof();
    }

    stdio_sync_filebuf &
    operator=(stdio_sync_filebuf &&filebuf) noexcept
    {
      streambuf_type::operator=(filebuf);
      m_file = std::exchange(filebuf.m_file, nullptr);
      m_unget_buf = std::exchange(filebuf.m_unget_buf, traits_type::eof());
      return *this;
    }

    void swap(stdio_sync_filebuf &filebuf) noexcept
    {
      streambuf_type::swap(filebuf);
      std::swap(m_file, filebuf.m_file);
      std::swap(m_unget_buf, filebuf.m_unget_buf);
    }

    inline FILE *file [[nodiscard]] () noexcept
    {
      return this->m_file;
    }

  protected:
    int_type syncgetc();

    int_type syncungetc(int_type character);

    int_type syncputc(int_type character);

    int_type underflow() override
    {
      int_type character = this->syncgetc();
      return this->syncungetc(character);
    }

    int_type uflow() override
    {
      // Store the gotten character in case we need to unget it.
      m_unget_buf = this->syncgetc();
      return m_unget_buf;
    }

    int_type pbackfail(int_type character = char_type::eof()) override
    {
      int_type char_result;
      const int_type eof = traits_type::eof();

      // Check if the unget or putback was requested
      if (traits_type::eq_int_type(character, eof)) // unget
      {
        if (!traits_type::eq_int_type(m_unget_buf, eof))
          char_result = this->syncungetc(m_unget_buf);
        else // buffer invalid, fail
          char_result = eof;
      }
      else // putback
      {
        char_result = this->syncungetc(character);
      }

      // The buffered character is no longer valid, discard it.
      m_unget_buf = eof;
      return char_result;
    }

    std::streamsize xsgetn(char_type *string, std::streamsize count) override;

    int_type overflow(int_type character = traits_type::eof()) override
    {
      int_type char_result;

      if (traits_type::eq_int_type(character, traits_type::eof()))
      {
        if (fflush(m_file))
          char_result = traits_type::eof();
        else
          char_result = traits_type::not_eof(character);
      }
      else
      {
        char_result = this->syncputc(character);
      }

      return char_result;
    }

    std::streamsize xsputn(const char_type *string, std::streamsize count) override;

    int32_t sync() override
    {
      return std::fflush(m_file);
    }

    std::streampos seekoff(std::streamoff offset, std::ios_base::seekdir direction, [[maybe_unused]] std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override
    {
      std::streampos position(std::streamoff(-1));
      int32_t whence;

      if (direction == std::ios_base::beg)
        whence = SEEK_SET;
      else if (direction == std::ios_base::cur)
        whence = SEEK_CUR;
      else
        whence = SEEK_END;

      if (!fseeko64(m_file, offset, whence))
        position = std::streampos(ftello64(m_file));

#if false
      if (!fseek(m_file, offset, whence))
        position = std::streampos(std::ftell(m_file));
#endif

      return position;
    }

    std::streampos seekpos(std::streampos position, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) override
    {
      return seekoff(std::streamoff(position), std::ios_base::beg, mode);
    }
  };



  ///////////////////////////////////////////////////////////////////////////////



  template <>
  inline stdio_sync_filebuf<char>::int_type
  stdio_sync_filebuf<char>::syncgetc()
  {
    return std::getc(m_file);
  }

  template <>
  inline stdio_sync_filebuf<char>::int_type
  stdio_sync_filebuf<char>::syncungetc(int_type character)
  {
    return std::ungetc(character, m_file);
  }

  template <>
  inline stdio_sync_filebuf<char>::int_type
  stdio_sync_filebuf<char>::syncputc(int_type character)
  {
    return std::putc(character, m_file);
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<char>::xsgetn(char *string, std::streamsize count)
  {
    std::streamsize objects_read = std::fread(string, sizeof(char), count, m_file);

    if (objects_read > 0)
      m_unget_buf = traits_type::to_int_type(string[objects_read - 1]);
    else
      m_unget_buf = traits_type::eof();

    return objects_read;
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<char>::xsputn(const char *string, std::streamsize count)
  {
    return std::fwrite(string, sizeof(char), count, m_file);
  }



  ///////////////////////////////////////////////////////////////////////////////



  template <>
  inline stdio_sync_filebuf<char8_t>::int_type
  stdio_sync_filebuf<char8_t>::syncgetc()
  {
    return std::getc(m_file);
  }

  template <>
  inline stdio_sync_filebuf<char8_t>::int_type
  stdio_sync_filebuf<char8_t>::syncungetc(int_type character)
  {
    return std::ungetc(character, m_file);
  }

  template <>
  inline stdio_sync_filebuf<char8_t>::int_type
  stdio_sync_filebuf<char8_t>::syncputc(int_type character)
  {
    return std::putc(character, m_file);
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<char8_t>::xsgetn(char8_t *string, std::streamsize count)
  {
    std::streamsize objects_read = std::fread(string, sizeof(char8_t), count, m_file);

    if (objects_read > 0)
      m_unget_buf = traits_type::to_int_type(string[objects_read - 1]);
    else
      m_unget_buf = traits_type::eof();

    return objects_read;
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<char8_t>::xsputn(const char8_t *string, std::streamsize count)
  {
    return std::fwrite(string, sizeof(char8_t), count, m_file);
  }



  ///////////////////////////////////////////////////////////////////////////////



  template <>
  inline stdio_sync_filebuf<uchar>::int_type
  stdio_sync_filebuf<uchar>::syncgetc()
  {
    return std::getwc(m_file);
  }

  template <>
  inline stdio_sync_filebuf<uchar>::int_type
  stdio_sync_filebuf<uchar>::syncungetc(int_type character)
  {
    return std::ungetwc(character, m_file);
  }

  template <>
  inline stdio_sync_filebuf<uchar>::int_type
  stdio_sync_filebuf<uchar>::syncputc(int_type character)
  {
    return std::putwc(character, m_file);
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<uchar>::xsgetn(uchar *string, std::streamsize count)
  {
    std::streamsize characters_read = 0;
    const int_type eof = traits_type::eof();

    while (count--)
    {
      int_type character = this->syncgetc();

      if (traits_type::eq_int_type(character, eof))
        break;

      string[characters_read] = traits_type::to_char_type(character);
      ++characters_read;
    }

    if (characters_read > 0)
      m_unget_buf = traits_type::to_int_type(string[characters_read - 1]);
    else
      m_unget_buf = traits_type::eof();

    return characters_read;
  }

  template <>
  inline std::streamsize
  stdio_sync_filebuf<uchar>::xsputn(const uchar *string, std::streamsize count)
  {
    std::streamsize written_characters = 0;
    const int_type eof = traits_type::eof();

    while (count--)
    {
      if (traits_type::eq_int_type(this->syncputc(*string++), eof))
        break;

      ++written_characters;
    }

    return written_characters;
  }

  // extern template class stdio_sync_filebuf<char>;
  // extern template class stdio_sync_filebuf<char8_t>;
  // extern template class stdio_sync_filebuf<uchar>;
} // namespace cxx

#endif //!__CXX_STDIO_SYNC_FILEBUF_HPP__
