#pragma once

#include "parse.hpp"
#include "types.hpp"
#include <charconv>
#include <tuple>
#include <utility>

namespace stdx {
template <typename T>
T process_single_value(std::string_view format, std::string_view input) {
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

template <typename... Ts, size_t... I>
std::tuple<Ts...> process(const std::vector<std::string_view> &formats, const std::vector<std::string_view> &inputs,
                          std::index_sequence<I...>) {
    auto process_element = [&formats, &inputs]<typename T, std::size_t Index>() {
        return process_single_value<T>(formats.at(Index), inputs.at(Index));
    };
    return std::make_tuple(process_element.template operator()<Ts, I>()...);
}

template <typename... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {
    stdx::details::scan_result<Ts...> res;
    auto pair_ = stdx::details::parse_sources(input, format);
    if (pair_.has_value()) {
        res.result = process<Ts...>(pair_->first, pair_->second, std::make_index_sequence<sizeof...(Ts)>{});
        return res;
    }
    return std::unexpected(details::scan_error{"Dumb implementation"});
}

}  // namespace stdx
