#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "calc.hpp"

// Floating point numbers are now supported
TEST(Calc, fp1)
{
    std::string input{"0.3*3"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), 0.9);
}

TEST(Calc, fp2)
{
    std::string input{".6*.2"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), 0.12);
}

TEST(Calc, fp3)
{
    std::string input{"123.34 + 9435.3413456"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), 9558.6813456);
}

// Exponents are now supported
TEST(Calc, fiveSquared)
{
    std::string input{"5^2"};
    EXPECT_EQ(Calc::eval(input), 25);
}

TEST(Calc, sqrtNine)
{
    std::string input{"9^0.5"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), 3);
}

TEST(Calc, eightBit)
{
    std::string input{"2^8"};
    EXPECT_EQ(Calc::eval(input), 256);
}

// Basic trig is now supported, as an extension of changing the token val type to strings
TEST(Calc, sin1)
{
    std::string input{"asin(1)/pi"};
    EXPECT_FLOAT_EQ(Calc::eval(input), 0.5);
}

TEST(Calc, cos1)
{
    std::string input{"cos(pi)"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), -1);
}

TEST(Calc, eq)
{
    std::string input{"cos(pi/2)^2 + sin(pi/2)^2"};
    EXPECT_DOUBLE_EQ(Calc::eval(input), 1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}