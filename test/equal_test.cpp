#include "equal.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>

#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif

namespace {
// ±1 まで許容
template <class T>
bool isNearlyEqual(T& lhs, T& rhs) {
  return lhs - 1 <= rhs && lhs + 1 >= rhs;
}
}  // namespace

TEST(Equal, ReturnTrue) {
  typedef std::vector<int> vector_type;

  vector_type vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(3);
  vector_type vec2;
  vec2.push_back(1);
  vec2.push_back(2);
  vec2.push_back(3);
  vec2.push_back(4);

  EXPECT_TRUE(ft::equal(vec1.begin(), vec1.end(), vec2.begin()));
}

TEST(Equal, ReturnFalse) {
  typedef std::vector<int> vector_type;

  vector_type vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(3);
  vector_type vec2;
  vec2.push_back(2);
  vec2.push_back(3);
  vec2.push_back(4);
  vec2.push_back(5);

  EXPECT_FALSE(ft::equal(vec1.begin(), vec1.end(), vec2.begin()));
}

TEST(Equal, ReturnTrueWithCustomOperatorFunction) {
  typedef std::vector<int> vector_type;

  vector_type vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(3);
  vector_type vec2;
  vec2.push_back(2);
  vec2.push_back(1);
  vec2.push_back(3);
  EXPECT_TRUE(
      ft::equal(vec1.begin(), vec1.end(), vec2.begin(), &isNearlyEqual<int>));
}

TEST(Equal, ReturnFalseWithCustomOperatorFunction) {
  typedef std::vector<int> vector_type;

  vector_type vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(3);
  vector_type vec2;
  vec2.push_back(3);
  vec2.push_back(0);
  vec2.push_back(5);
  EXPECT_FALSE(
      ft::equal(vec1.begin(), vec1.end(), vec2.begin(), &isNearlyEqual<int>));
}