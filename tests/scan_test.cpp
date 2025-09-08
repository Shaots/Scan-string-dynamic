#include <climits>
#include <cmath>
#include <cstddef>
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
    EXPECT_EQ(res->get<0>(), 4);
    EXPECT_EQ(res->get<1>(), 15);
    EXPECT_EQ(res->get<2>(), -17);
    EXPECT_EQ(res->get<3>(), 15.6);
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
}

// 2 : Check integral format positive and negative
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

    auto res11 = stdx::details::parse_value_with_format<int>(std::to_string(INT_MIN), fmt);
    EXPECT_EQ(*res11, INT_MIN);

    auto res12 = stdx::details::parse_value_with_format<int8_t>(std::to_string(INT8_MIN), fmt);
    EXPECT_EQ(*res12, INT8_MIN);

    auto res13 = stdx::details::parse_value_with_format<int16_t>(std::to_string(INT16_MIN), fmt);
    EXPECT_EQ(*res13, INT16_MIN);

    auto res14 = stdx::details::parse_value_with_format<int32_t>(std::to_string(INT32_MIN), fmt);
    EXPECT_EQ(*res14, INT32_MIN);

    auto res15 = stdx::details::parse_value_with_format<int64_t>(std::to_string(INT64_MIN), fmt);
    EXPECT_EQ(*res15, INT64_MIN);
}

// 3 : Check float format
TEST(ScanTest, floatFormat) {
    std::string fmt_ = "\%f";
    std::string_view fmt(fmt_);
    std::string input("3.14159265359");
    auto res1 = stdx::details::parse_value_with_format<float>(input, fmt);
    EXPECT_NEAR(*res1, M_PI, 1e-5);

    auto res2 = stdx::details::parse_value_with_format<double>(input, fmt);
    EXPECT_NEAR(*res2, M_PI, 1e-10);
}

// 4 : Check string format
TEST(ScanTest, stringFormat) {
    std::string fmt_ = "\%s";
    std::string_view fmt(fmt_);
    std::string input("My name is Roman");
    auto res1 = stdx::details::parse_value_with_format<std::string>(input, fmt);
    EXPECT_EQ(*res1, input);

    auto res2 = stdx::details::parse_value_with_format<std::string_view>(input, fmt);
    EXPECT_EQ(*res2, input);
}

// ---------------------------------

// 6 : Scan string
TEST(ScanTest, scanString) {
    auto res = stdx::scan<std::string, std::string_view, std::string_view, std::string>(
        "ABC 04 DEFG 15 HIGK -17 LMN 15.6", "ABC {\%s} DEFG {\%s} HIGK {\%s} LMN {\%s}");
    EXPECT_EQ(res->get<0>(), "04");
    EXPECT_EQ(res->get<1>(), "15");
    EXPECT_EQ(res->get<2>(), "-17");
    EXPECT_EQ(res->get<3>(), "15.6");
    ASSERT_TRUE(res);
}

// 7 : Scan integer signed and unsigned
TEST(ScanTest, scanInteger) {
    auto res = stdx::scan<int, unsigned int, int8_t, uint16_t, int32_t, uint64_t>(
        "ABC -04 DEFG 15 HIGK -17 LMN 12345 OPQ -789654 RST 123987456",
        "ABC {\%d} DEFG {\%u} HIGK {\%d} LMN {\%u} OPQ {\%d} RST {\%u}");
    EXPECT_EQ(res->get<0>(), -4);
    EXPECT_EQ(res->get<1>(), 15);
    EXPECT_EQ(res->get<2>(), -17);
    EXPECT_EQ(res->get<3>(), 12345);
    EXPECT_EQ(res->get<4>(), -789654);
    EXPECT_EQ(res->get<5>(), 123987456);
    ASSERT_TRUE(res);
}

