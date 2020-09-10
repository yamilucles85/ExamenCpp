#ifndef __CXX_ITERATOR_HPP__
#define __CXX_ITERATOR_HPP__

#include <utility>
#include <istream>
#include <cassert>

namespace cxx
{
  template <class Container>
  class emplace_back_insert_iterator
  {
  public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;
    using difference_type   = ptrdiff_t;
    using container_type    = Container;

    explicit constexpr emplace_back_insert_iterator(Container &container) noexcept
        : container{ std::addressof(container) } { }

    constexpr emplace_back_insert_iterator& operator=(const typename Container::value_type &value)
    {
      container->emplace_back(value);
      return *this;
    }

    constexpr emplace_back_insert_iterator& operator=(typename Container::value_type &&value)
    {
      container->emplace_back(std::move(value));
      return *this;
    }

    [[nodiscard]] constexpr emplace_back_insert_iterator& operator*() noexcept
    {
      return *this;
    }

    constexpr emplace_back_insert_iterator& operator++() noexcept
    {
      return *this;
    }

    constexpr emplace_back_insert_iterator operator++(int) noexcept
    {
      return *this;
    }

  protected:
    Container *container = nullptr;
  };

  template <typename Container>
  emplace_back_insert_iterator(Container) -> emplace_back_insert_iterator<Container>;

  template<typename Container>
  constexpr emplace_back_insert_iterator<Container> back_inserter(Container &container)
  {
    return emplace_back_insert_iterator<Container>(container);
  }

  template <class TValue, class TChar = char, class Traits = std::char_traits<TChar>, class TDiff = ptrdiff_t>
  class istream_iterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = TValue;
    using difference_type   = TDiff;
    using pointer           = const TValue*;
    using reference         = const TValue&;
    using char_type         = TChar;
    using traits_type       = Traits;
    using istream_type      = std::basic_istream<TChar, Traits>;

    static_assert(std::conjunction_v<std::is_default_constructible<TValue>, std::is_copy_constructible<TValue>, std::is_copy_assignable<TValue>>,
                  "istream_iterator<T> requires T to be default constructible, copy constructible, and copy assignable. "
                  "(N4835 [istream.iterator]/2)");

    constexpr istream_iterator() : Istr{ nullptr }, m_value{ } { }

    istream_iterator(istream_type &Istr) : Istr{ std::addressof(Istr) } { read_value(); }

    [[nodiscard]] const TValue& operator*() const { return m_value; }

    [[nodiscard]] const TValue* operator->() const { return std::addressof(m_value); }

    istream_iterator& operator++()
    {
      read_value();
      return *this;
    }

    istream_iterator operator++(int)
    {
      istream_iterator temp = *this;
      read_value();
      return temp;
    }

    bool equal(const istream_iterator &right) const noexcept { return Istr == right.Istr; }

  private:
    void read_value()
    {
      if (!(*Istr >> m_value))
      {
        Istr = nullptr;
      }
    }

    istream_type *Istr{ nullptr };
    TValue m_value;
  };

  template <class TValue, class TChar, class Traits, class TDiff>
  [[nodiscard]] bool operator==(const istream_iterator<TValue, TChar, Traits, TDiff> &left, const istream_iterator<TValue, TChar, Traits, TDiff> &right)
  {
    return left.equal(right);
  }

  template <class TValue, class TChar, class Traits, class TDiff>
  [[nodiscard]] bool operator!=(const istream_iterator<TValue, TChar, Traits, TDiff> &left, const istream_iterator<TValue, TChar, Traits, TDiff> &right)
  {
    return !(left == right);
  }
} // namespace cxx

#endif