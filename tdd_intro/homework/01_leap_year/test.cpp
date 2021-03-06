/*
Given a year, report if it is a leap year.

The tricky thing here is that a leap year in the Gregorian calendar occurs:

on every year that is evenly divisible by 4
  except every year that is evenly divisible by 100
    unless the year is also evenly divisible by 400

For example, 1997 is not a leap year, but 1996 is. 1900 is not a leap year, but 2000 is.

If your language provides a method in the standard library that does this look-up, pretend it doesn't exist and implement it yourself.
*/

#include <gtest/gtest.h>

bool LeapYear(int year)
{
    if (year%400 == 0)
    {
        return true;
    }
    if (year%100 == 0)
    {
        return false;
    }
    if (year%4 == 0)
    {
        return true;
    }
    return false;
}

TEST(LeapYear, DevidedBy4)
{
    ASSERT_TRUE(LeapYear(4));
}

TEST(LeapYear, DevidedBy100)
{
    ASSERT_FALSE(LeapYear(100));
}

TEST(LeapYear, DevidedBy400)
{
    ASSERT_TRUE(LeapYear(400));
}

TEST(LeapYear, RandomYearsTest)
{
    ASSERT_TRUE(LeapYear(2000));
    ASSERT_FALSE(LeapYear(1000));
    ASSERT_TRUE(LeapYear(1996));
    ASSERT_FALSE(LeapYear(1997));
    ASSERT_TRUE(LeapYear(2020));
    ASSERT_FALSE(LeapYear(2018));
    ASSERT_TRUE(LeapYear(-2000));
    ASSERT_FALSE(LeapYear(-1000));
}
