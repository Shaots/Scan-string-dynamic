#pragma once

#include "parse.hpp"
#include "types.hpp"
#include <tuple>
#include <utility>

namespace stdx {

using namespace details;

template <typename T>
std::expected<T, scan_error> process_single_value(std::string_view format, std::string_view input) {
    return parse_value_with_format<T>(input, format);
}

template <typename... Ts, size_t... I>
std::expected<std::tuple<Ts...>, scan_error> process(const std::vector<std::string_view> &formats,
                                                     const std::vector<std::string_view> &inputs,
                                                     std::index_sequence<I...>) {
    auto process_element = [&formats, &inputs]<typename T, std::size_t Index>() {
        return process_single_value<T>(formats.at(Index), inputs.at(Index));
    };
    auto res = std::make_tuple(process_element.template operator()<Ts, I>()...);

    // Look for scan_error type in tuple
    bool detect = true;
    std::apply([&detect](const auto &...tupleArgs) { ((detect &= tupleArgs.has_value()), ...); }, res);

    if (!detect) {
        return std::unexpected(scan_error{err_msg::Err_exist_error});
    }
    return std::apply([](const auto &...tupleArgs) { return std::make_tuple(tupleArgs.value()...); }, res);
}

template <ScanableType... Ts>
std::expected<scan_result<Ts...>, scan_error> scan(std::string_view input, std::string_view format) {
    auto pair_ = stdx::parse_sources(input, format);
    if (!pair_.has_value()) {
        return std::unexpected(pair_.error());
    }

    auto res_ = process<Ts...>(pair_->first, pair_->second, std::make_index_sequence<sizeof...(Ts)>{});
    if (!res_.has_value()) {
        return std::unexpected(res_.error());
    }
    scan_result<Ts...> res(std::move(*res_));
    return res;
}

}  // namespace stdx
