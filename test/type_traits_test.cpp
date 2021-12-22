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

// is_integral
TEST(IsIntegral, Integrals) {
  EXPECT_TRUE(ft::is_integral<bool>::value);
  EXPECT_TRUE(ft::is_integral<char>::value);
  EXPECT_TRUE(ft::is_integral<wchar_t>::value);
  EXPECT_TRUE(ft::is_integral<short>::value);
  EXPECT_TRUE(ft::is_integral<int>::value);
  EXPECT_TRUE(ft::is_integral<long>::value);
  EXPECT_TRUE(ft::is_integral<long long>::value);
}

struct MyStruct {};

TEST(IsIntegral, NotIntegrals) {
  EXPECT_FALSE(ft::is_integral<void>::value);
  EXPECT_FALSE(ft::is_integral<float>::value);
  EXPECT_FALSE(ft::is_integral<double>::value);
  EXPECT_FALSE(ft::is_integral<char*>::value);
  EXPECT_FALSE(ft::is_integral<MyStruct>::value);
}

template <class T>
bool return_true_if_integral(
    T val,
    typename ft::enable_if<std::is_integral<T>::value>::type* = nullptr) {
  return true;
}
template <class T>
bool return_true_if_integral(
    T val,
    typename ft::disable_if<std::is_integral<T>::value>::type* = nullptr) {
  return false;
}

TEST(EnableIf, EnableIfTIsIntegral) {
  EXPECT_TRUE(return_true_if_integral<int>(10));
}

TEST(DisableIf, DisableIfIfTIsIntegral) {
  EXPECT_FALSE(return_true_if_integral<const char*>("abc"));
}

TEST(IsSame, TwoAreSame) {
  // テンプレート引数のカンマが関数マクロの引数の区切りと判定されるのを防ぐ為に括弧で囲む
  EXPECT_TRUE((ft::is_same<int, signed int>::value));
  EXPECT_TRUE((ft::is_same<int, std::int32_t>::value));
}

TEST(IsSame, TwoAreNotSame) {
  EXPECT_FALSE((ft::is_same<int, unsigned int>::value));
  EXPECT_FALSE((ft::is_same<char, int>::value));
}