#include <gtest/gtest.h>
#include <print>
#include <string_view>

#include "scan.hpp"

TEST(ScanTest, SimpleTest) {
    auto res = stdx::scan<int, int, int, double>("ABC 04 DEFG 15 HIGK -17 LMN 15.6",
                                                 "ABC {\%d} DEFG {\%d} HIGK {\%d} LMN {\%f}");
    EXPECT_EQ(std::get<0>(res->result), 4);
    EXPECT_EQ(std::get<1>(res->result), 15);
    EXPECT_EQ(std::get<2>(res->result), -17);
    EXPECT_EQ(std::get<3>(res->result), 15.6);
    ASSERT_TRUE(res);
}

TEST(ScanTest, with_format) {
    std::string fmt_ = "\%d";
    std::string input_ = "78";
    std::string_view fmt(fmt_);
    std::string_view input(input_);
    stdx::details::parse_value_with_format<int>(input, fmt);
}