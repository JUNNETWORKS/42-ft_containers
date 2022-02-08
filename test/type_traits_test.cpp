// This test require C++11 or above

#include "type_traits.hpp"

#include <stdint.h>

#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif

/* std::is_const はC++11 以降 */

// remove_cv

TEST(RemoveCV, NoCVSpecifier) {
  EXPECT_FALSE(ft::is_const<ft::remove_cv<int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, ConstT) {
  EXPECT_FALSE(ft::is_const<ft::remove_cv<const int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, VolatileT) {
  EXPECT_FALSE(ft::is_const<ft::remove_cv<volatile int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_cv<int>::type>::value);
}

TEST(RemoveCV, ConstVolatileT) {
  EXPECT_FALSE(ft::is_const<ft::remove_cv<const volatile int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_cv<int>::type>::value);
}

// remove_const

TEST(RemoveConst, NoCVSpecifier) {
  EXPECT_FALSE(ft::is_const<ft::remove_const<int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_const<int>::type>::value);
}

TEST(RemoveConst, ConstT) {
  EXPECT_FALSE(ft::is_const<ft::remove_const<const int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_const<const int>::type>::value);
}

TEST(RemoveConst, VolatileT) {
  EXPECT_FALSE(ft::is_const<ft::remove_const<volatile int>::type>::value);
  EXPECT_TRUE(ft::is_volatile<ft::remove_const<volatile int>::type>::value);
}

TEST(RemoveConst, ConstTolatileT) {
  EXPECT_FALSE(ft::is_const<ft::remove_const<const volatile int>::type>::value);
  EXPECT_TRUE(
      ft::is_volatile<ft::remove_const<const volatile int>::type>::value);
}

// remove_volatile

TEST(RemoveVolatile, NoCVSpecifier) {
  EXPECT_FALSE(ft::is_const<ft::remove_volatile<int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_volatile<int>::type>::value);
}

TEST(RemoveVolatile, ConstT) {
  EXPECT_TRUE(ft::is_const<ft::remove_volatile<const int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_volatile<const int>::type>::value);
}

TEST(RemoveVolatile, VolatileT) {
  EXPECT_FALSE(ft::is_const<ft::remove_volatile<volatile int>::type>::value);
  EXPECT_FALSE(ft::is_volatile<ft::remove_volatile<volatile int>::type>::value);
}

TEST(RemoveVolatile, ConstTolatileT) {
  EXPECT_TRUE(
      ft::is_const<ft::remove_volatile<const volatile int>::type>::value);
  EXPECT_FALSE(
      ft::is_volatile<ft::remove_volatile<const volatile int>::type>::value);
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

// is_const
TEST(IsConst, Const) {
  EXPECT_TRUE(ft::is_const<const int>::value);
  EXPECT_TRUE(ft::is_const<const long long>::value);
  EXPECT_TRUE(ft::is_const<const std::string>::value);
  EXPECT_TRUE(ft::is_const<const MyStruct>::value);
}

TEST(IsConst, NotConst) {
  EXPECT_FALSE(ft::is_const<int>::value);
  EXPECT_FALSE(ft::is_const<long long>::value);
  EXPECT_FALSE(ft::is_const<std::string>::value);
  EXPECT_FALSE(ft::is_const<MyStruct>::value);
}

// is_volatile
TEST(IsVolatile, Volatile) {
  EXPECT_TRUE(ft::is_volatile<volatile int>::value);
  EXPECT_TRUE(ft::is_volatile<volatile long long>::value);
  EXPECT_TRUE(ft::is_volatile<volatile std::string>::value);
  EXPECT_TRUE(ft::is_volatile<volatile MyStruct>::value);
}

TEST(IsVolatile, NotVolatile) {
  EXPECT_FALSE(ft::is_volatile<int>::value);
  EXPECT_FALSE(ft::is_volatile<long long>::value);
  EXPECT_FALSE(ft::is_volatile<std::string>::value);
  EXPECT_FALSE(ft::is_volatile<MyStruct>::value);
}

namespace {
template <class T>
bool return_true_if_integral(
    T val, typename ft::enable_if<ft::is_integral<T>::value>::type* = NULL) {
  (void)val;
  return true;
}

template <class T>
bool return_true_if_integral(
    T val, typename ft::disable_if<ft::is_integral<T>::value>::type* = NULL) {
  (void)val;
  return false;
}
}  // namespace

TEST(EnableIf, EnableIfTIsIntegral) {
  EXPECT_TRUE(return_true_if_integral<int>(10));
}

TEST(DisableIf, DisableIfIfTIsIntegral) {
  EXPECT_FALSE(return_true_if_integral<const char*>("abc"));
}

TEST(IsSame, TwoAreSame) {
  // テンプレート引数のカンマが関数マクロの引数の区切りと判定されるのを防ぐ為に括弧で囲む
  EXPECT_TRUE((ft::is_same<int, signed int>::value));
  EXPECT_TRUE((ft::is_same<int, int32_t>::value));
}

TEST(IsSame, TwoAreNotSame) {
  EXPECT_FALSE((ft::is_same<int, unsigned int>::value));
  EXPECT_FALSE((ft::is_same<char, int>::value));
}