#include <climits>
#include<cstdint>
#include <gtest/gtest.h>
#include <print>
#include <string>
#include <string_view>

#include "scan.hpp"

// 0 : General test
TEST(ScanTest, SimpleTest) {
    auto res = stdx::scan<int, int, int, double>("ABC 04 DEFG 15 HIGK -17 LMN 15.6",
                                                 "ABC {\%d} DEFG {\%d} HIGK {\%d} LMN {\%f}");
    EXPECT_EQ(std::get<0>(res->result), 4);
    EXPECT_EQ(std::get<1>(res->result), 15);
    EXPECT_EQ(std::get<2>(res->result), -17);
    EXPECT_EQ(std::get<3>(res->result), 15.6);
    ASSERT_TRUE(res);
}

// 1 : Check format ant template type
TEST(ScanTest, format) {
    auto res = stdx::details::parse_value_with_format<unsigned int>("78", "\%d");
    EXPECT_EQ(*res, 78);

    res = stdx::details::parse_value_with_format<int>("79", "\%d");
    EXPECT_EQ(*res, 79);

    res = stdx::details::parse_value_with_format<int>("78.5", "\%f");
    EXPECT_EQ(res.has_value(), false);
    EXPECT_EQ(res.error().message, "Incorrect format");
}

// 2 : Check integral format
TEST(ScanTest, integralFormat) {
    std::string fmt_ = "\%d";
    std::string_view fmt(fmt_);
    auto res = stdx::details::parse_value_with_format<int>(std::to_string(INT_MAX), fmt);
    EXPECT_EQ(*res, INT_MAX);

    res = stdx::details::parse_value_with_format<int8_t>(std::to_string(INT8_MAX), fmt);
    EXPECT_EQ(*res, INT8_MAX);

    res = stdx::details::parse_value_with_format<int16_t>(std::to_string(INT16_MAX), fmt);
    EXPECT_EQ(*res, INT16_MAX);

    res = stdx::details::parse_value_with_format<int32_t>(std::to_string(INT32_MAX), fmt);
    EXPECT_EQ(*res, INT32_MAX);

    res = stdx::details::parse_value_with_format<int64_t>(std::to_string(INT64_MAX), fmt);
    EXPECT_EQ(*res, INT64_MAX);

    res = stdx::details::parse_value_with_format<unsigned int>(std::to_string(UINT_MAX), fmt);
    EXPECT_EQ(*res, UINT_MAX);

    res = stdx::details::parse_value_with_format<uint8_t>(std::to_string(UINT8_MAX), fmt);
    EXPECT_EQ(*res, UINT8_MAX);

    res = stdx::details::parse_value_with_format<uint16_t>(std::to_string(UINT16_MAX), fmt);
    EXPECT_EQ(*res, UINT16_MAX);

    res = stdx::details::parse_value_with_format<uint32_t>(std::to_string(UINT32_MAX), fmt);
    EXPECT_EQ(*res, UINT32_MAX);

    res = stdx::details::parse_value_with_format<uint64_t>(std::to_string(UINT64_MAX), fmt);
    EXPECT_EQ(*res, UINT64_MAX);
}