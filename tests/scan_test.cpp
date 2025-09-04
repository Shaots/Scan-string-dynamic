#include <climits>
#include <cstdint>
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
    auto res1 = stdx::details::parse_value_with_format<unsigned int>("78", "\%d");
    EXPECT_EQ(*res1, 78);

    auto res2 = stdx::details::parse_value_with_format<int>("79", "\%d");
    EXPECT_EQ(*res2, 79);

    auto res3 = stdx::details::parse_value_with_format<int>("78.5", "\%f");
    EXPECT_EQ(res3.has_value(), false);
    EXPECT_EQ(res3.error().message, "Incorrect format");
}

// 2 : Check integral format
TEST(ScanTest, integralFormat) {
    std::string fmt_ = "\%d";
    std::string_view fmt(fmt_);
    auto res1 = stdx::details::parse_value_with_format<int>(std::to_string(INT_MAX), fmt);
    EXPECT_EQ(*res1, INT_MAX);

    auto res2 = stdx::details::parse_value_with_format<int8_t>(std::to_string(INT8_MAX), fmt);
    EXPECT_EQ(*res2, INT8_MAX);

    auto res3 = stdx::details::parse_value_with_format<int16_t>(std::to_string(INT16_MAX), fmt);
    EXPECT_EQ(*res3, INT16_MAX);

    auto res4 = stdx::details::parse_value_with_format<int32_t>(std::to_string(INT32_MAX), fmt);
    EXPECT_EQ(*res4, INT32_MAX);

    auto res5 = stdx::details::parse_value_with_format<int64_t>(std::to_string(INT64_MAX), fmt);
    EXPECT_EQ(*res5, INT64_MAX);

    auto res6 = stdx::details::parse_value_with_format<unsigned int>(std::to_string(UINT_MAX), fmt);
    EXPECT_EQ(*res6, UINT_MAX);

    auto res7 = stdx::details::parse_value_with_format<uint8_t>(std::to_string(UINT8_MAX), fmt);
    EXPECT_EQ(*res7, UINT8_MAX);

    auto res8 = stdx::details::parse_value_with_format<uint16_t>(std::to_string(UINT16_MAX), fmt);
    EXPECT_EQ(*res8, UINT16_MAX);

    auto res9 = stdx::details::parse_value_with_format<uint32_t>(std::to_string(UINT32_MAX), fmt);
    EXPECT_EQ(*res9, UINT32_MAX);

    auto res10 = stdx::details::parse_value_with_format<uint64_t>(std::to_string(UINT64_MAX), fmt);
    EXPECT_EQ(*res10, UINT64_MAX);
}