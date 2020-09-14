#ifndef __EXCEPTION_HELPERS_HPP__
#define __EXCEPTION_HELPERS_HPP__

#include "helpers.hpp"

#include <cxxabi.h>
#include <stdexcept>
#include <functional>
#include <typeinfo>
#include <memory>
#include <list>

namespace cxx
{
  [[nodiscard]] std::list<std::reference_wrapper<const std::exception>>
  get_exceptions(const std::exception &exception) noexcept
  {
    std::list<std::reference_wrapper<const std::exception>> exceptions;
    std::function<void(const std::exception &)> recursive_lambda;

    recursive_lambda = [&](const std::exception &exception)
    {
      try
      {
        exceptions.emplace_front(std::cref(exception));
        std::rethrow_if_nested(exception);
      }
      catch (const std::exception &exception)
      {
        recursive_lambda(exception);
      }
      catch (...)
      {
      }
    };

    recursive_lambda(exception);
    return std::move(exceptions);
  }

  auto get_exceptions [[nodiscard]] () noexcept -> decltype(get_exceptions(std::exception{}))
  {
    try
    {
      std::exception_ptr exception_ptr = std::current_exception();

      if (exception_ptr)
      {
        std::rethrow_exception(exception_ptr);
      }

      return decltype(get_exceptions(std::exception{})){};
    }
    catch (const std::exception &exception)
    {
      return get_exceptions(exception);
    }
  }

  struct exception_type_name
  {
    std::unique_ptr<char, decltype(&std::free)> buffer;
    std::string_view demangle;

    exception_type_name(char *name) noexcept
        : buffer(name, std::free), demangle{name}
    {
    }

    exception_type_name(exception_type_name &&right) noexcept
        : buffer(std::move(right.buffer)), demangle(right.demangle)
    {
      right.demangle = std::string_view{};
    }

    exception_type_name(exception_type_name const &) = delete;
    exception_type_name &operator=(exception_type_name const &) = delete;
    ~exception_type_name() = default;

    exception_type_name &operator=(exception_type_name &&right) noexcept
    {
      if (this != &right)
      {
        this->buffer = std::move(right.buffer);
        this->demangle = right.demangle;
        right.demangle = std::string{};
      }

      return *this;
    }

    [[nodiscard]] const std::string_view *operator->() const noexcept
    {
      return &demangle;
    }

    operator std::string_view() const noexcept
    {
      return demangle;
    }
  };

  std::ostream &operator<<(std::ostream &out, const exception_type_name &exception_name) noexcept
  {
    return out << exception_name.demangle;
  }

  exception_type_name get_exception_type_name [[nodiscard]] () noexcept
  {
    std::type_info *type = abi::__cxa_current_exception_type();
    return abi::__cxa_demangle(type->name(), nullptr, nullptr, &discard_value<int32_t>());
  }
} // namespace cxx

// https://en.cppreference.com/w/cpp/language/cast_operator
// https://en.cppreference.com/w/cpp/error/nested_exception
// https://en.cppreference.com/w/cpp/error/exception_ptr
// https://en.cppreference.com/w/cpp/error/rethrow_if_nested
// https://stackoverflow.com/questions/4885334/c-finding-the-type-of-a-caught-default-exception/24997351#24997351

#endif //!__EXCEPTION_HELPERS_HPP__