#pragma once

#include "parse.hpp"
#include "types.hpp"
#include <tuple>
#include <utility>

namespace stdx {
template <typename T>
T process_single_value(std::string_view format, std::string_view input) {
    auto result = details::parse_value_with_format<T>(input, format);
    return *result;
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
