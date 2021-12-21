// This test require C++11 or above

#include "type_traits.hpp"

#include <gtest/gtest.h>

#include <type_traits>

// remove_cv

TEST(RemoveCV, NoCVSpecifier) {
  EXPECT_FALSE(std::is_const<ft::remove_cv<int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, ConstT) {
  EXPECT_FALSE(std::is_const<ft::remove_cv<const int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, VolatileT) {
  EXPECT_FALSE(std::is_const<ft::remove_cv<volatile int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, ConstVolatileT) {
  EXPECT_FALSE(std::is_const<ft::remove_cv<const volatile int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_cv<int>::type>::value);
}

// remove_const

TEST(RemoveConst, NoCVSpecifier) {
  EXPECT_FALSE(std::is_const<ft::remove_const<int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_const<int>::type>::value);
}

TEST(RemoveConst, ConstT) {
  EXPECT_FALSE(std::is_const<ft::remove_const<const int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_const<const int>::type>::value);
}

TEST(RemoveConst, VolatileT) {
  EXPECT_FALSE(std::is_const<ft::remove_const<volatile int>::type>::value);
  EXPECT_TRUE(std::is_volatile<ft::remove_const<volatile int>::type>::value);
}

TEST(RemoveConst, ConstTolatileT) {
  EXPECT_FALSE(
      std::is_const<ft::remove_const<const volatile int>::type>::value);
  EXPECT_TRUE(
      std::is_volatile<ft::remove_const<const volatile int>::type>::value);
}

// remove_volatile

TEST(RemoveVolatile, NoCVSpecifier) {
  EXPECT_FALSE(std::is_const<ft::remove_volatile<int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_volatile<int>::type>::value);
}

TEST(RemoveVolatile, ConstT) {
  EXPECT_TRUE(std::is_const<ft::remove_volatile<const int>::type>::value);
  EXPECT_FALSE(std::is_volatile<ft::remove_volatile<const int>::type>::value);
}

TEST(RemoveVolatile, VolatileT) {
  EXPECT_FALSE(std::is_const<ft::remove_volatile<volatile int>::type>::value);
  EXPECT_FALSE(
      std::is_volatile<ft::remove_volatile<volatile int>::type>::value);
}

TEST(RemoveVolatile, ConstTolatileT) {
  EXPECT_TRUE(
      std::is_const<ft::remove_volatile<const volatile int>::type>::value);
  EXPECT_FALSE(
      std::is_volatile<ft::remove_volatile<const volatile int>::type>::value);
}