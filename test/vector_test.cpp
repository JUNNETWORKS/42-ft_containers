#include "vector.hpp"

#include <gtest/gtest.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>

#include "debug_utils.hpp"

class VectorTest : public ::testing::Test {
 protected:
  typedef int value_type;
  typedef std::vector<value_type> stl_container;
  typedef ft::vector<value_type> ft_container;
  typedef typename stl_container::iterator stl_iterator;
  typedef typename ft_container::iterator ft_iterator;

  static const size_t kDefaultSize = 10;

  void SetUp() override {
    srand(time(NULL));

    for (size_t i = 0; i < kDefaultSize; ++i) {
      stl_vector.push_back(i);
      ft_vector.push_back(i);
    }
  }

  template <typename T>
  void expect_same_data_in_vector(std::vector<T>& stl_vector,
                                  ft::vector<T>& ft_vector) {
    typename std::vector<T>::iterator stl_it = stl_vector.begin();
    typename ft::vector<T>::iterator ft_it = ft_vector.begin();

    EXPECT_EQ(stl_vector.size(), ft_vector.size());
    EXPECT_EQ(stl_vector.max_size(), ft_vector.max_size());
    // capacityの初期値や増加ルールなどの厳密な値は仕様で定められていない
    // EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());
    EXPECT_EQ(stl_vector.empty(), ft_vector.empty());

    while (stl_it != stl_vector.end()) {
      EXPECT_EQ(*stl_it++, *ft_it++);
    }
    EXPECT_TRUE(ft_it == ft_vector.end());
  }

  stl_container stl_vector;
  ft_container ft_vector;
  stl_container stl_empty_vector;
  ft_container ft_empty_vector;

  std::vector<std::string> stl_str_vector;
  ft::vector<std::string> ft_str_vector;
};

TEST_F(VectorTest, DefaultConstructor) {
  stl_container stl_vector;
  ft_container ft_vector;

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ConstructorWithOneNumericArg) {
  stl_container stl_vector(10);
  ft_container ft_vector(10);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, CopyConstructor) {
  stl_container stl_vector2(stl_vector);
  ft_container ft_vector2(ft_vector);

  stl_vector2.push_back(2);
  ft_vector2.push_back(2);

  expect_same_data_in_vector(stl_vector, ft_vector);
  expect_same_data_in_vector(stl_vector2, ft_vector2);
}

