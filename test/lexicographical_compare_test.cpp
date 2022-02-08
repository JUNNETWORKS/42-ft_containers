#include "lexicographical_compare.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>

#include "testlib/testlib.hpp"

namespace {

typedef std::vector<int> container;

template <typename T>
bool compare(T a, T b) {
  return a < b;
}

TEST(LexicographicalCompare, FirstRangeIsEquivalentToSecondRange) {
  container vec1(10);
  container vec2(10);

  EXPECT_FALSE(ft::lexicographical_compare(vec1.begin(), vec1.end(),
                                           vec2.begin(), vec2.end()));
  EXPECT_FALSE(ft::lexicographical_compare(vec2.begin(), vec2.end(),
                                           vec1.begin(), vec1.end()));
}

TEST(LexicographicalCompare, FirstRangeIsLessThanSecondRange) {
  container vec1;
  container vec2;
  vec1.push_back(10);
  vec2.push_back(11);

  EXPECT_TRUE(ft::lexicographical_compare(vec1.begin(), vec1.end(),
                                          vec2.begin(), vec2.end()));
  EXPECT_FALSE(ft::lexicographical_compare(vec2.begin(), vec2.end(),
                                           vec1.begin(), vec1.end()));
}

TEST(LexicographicalCompare, FirstRangeLengthIsLessThanSecondRangeLength) {
  container vec1(1);
  container vec2(2);

  EXPECT_TRUE(ft::lexicographical_compare(vec1.begin(), vec1.end(),
                                          vec2.begin(), vec2.end()));
  EXPECT_FALSE(ft::lexicographical_compare(vec2.begin(), vec2.end(),
                                           vec1.begin(), vec1.end()));
}

TEST(LexicographicalCompare, FirstRangeLengthIsZero) {
  container vec1;
  container vec2(2);

  EXPECT_TRUE(ft::lexicographical_compare(vec1.begin(), vec1.end(),
                                          vec2.begin(), vec2.end()));
  EXPECT_FALSE(ft::lexicographical_compare(vec2.begin(), vec2.end(),
                                           vec1.begin(), vec1.end()));
}

TEST(LexicographicalCompareWithComparisonFunction,
     FirstRangeIsEquivalentToSecondRange) {
  container vec1(10);
  container vec2(10);

  EXPECT_FALSE(ft::lexicographical_compare(
      vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), compare<int>));
  EXPECT_FALSE(ft::lexicographical_compare(
      vec2.begin(), vec2.end(), vec1.begin(), vec1.end(), compare<int>));
}

TEST(LexicographicalCompareWithComparisonFunction,
     FirstRangeIsLessThanSecondRange) {
  container vec1;
  container vec2;
  vec1.push_back(10);
  vec2.push_back(11);

  EXPECT_TRUE(ft::lexicographical_compare(
      vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), compare<int>));
  EXPECT_FALSE(ft::lexicographical_compare(
      vec2.begin(), vec2.end(), vec1.begin(), vec1.end(), compare<int>));
}

TEST(LexicographicalCompareWithComparisonFunction,
     FirstRangeLengthIsLessThanSecondRangeLength) {
  container vec1(1);
  container vec2(2);

  EXPECT_TRUE(ft::lexicographical_compare(
      vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), compare<int>));
  EXPECT_FALSE(ft::lexicographical_compare(
      vec2.begin(), vec2.end(), vec1.begin(), vec1.end(), compare<int>));
}

TEST(LexicographicalCompareWithComparisonFunction, FirstRangeLengthIsZero) {
  container vec1;
  container vec2(2);

  EXPECT_TRUE(ft::lexicographical_compare(
      vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), compare<int>));
  EXPECT_FALSE(ft::lexicographical_compare(
      vec2.begin(), vec2.end(), vec1.begin(), vec1.end(), compare<int>));
}

}  // namespace