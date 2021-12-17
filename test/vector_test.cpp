#include "vector.hpp"

#include <gtest/gtest.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

class VectorTest : public ::testing::Test {
 protected:
  static const size_t kDefaultSize = 10;

  void SetUp() override {
    srand(time(NULL));

    for (size_t i = 0; i < kDefaultSize; ++i) {
      stl_vector.push_back(i);
      ft_vector.push_back(i);
    }
  }

  template <typename T>
  inline void expect_same_vector(std::vector<T> &stl_vector,
                                 ft::vector<T> &ft_vector) {
    typename std::vector<T>::iterator stl_it = stl_vector.begin();
    typename ft::vector<T>::iterator ft_it = ft_vector.begin();

    EXPECT_EQ(stl_vector.size(), ft_vector.size());
    EXPECT_EQ(stl_vector.max_size(), ft_vector.max_size());
    EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());
    EXPECT_EQ(stl_vector.empty(), ft_vector.empty());

    while (stl_it != stl_vector.end()) {
      EXPECT_EQ(*stl_it++, *ft_it++);
    }
    EXPECT_TRUE(ft_it == ft_vector.end());
  }

  std::vector<int> stl_vector;
  ft::vector<int> ft_vector;
  std::vector<int> stl_empty_vector;
  ft::vector<int> ft_empty_vector;

  std::vector<std::string> stl_str_vector;
  ft::vector<std::string> ft_str_vector;
};

TEST_F(VectorTest, DefaultConstructor) {
  std::vector<int> stl_vector;
  ft::vector<int> ft_vector;

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ConstructorWithOneNumericArg) {
  std::vector<int> stl_vector(10);
  ft::vector<int> ft_vector(10);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, CopyConstructor) {
  std::vector<int> stl_vector2(stl_vector);
  ft::vector<int> ft_vector2(ft_vector);

  stl_vector2.push_back(2);
  ft_vector2.push_back(2);

  expect_same_vector(stl_vector, ft_vector);
  expect_same_vector(stl_vector2, ft_vector2);
}

TEST_F(VectorTest, NormalIterator) {
  /* begin() can return iterator and const_iterator */
  std::vector<int>::iterator stl_it = stl_vector.begin();
  ft::vector<int>::iterator ft_it = ft_vector.begin();
  ASSERT_EQ(*stl_it, *ft_it);
  std::vector<int>::const_iterator const_stl_it = stl_vector.begin();
  ft::vector<int>::const_iterator const_ft_it = ft_vector.begin();
  ASSERT_EQ(*const_stl_it, *const_ft_it);

  /* for loop ends until it reach end() */
  const_stl_it = stl_vector.begin();
  const_ft_it = ft_vector.begin();
  for (; const_stl_it != stl_vector.end() && const_ft_it != ft_vector.end();
       const_stl_it++, const_ft_it++) {
  }

  stl_it = stl_vector.begin();
  ft_it = ft_vector.begin();
  /* iterator allow write value at *it */
  for (int i = 0; stl_it != stl_vector.end() && ft_it != ft_vector.end();
       ++i, ++stl_it, ++ft_it) {
    *stl_it = i;
    *ft_it = i;
  }

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ReverseIterator) {
  /* begin() can return iterator and const_iterator */
  std::vector<int>::reverse_iterator stl_rit = stl_vector.rbegin();
  ft::vector<int>::reverse_iterator ft_rit = ft_vector.rbegin();
  ASSERT_EQ(*stl_rit, *ft_rit);
  std::vector<int>::const_reverse_iterator const_stl_rit = stl_vector.rbegin();
  ft::vector<int>::const_reverse_iterator const_ft_rit = ft_vector.rbegin();
  ASSERT_EQ(*const_stl_rit, *const_ft_rit);

  /* for loop ends until it reach rend() */
  const_stl_rit = stl_vector.rbegin();
  const_ft_rit = ft_vector.rbegin();
  for (; const_stl_rit != stl_vector.rend() && const_ft_rit != ft_vector.rend();
       const_stl_rit++, const_ft_rit++) {
  }

  stl_rit = stl_vector.rbegin();
  ft_rit = ft_vector.rbegin();
  /* iterator allow write value at *it */
  for (int i = 0; stl_rit != stl_vector.rend() && ft_rit != ft_vector.rend();
       ++i, ++stl_rit, ++ft_rit) {
    *stl_rit = i;
    *ft_rit = i;
  }
  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, CapacityIncreaseByTheRuleOfDoubles) {
  for (int i = 0; i < 100; ++i) {
    stl_vector.push_back(i);
    ft_vector.push_back(i);

    EXPECT_EQ(stl_vector.size(), ft_vector.size());
    EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());
  }
}

