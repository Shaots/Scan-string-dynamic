#pragma once
#include <string>
#include <tuple>
namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

struct scan_error {
    std::string message;
};

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> result;
    void value();
};

namespace err_msg {
const char *Err_format = "Incorrect format\0";
const char *Err_invalid_arg = "Invalid arguments\0";
const char *Err_out_range = "Result out of the range\0";
}  // namespace err_msg

}  // namespace stdx::details
