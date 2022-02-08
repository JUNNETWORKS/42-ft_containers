#include "pair.hpp"

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

TEST(Pair, DefaultConstructor) {
  ft::pair<std::string, int> student1;

  EXPECT_EQ(student1.first, "");
  EXPECT_EQ(student1.second, 0);
}

TEST(Pair, ConstructorWithValues) {
  ft::pair<std::string, int> student1("Jun", 20);

  EXPECT_EQ(student1.first, "Jun");
  EXPECT_EQ(student1.second, 20);
}

TEST(Pair, CopyConstructor) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2(student1);

  EXPECT_EQ(student2.first, student1.first);
  EXPECT_EQ(student2.second, student1.second);
}

TEST(Pair, CopyConstructorWithCast) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, double> student2(student1);

  EXPECT_EQ(student2.first, student1.first);
  EXPECT_EQ(student2.second, student1.second);
}

TEST(Pair, AssignmentOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Mike", 21);

  EXPECT_EQ(student2.first, "Mike");
  EXPECT_EQ(student2.second, 21);

  student2 = student1;

  EXPECT_EQ(student2.first, student1.first);
  EXPECT_EQ(student2.second, student1.second);
}

TEST(Pair, EqualOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_TRUE(student1 == student2);
  student2.second++;
  EXPECT_FALSE(student1 == student2);
}

TEST(Pair, NotEqualOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_FALSE(student1 != student2);
  student2.second++;
  EXPECT_TRUE(student1 != student2);
}

TEST(Pair, LessThanOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_FALSE(student1 < student2);
  student2.second++;
  EXPECT_TRUE(student1 < student2);  // ("Jun", 20) < ("Jun", 21)
  student1.first = "XYZ";
  EXPECT_FALSE(student1 < student2);  // ("XYZ", 20) < ("Jun", 21)
}

TEST(Pair, LessThanOrEqualOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_TRUE(student1 <= student2);
  student2.second++;
  EXPECT_TRUE(student1 <= student2);  // ("Jun", 20) <= ("Jun", 21)
  student1.first = "XYZ";
  EXPECT_FALSE(student1 <= student2);  // ("XYZ", 20) <= ("Jun", 21)
}

TEST(Pair, GreaterThanOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_FALSE(student1 > student2);
  student1.second++;
  EXPECT_TRUE(student1 > student2);  // ("Jun", 21) > ("Jun", 20)
  student2.first = "XYZ";
  EXPECT_FALSE(student1 > student2);  // ("Jun", 20) > ("XYZ", 20)
}

TEST(Pair, GreaterThanOrEqualOperator) {
  ft::pair<std::string, int> student1("Jun", 20);
  ft::pair<std::string, int> student2("Jun", 20);

  EXPECT_TRUE(student1 >= student2);
  student1.second++;
  EXPECT_TRUE(student1 >= student2);  // ("Jun", 21) >= ("Jun", 20)
  student2.first = "XYZ";
  EXPECT_FALSE(student1 >= student2);  // ("Jun", 21) >= ("XYZ", 20)
}

TEST(MakePair, MakePair) {
  ft::pair<std::string, int> student1 = ft::make_pair(std::string("Jun"), 20);

  EXPECT_EQ(student1.first, "Jun");
  EXPECT_EQ(student1.second, 20);
}