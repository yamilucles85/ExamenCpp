#ifndef __CXX_INPUT_HPP__
#define __CXX_INPUT_HPP__

#include <iostream>
#include <concepts>
#include <string>
#include <cstddef>
#include <sstream>

namespace cxx
{
  namespace detail
  {
    template <typename T>
    T convert(const std::string &number, std::size_t *index);

    template <>
    std::byte convert<std::byte>(const std::string &number, std::size_t *index)
    {
      int32_t value = std::stoi(number, index);

      if (value < std::numeric_limits<std::underlying_type_t<std::byte>>::min() or
          value > std::numeric_limits<std::underlying_type_t<std::byte>>::max())
      {
        throw std::out_of_range("");
      }

      return static_cast<std::byte>(value);
    }

    template <>
    int8_t convert<int8_t>(const std::string &number, std::size_t *index)
    {
      int32_t value = std::stoi(number, index);

      if (value < std::numeric_limits<int8_t>::min() or
          value > std::numeric_limits<int8_t>::max())
      {
        throw std::out_of_range("");
      }

      return static_cast<int8_t>(value);
    }

    template <>
    int16_t convert<int16_t>(const std::string &number, std::size_t *index)
    {
      int32_t value = std::stoi(number, index);

      if (value < std::numeric_limits<int16_t>::min() or
          value > std::numeric_limits<int16_t>::max())
      {
        throw std::out_of_range("");
      }

      return static_cast<int16_t>(value);
    }

    template <>
    int32_t convert<int32_t>(const std::string &number, std::size_t *index)
    {
      return std::stoi(number, index);
    }

    template <>
    long convert<long>(const std::string &number, std::size_t *index)
    {
      return std::stol(number, index);
    }

    template <>
    long long convert<long long>(const std::string &number, std::size_t *index)
    {
      return std::stoll(number, index);
    }

    template <>
    unsigned long convert<unsigned long>(const std::string &number, std::size_t *index)
    {
      return std::stoul(number, index);
    }

    template <>
    unsigned long long convert<unsigned long long>(const std::string &number, std::size_t *index)
    {
      return std::stoull(number, index);
    }

    template <>
    float convert<float>(const std::string &number, std::size_t *index)
    {
      return std::stof(number, index);
    }

    template <>
    double convert<double>(const std::string &number, std::size_t *index)
    {
      return std::stod(number, index);
    }

    template <>
    long double convert<long double>(const std::string &number, std::size_t *index)
    {
      return std::stold(number, index);
    }
  } // namespace detail

  bool read(std::string &value, std::string *msg_error = nullptr)
  {
    std::getline(std::cin, value);

    if (std::cin.good())
    {
      return true;
    }
    else
    {
      if (msg_error != nullptr)
      {
        std::error_code error = std::make_error_code(static_cast<std::errc>(errno));
        msg_error->assign(error.message());
      }

      return false;
    }
  }

  template <typename T>
  concept Number = std::integral<T> or std::floating_point<T>;

  bool read(Number auto &value, std::string *msg_error = nullptr)
  {
    using value_t = std::remove_cvref_t<decltype(value)>;

    std::string number;
    std::getline(std::cin >> std::ws, number);

    if (std::cin.good())
    {
      try
      {
        size_t number_of_characters_processed;
        value = detail::convert<value_t>(number, &number_of_characters_processed);

        if (number_of_characters_processed != number.size())
        {
          if (msg_error != nullptr)
          {
            std::string_view valid_part(number.c_str(), number_of_characters_processed);
            std::string_view invalid_part(
                number.data() + number_of_characters_processed,
                number.length() - number_of_characters_processed);

            std::ostringstream error;
            error << "The number is not in the correct format. "
                  << "Valid part: \"" << valid_part << "\", "
                  << "Invalid part: \"" << invalid_part << "\".";

            msg_error->assign(error.str());
          }

          return false;
        }

        return true;
      }
      catch (const std::invalid_argument &)
      {
        if (msg_error != nullptr)
        {
          std::error_code error = std::make_error_code(std::errc::invalid_argument);
          msg_error->assign(error.message());
        }

        return false;
      }
      catch (const std::out_of_range &)
      {
        if (msg_error != nullptr)
        {
          std::error_code error = std::make_error_code(std::errc::result_out_of_range);
          msg_error->assign(error.message());
        }

        return false;
      }
    }
    else
    {
      if (msg_error != nullptr)
      {
        std::error_code error = std::make_error_code(static_cast<std::errc>(errno));
        msg_error->assign(error.message());
      }

      return false;
    }
  }

  template <typename T>
  struct read_result
  {
    T           value;
    bool        is_success;
    bool        has_error;
    std::string error_message;
  };

  template <typename T>
  read_result<T> read()
  {
    read_result<T> result;
    result.is_success = read(result.value, &result.error_message);
    result.has_error  = !result.is_success;
    return result;
  }

  std::ostream &operator<<(std::ostream &out, const std::byte byte)
  {
    return out << std::to_integer<int32_t>(byte);
  }
} // namespace cxx

#endif //!__CXX_INPUT_HPP__