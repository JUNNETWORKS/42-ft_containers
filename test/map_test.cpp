#include "map.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

#include "pair.hpp"
#include "test_utils.hpp"
#include "utils/comparison.hpp"
#include "utils/my_allocator.hpp"

namespace {
const std::uint64_t kLoopMax = 1000;
}

TEST(DefaultConstructor, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  EXPECT_EQ(m.size(), map_type::size_type(0));
  EXPECT_EQ(m.begin(), m.end());
}

TEST(ConstructorWithCompareMethod, CustomizableComparison) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::test::less_or_greater<key_type> compare_type;
  typedef ft::map<key_type, mapped_type, compare_type> map_type;
  typedef map_type::iterator map_iterator;

  const compare_type lesscomp = compare_type(true);
  const compare_type greatercomp = compare_type(false);

  map_type less_map(lesscomp);
  map_type greater_map(greatercomp);

  for (char c = 'A'; c <= 'Z'; ++c) {
    std::string s(1, c);
    less_map[s] = 0;
    greater_map[s] = 0;
  }

  map_iterator it = less_map.begin();
  for (char c = 'A'; c <= 'Z' && it != less_map.end(); ++c, ++it) {
    EXPECT_EQ((*it).first[0], c);
  }
  EXPECT_EQ(it, less_map.end());

  it = greater_map.begin();
  for (char c = 'Z'; c >= 'A' && it != greater_map.end(); --c, ++it) {
    EXPECT_EQ((*it).first[0], c);
  }
  EXPECT_EQ(it, greater_map.end());
}

TEST(ConstructorWithIterator, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::iterator map_iterator;
  typedef std::vector<map_type::value_type> vector_type;
  typedef vector_type::iterator vector_iterator;

  vector_type vec;

  for (int c = 'A'; c <= 'Z'; ++c) {
    std::string s(1, c);
    vec.push_back(map_type::value_type(s, 0));
  }

  map_type m(vec.begin(), vec.end());

  map_iterator map_it = m.begin();
  vector_iterator vec_it = vec.begin();
  for (; vec_it != vec.end(); ++vec_it, ++map_it) {
    EXPECT_EQ((*map_it).first, (*vec_it).first);
  }
  EXPECT_EQ(map_it, m.end());
}

TEST(CopyConstructorAndAssignation, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m1;
  m1["A"] = 0;
  m1["B"] = 1;

  map_type m2(m1);
  m2["A"] = 1;
  m2["B"] = 2;

  map_type m3;
  m3 = m2;
  m3["A"] = 2;
  m3["B"] = 3;

  EXPECT_EQ(m1["A"], 0);
  EXPECT_EQ(m1["B"], 1);

  EXPECT_EQ(m2["A"], 1);
  EXPECT_EQ(m2["B"], 2);

  EXPECT_EQ(m3["A"], 2);
  EXPECT_EQ(m3["B"], 3);
}

TEST(GetAllocator, DefaultAllocator) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef std::allocator<pair_type> allocator_type;
  typedef ft::map<key_type, mapped_type, std::less<key_type>, allocator_type>
      map_type;

  map_type m;
  map_type::allocator_type allocator = m.get_allocator();
  EXPECT_TRUE(dynamic_cast<allocator_type*>(&allocator) != nullptr);
}

TEST(GetAllocator, CustomAllocator) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef ft::test::MyAllocator<pair_type> allocator_type;
  typedef ft::map<key_type, mapped_type, std::less<key_type>, allocator_type>
      map_type;

  map_type m;
  map_type::allocator_type allocator = m.get_allocator();
  EXPECT_TRUE(dynamic_cast<allocator_type*>(&allocator) != nullptr);
}

TEST(ElementAccess, NeedToThinkAboutWhatTestDo) {}

TEST(ForwardIterator, NeedToThinkAboutWhatTestDo) {}

TEST(ConstForwardIterator, NeedToThinkAboutWhatTestDo) {}

TEST(ReverseIterator, NeedToThinkAboutWhatTestDo) {}

TEST(ConstReverseIterator, NeedToThinkAboutWhatTestDo) {}

TEST(Capacity, NeedToThinkAboutWhatTestDo) {}

TEST(Clear, NeedToThinkAboutWhatTestDo) {}

TEST(Insert, NeedToThinkAboutWhatTestDo) {}

TEST(InsertWithHint, NeedToThinkAboutWhatTestDo) {}

TEST(InsertRange, NeedToThinkAboutWhatTestDo) {}

TEST(Erase, NeedToThinkAboutWhatTestDo) {}

TEST(Swap, NeedToThinkAboutWhatTestDo) {}

TEST(FindAndCount, NeedToThinkAboutWhatTestDo) {}

TEST(EqualRange, NeedToThinkAboutWhatTestDo) {}

TEST(LowerBound, NeedToThinkAboutWhatTestDo) {}

TEST(UpperBound, NeedToThinkAboutWhatTestDo) {}

TEST(Observers, NeedToThinkAboutWhatTestDo) {}

TEST(KeyComp, NeedToThinkAboutWhatTestDo) {}

TEST(Operators, NeedToThinkAboutWhatTestDo) {}

TEST(Random, RandomInsertAndDelete) {}

TEST(MyOriginalClass, Student) {
  typedef ft::test::Student Student;
  typedef Student::id_type key_type;
  typedef Student mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  for (std::uint64_t i = 0; i < kLoopMax; ++i) {
    Student::name_type name = ft::test::generate_random_string(12);
    Student student(name, std::rand() % UINT8_MAX);
    m[student.id_] = student;
  }
}