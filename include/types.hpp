#pragma once
#include <cstddef>
#include <expected>
#include <string>
#include <tuple>
namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

struct scan_error {
    std::string message;
};

template <typename T>
using parse_result = std::expected<T, scan_error>;

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
struct scan_result {
    scan_result(std::tuple<Ts...>&& res) : result(std::move(res)) {}

    const std::tuple<Ts...>& value() {
        return result;
    }

    template<size_t I>
    constexpr auto get() {
        return std::get<I>(result);
    }

private:
    std::tuple<Ts...> result;
};

namespace err_msg {
const char *Err_format = "Incorrect format\0";
const char *Err_invalid_arg = "Invalid arguments\0";
const char *Err_out_range = "Result out of the range\0";
const char *Err_unmatched = "Formatted string does not match source string\0";
const char *Err_exist_error = "Scan error exists in tuple\0";
}  // namespace err_msg

}  // namespace stdx::details