TEST_F(VectorTest, NormalIterator) {
  /* begin() can return iterator and const_iterator */
  stl_container::iterator stl_it = stl_vector.begin();
  ft_container::iterator ft_it = ft_vector.begin();
  ASSERT_EQ(*stl_it, *ft_it);
  stl_container::const_iterator const_stl_it = stl_vector.begin();
  ft_container::const_iterator const_ft_it = ft_vector.begin();
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

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ReverseIterator) {
  /* begin() can return iterator and const_iterator */
  stl_container::reverse_iterator stl_rit = stl_vector.rbegin();
  ft_container::reverse_iterator ft_rit = ft_vector.rbegin();
  ASSERT_EQ(*stl_rit, *ft_rit);
  stl_container::const_reverse_iterator const_stl_rit = stl_vector.rbegin();
  ft_container::const_reverse_iterator const_ft_rit = ft_vector.rbegin();
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
  expect_same_data_in_vector(stl_vector, ft_vector);
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
  EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());
  stl_vector.reserve(1);
  ft_vector.reserve(1);
  EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ReserveIncreaseCapacityIfArgIsLargerThanCurrentSize) {
  stl_vector.reserve(100);
  ft_vector.reserve(100);
  EXPECT_EQ(stl_vector.capacity(), ft_vector.capacity());

  expect_same_data_in_vector(stl_vector, ft_vector);
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
  stl_container new_values(kDefaultSize * 2, 1);

  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignWithIteratorOverSize) {
  // sizeを超える時
  stl_vector.reserve(kDefaultSize * 10);
  ft_vector.reserve(kDefaultSize * 10);
  stl_container new_values(kDefaultSize * 2, 1);
  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignWithIteratorNotOverCapacityAndSize) {
  // sizeもcapacityも超えない時
  stl_container stl_vector(kDefaultSize * 10);
  ft_container ft_vector(kDefaultSize * 10);

  stl_container new_values(kDefaultSize, 1);

  stl_vector.assign(new_values.begin(), new_values.end());
  ft_vector.assign(new_values.begin(), new_values.end());

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignOverCapacity) {
  // capacityを超えるとき
  stl_vector.assign(kDefaultSize * 2, 2);
  ft_vector.assign(kDefaultSize * 2, 2);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

// TODO: Iteratorを受け取る関数のテストに
// RandomAccessIterator以外のイテレータのテストを追加する.
// InputIteratorが動けば他のiteratorは動くはずなのでInputIteratorのテストを追加する.

TEST_F(VectorTest, AssignOverSize) {
  // capacityは超えないけどsizeを超える時
  stl_vector.reserve(kDefaultSize * 10);
  ft_vector.reserve(kDefaultSize * 10);

  stl_vector.assign(kDefaultSize * 2, 2);
  ft_vector.assign(kDefaultSize * 2, 2);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, AssignNotOverCapacityAndSize) {
  // capacityもsizeも超えない
  stl_vector.assign(kDefaultSize / 2, 2);
  ft_vector.assign(kDefaultSize / 2, 2);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ResizeGreaterThanSize) {
  stl_vector.resize(stl_vector.size() * 2);
  ft_vector.resize(ft_vector.size() * 2);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ResizeEqualSize) {
  stl_vector.resize(stl_vector.size());
  ft_vector.resize(ft_vector.size());

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, ResizeLessThanSize) {
  stl_vector.resize(stl_vector.size() / 2);
  ft_vector.resize(ft_vector.size() / 2);

  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertOverCapacityAtFirst) {
  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.begin(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertOverCapacityAtMiddle) {
  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 10);
}

TEST_F(VectorTest, InsertOverCapacityAtLast) {
  // 最後に挿入
  stl_vector.insert(stl_vector.end(), stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.end(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertNotOverCapacityAtFirst) {
  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), 1, 10);
  ft_vector.insert(ft_vector.begin(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertNotOverCapacityAtMiddle) {
  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 1, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 10);
}

TEST_F(VectorTest, InsertNotOverCapacityAtLast) {
  // 最後に挿入
  stl_vector.insert(stl_vector.end(), 1, 10);
  ft_vector.insert(ft_vector.end(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertWithIteratorOverCapacityAtFirst) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  // 最初に挿入
  // 1回目のcapacity拡大は cap = size() + (last - first)
  stl_vector.insert(stl_vector.begin(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.begin(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);

  // 2回目以降のcapacity拡大は cap = cap * 2 になるっぽい??
  stl_vector.insert(stl_vector.begin(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.begin(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorOverCapacityAtMiddle) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    additional_values.begin(), additional_values.end());
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   additional_values.begin(), additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorOverCapacityAtLast) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.end(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorNotOverCapacityAtFirst) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  stl_vector.reserve(stl_vector.capacity() * 10);
  ft_vector.reserve(ft_vector.capacity() * 10);

  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.begin(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorNotOverCapacityAtMiddle) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  stl_vector.reserve(stl_vector.capacity() * 10);
  ft_vector.reserve(ft_vector.capacity() * 10);

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    additional_values.begin(), additional_values.end());
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   additional_values.begin(), additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorNotOverCapacityAtLast) {
  stl_container additional_values;
  for (int i = 0; i < stl_vector.capacity(); ++i) {
    additional_values.push_back(i);
  }

  stl_vector.reserve(stl_vector.capacity() * 10);
  ft_vector.reserve(ft_vector.capacity() * 10);

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.end(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertOverCapacity) {
  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.begin(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 10);

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), stl_vector.capacity(), 10);
  ft_vector.insert(ft_vector.end(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertNotOverCapacity) {
  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), 1, 10);
  ft_vector.insert(ft_vector.begin(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 1, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 10);

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), 1, 10);
  ft_vector.insert(ft_vector.end(), ft_vector.capacity(), 10);
  stl_vector.insert(stl_vector.begin(), 10);
  ft_vector.insert(ft_vector.begin(), 10);
}

TEST_F(VectorTest, InsertWithIteratorOverCapacity) {
  stl_container additional_values(stl_vector.capacity(), 10);

  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.begin(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);

  // 途中に挿入
  additional_values.assign(additional_values.size(), 11);
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    additional_values.begin(), additional_values.end());
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   additional_values.begin(), additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);

  // 最後に挿入
  additional_values.assign(additional_values.size(), 12);
  stl_vector.insert(stl_vector.end(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.end(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, InsertWithIteratorNotOverCapacity) {
  stl_vector.reserve(stl_vector.capacity() * 10);
  ft_vector.reserve(ft_vector.capacity() * 10);

  stl_container additional_values(1, 0);

  // 最初に挿入
  stl_vector.insert(stl_vector.begin(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.begin(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2,
                    additional_values.begin(), additional_values.end());
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2,
                   additional_values.begin(), additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), additional_values.begin(),
                    additional_values.end());
  ft_vector.insert(ft_vector.end(), additional_values.begin(),
                   additional_values.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithOneIteratorAtBegin) {
  stl_iterator stl_it = stl_vector.erase(stl_vector.begin());
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin());
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithOneIteratorAtMiddle) {
  stl_iterator stl_it =
      stl_vector.erase(stl_vector.begin() + stl_vector.size() / 2);
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin() + ft_vector.size() / 2);
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithOneIteratorAtLast) {
  stl_iterator stl_it = stl_vector.erase(stl_vector.end() - 1);
  ft_iterator ft_it = ft_vector.erase(ft_vector.end() - 1);
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithOneIteratorOnlyOneElement) {
  stl_container stl_vector(1);
  ft_container ft_vector(1);
  stl_iterator stl_it = stl_vector.erase(stl_vector.begin());
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin());

  EXPECT_EQ(stl_it == stl_vector.end(), ft_it == ft_vector.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithTwoIteratorBetweenBeginAndMiddle) {
  stl_iterator stl_it = stl_vector.erase(
      stl_vector.begin(), stl_vector.begin() + stl_vector.size() / 2);
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin(),
                                      ft_vector.begin() + ft_vector.size() / 2);
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithTwoIteratorBetweenMiddleAndMiddle) {
  stl_iterator stl_it =
      stl_vector.erase(stl_vector.begin() + 2, stl_vector.end() - 2);
  ft_iterator ft_it =
      ft_vector.erase(ft_vector.begin() + 2, ft_vector.end() - 2);
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithTwoIteratorBetweenMiddleAndLast) {
  stl_iterator stl_it = stl_vector.erase(
      stl_vector.begin() + stl_vector.size() / 2, stl_vector.end());
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin() + ft_vector.size() / 2,
                                      ft_vector.end());
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithTwoIteratorBetweenBeginAndLast) {
  stl_iterator stl_it = stl_vector.erase(
      stl_vector.begin() + stl_vector.size() / 2, stl_vector.end());
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin() + ft_vector.size() / 2,
                                      ft_vector.end());
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, PushBack) {
  // size() == capacity() になったら自動拡張される
  for (int i = 0; i < 16; ++i) {
    stl_vector.push_back(i);
    ft_vector.push_back(i);
  }
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, PopBack) {
  for (int i = 0; i < 16; ++i) {
    stl_vector.push_back(i);
    ft_vector.push_back(i);
  }
  for (int i = 0; i < 16; ++i) {
    stl_vector.pop_back();
    ft_vector.pop_back();
    expect_same_data_in_vector(stl_vector, ft_vector);
  }
}

TEST_F(VectorTest, Clear) {
  stl_vector.clear();
  ft_vector.clear();
  EXPECT_EQ(stl_vector.empty(), ft_vector.empty());
  expect_same_data_in_vector(stl_vector, ft_vector);
}