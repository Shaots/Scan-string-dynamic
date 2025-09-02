#pragma once

#include "types.hpp"
#include <expected>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace stdx::details {

template <typename T>
bool check_type(std::string_view fmt) {
    if (fmt == "\%d") {
        return std::is_integral<T>::value;
    }
    if (fmt == "\%s") {
        return std::is_same_v<T, std::string>;
    }
    if (fmt == "\%u") {
        return std::is_integral<T>::value && std::is_unsigned<T>::value;
    }
    if (fmt == "\%f") {
        return std::is_floating_point<T>::value;
    }

    return false;
}

template <typename T>
std::expected<T, scan_error> parse_value_with_format(std::string_view input, std::string_view fmt) {
    if (check_type<T>(fmt)) {
        T result{};
        auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result);
        if (ec == std::errc()) {
            std::cout << "Successfully converted: " << result << std::endl;
        } else if (ec == std::errc::invalid_argument) {
            std::cout << "Invalid argument: not a number." << std::endl;
        } else if (ec == std::errc::result_out_of_range) {
            std::cout << "Result out of range for int." << std::endl;
        }
        return static_cast<T>(result);
    }
    return std::unexpected(scan_error{"Unsupported type"});
}

// Функция для проверки корректности входных данных и выделения из обеих строк интересующих данных для парсинга
template <typename... Ts>
std::expected<std::pair<std::vector<std::string_view>, std::vector<std::string_view>>, scan_error>
parse_sources(std::string_view input, std::string_view format) {
    std::vector<std::string_view> format_parts;  // Части формата между {}
    std::vector<std::string_view> input_parts;
    size_t start = 0;
    while (true) {
        size_t open = format.find('{', start);
        if (open == std::string_view::npos) {
            break;
        }
        size_t close = format.find('}', open);
        if (close == std::string_view::npos) {
            break;
        }

        // Если между предыдущей } и текущей { есть текст,
        // проверяем его наличие во входной строке
        if (open > start) {
            std::string_view between = format.substr(start, open - start);
            auto pos = input.find(between);
            if (input.size() < between.size() || pos == std::string_view::npos) {
                return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
            }
            if (start != 0) {
                input_parts.emplace_back(input.substr(0, pos));
            }

            input = input.substr(pos + between.size());
        }

        // Сохраняем спецификатор формата (то, что между {})
        format_parts.push_back(format.substr(open + 1, close - open - 1));
        start = close + 1;
    }

    // Проверяем оставшийся текст после последней }
    if (start < format.size()) {
        std::string_view remaining_format = format.substr(start);
        auto pos = input.find(remaining_format);
        if (input.size() < remaining_format.size() || pos == std::string_view::npos) {
            return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
        }
        input_parts.emplace_back(input.substr(0, pos));
        input = input.substr(pos + remaining_format.size());
    } else {
        input_parts.emplace_back(input);
    }
    return std::pair{format_parts, input_parts};
}

}  // namespace stdx::details