// 7 : Scan float positive and negative
TEST(ScanTest, scanFloat) {
    auto res = stdx::scan<double, double, double, double, float, float, float, float>(
        "ABC 0.123456789 DEFG -0.123456789 HIGK 15 LMN -16 OPQ 0.1234 RST -0.1234 UVW 789 XYZ 101112",
        "ABC {\%f} DEFG {\%f} HIGK {\%f} LMN {\%f} OPQ {\%f} RST {\%f} UVW {\%f} XYZ {\%f}");
    double d_eps = 1e-10;
    double f_eps = 1e-05;
    EXPECT_NEAR(res->get<0>(), 0.123456789, d_eps);
    EXPECT_NEAR(res->get<1>(), -0.123456789, d_eps);
    EXPECT_NEAR(res->get<2>(), 15, d_eps);
    EXPECT_NEAR(res->get<3>(), -16, d_eps);
    EXPECT_NEAR(res->get<4>(), 0.1234, f_eps);
    EXPECT_NEAR(res->get<5>(), -0.1234, f_eps);
    EXPECT_NEAR(res->get<6>(), 789, f_eps);
    EXPECT_NEAR(res->get<7>(), 101112, f_eps);
    ASSERT_TRUE(res);
}

// 8 : Scan without format
TEST(ScanTest, scanNoFormat) {
    auto res = stdx::scan<int, int, int, double>("ABC 04 DEFG 15 HIGK -17 LMN 15.6", "ABC {} DEFG {} HIGK {} LMN {}");
    EXPECT_EQ(res->get<0>(), 4);
    EXPECT_EQ(res->get<1>(), 15);
    EXPECT_EQ(res->get<2>(), -17);
    EXPECT_EQ(res->get<3>(), 15.6);
    ASSERT_TRUE(res);
}

// 9 : Scan mixed format
TEST(ScanTest, scanMixed) {
    auto res = stdx::scan<unsigned int, float, int, double, std::string>(
        "ABC 04 DEFG 15 HIGK -17 LMN 15.6 OPQ ABC", "ABC {\%u} DEFG {} HIGK {\%d} LMN {} OPQ {\%s}");
    EXPECT_EQ(res->get<0>(), 4);
    EXPECT_NEAR(res->get<1>(), 15, 1e-05);
    EXPECT_EQ(res->get<2>(), -17);
    EXPECT_NEAR(res->get<3>(), 15.6, 1e-10);
    EXPECT_EQ(res->get<4>(), "ABC");
    ASSERT_TRUE(res);
}

// 10 : Scan without placeholder
TEST(ScanTest, scanNoPlaceholder) {
    auto res = stdx::scan<>("ABC 04 DEFG 15 HIGK -17 LMN 15.6 OPQ ABC", "ABC 04 DEFG 15 HIGK -17 LMN 15.6 OPQ ABC");
    ASSERT_TRUE(res);
}

// ---------------------------------

// 11 : Unmatched format
TEST(ScanTest, errorUnmatchedFormat) {
    auto res = stdx::scan<int>("ABC 04 DEFG", "ABC {\%s} DEFG");
    ASSERT_FALSE(res.has_value());
}

// 12 : Long text Unmatched format
TEST(ScanTest, errorLongUnmatchedFormat) {
    auto res = stdx::scan<int, std::string, unsigned int, double>("ABC 04 DEFG 15 HIGK -17 LMN 15.6 OPQ ABC",
                                                                  "ABC {\%d} DEFG {\%s} HIGK {\%u} LMN {\%f} OPQ ABC");
    ASSERT_FALSE(res.has_value());
}

// 13 : Incorrect format
TEST(ScanTest, errorIncorrectFormat) {
    auto res = stdx::scan<double>("ABC 0.123456789 DEFG", "ABC {\%lf} DEFG");
    ASSERT_FALSE(res.has_value());
}

// 14 : Unmatched formatted string
TEST(ScanTest, errorUnmatchedFormattedString) {
    auto res = stdx::scan<double>("ABC 0.123456789 DEFG", "ABCD {\%f} DEFG");
    ASSERT_FALSE(res.has_value());
}

// ---------------------------------------
// The following code does not compile

// 15 : Byte
// TEST(ScanTest, byte) { auto res = stdx::scan<std::byte>("ABC 1 DEFG", "ABCD {\%f} DEFG"); }