TEST_F(VectorTest, ReserveDoesnotDoAnythingIfArgIsLessThanCurrentSize) {
  stl_vector.reserve(0);
  ft_vector.reserve(0);
  stl_vector.reserve(1);
  ft_vector.reserve(1);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ReserveIncreaseCapacityIfArgIsLargerThanCurrentSize) {
  stl_vector.reserve(100);
  ft_vector.reserve(100);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ReserveThrowExceptionIfArgIsLargerThanMaxSizeOfAllocator) {
  EXPECT_THROW(ft_vector.reserve(std::allocator<int>().max_size() + 1),
               std::length_error);
}

TEST_F(VectorTest, AccessElementBySquareBrackets) {
  for (int i = 0; i < stl_vector.size(); ++i) {
    EXPECT_EQ(stl_vector[i], ft_vector[i]);
  }
}

TEST_F(VectorTest, AccessElementByAt) {
  for (int i = 0; i < stl_vector.size(); ++i) {
    EXPECT_EQ(stl_vector.at(i), ft_vector.at(i));
  }
}

TEST_F(VectorTest, AtThrowOutOfRangeIfArgIsOutOfRange) {
  EXPECT_THROW(ft_vector.at(ft_vector.size()), std::out_of_range);
}

TEST_F(VectorTest, AssignWithIteratorOverCapacity) {
  // capacityを超えるとき
  std::vector<int> new_values(kDefaultSize * 2, 1);

  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());
  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignWithIteratorOverSize) {
  // sizeを超える時
  stl_vector.reserve(kDefaultSize * 10);
  ft_vector.reserve(kDefaultSize * 10);
  std::vector<int> new_values(kDefaultSize * 2, 1);
  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignWithIteratorNotOverCapacityAndSize) {
  // sizeもcapacityも超えない時
  std::vector<int> stl_vector(kDefaultSize * 10);
  ft::vector<int> ft_vector(kDefaultSize * 10);

  std::vector<int> new_values(kDefaultSize, 1);

  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignOverCapacity) {
  // capacityを超えるとき
  stl_vector.assign(kDefaultSize * 2, 2);
  ft_vector.assign(kDefaultSize * 2, 2);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignOverSize) {
  // capacityは超えないけどsizeを超える時
  stl_vector.reserve(kDefaultSize * 10);
  ft_vector.reserve(kDefaultSize * 10);

  stl_vector.assign(kDefaultSize * 2, 2);
  ft_vector.assign(kDefaultSize * 2, 2);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignNotOverCapacityAndSize) {
  // capacityもsizeも超えない
  stl_vector.assign(kDefaultSize / 2, 2);
  ft_vector.assign(kDefaultSize / 2, 2);

  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, PushBack) {
  // size() == capacity() になったら自動拡張される
  for (int i = 0; i < 16; ++i) {
    stl_vector.push_back(i);
    ft_vector.push_back(i);
  }
  expect_same_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, PopBack) {
  for (int i = 0; i < 16; ++i) {
    stl_vector.push_back(i);
    ft_vector.push_back(i);
  }
  for (int i = 0; i < 16; ++i) {
    stl_vector.pop_back();
    ft_vector.pop_back();
    expect_same_vector(stl_vector, ft_vector);
  }
}

TEST_F(VectorTest, Clear) {
  stl_vector.clear();
  ft_vector.clear();
  EXPECT_EQ(stl_vector.empty(), ft_vector.empty());
  expect_same_vector(stl_vector, ft_vector);
}
