#include "vector.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>

#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif

#include "utils/debug_utils.hpp"
#include "utils/my_allocator.hpp"

namespace {

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

void add_nums_into_vector(std::vector<int>& stl_vec, ft::vector<int>& ft_vec,
                          size_t n = 100) {
  for (size_t i = 0; i < n; ++i) {
    stl_vec.push_back(i);
    ft_vec.push_back(i);
  }
}

}  // namespace

TEST(Vector, DefaultConstructor) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec;
  ft_vec_type ft_vec;
  expect_same_data_in_vector(stl_vec, ft_vec);
}

TEST(Vector, ConstructorWithValue) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec(10, 10);
  ft_vec_type ft_vec(10, 10);
  expect_same_data_in_vector(stl_vec, ft_vec);
}

TEST(Vector, ConstructorWithIterator) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  stl_vec_type stl_vec_for_copy;
  ft_vec_type ft_vec_for_copy;
  add_nums_into_vector(stl_vec_for_copy, ft_vec_for_copy);

  stl_vec_type stl_vec1(stl_vec_for_copy.begin(), stl_vec_for_copy.begin());
  ft_vec_type ft_vec1(ft_vec_for_copy.begin(), ft_vec_for_copy.begin());
  expect_same_data_in_vector(stl_vec1, ft_vec1);

  stl_vec_type stl_vec2(ft_vec_for_copy.begin(), ft_vec_for_copy.begin());
  ft_vec_type ft_vec2(stl_vec_for_copy.begin(), stl_vec_for_copy.begin());
  expect_same_data_in_vector(stl_vec2, ft_vec2);

  stl_vec_type stl_vec3(stl_empty_vec.begin(), stl_empty_vec.end());
  ft_vec_type ft_vec3(ft_empty_vec.begin(), ft_empty_vec.end());
  expect_same_data_in_vector(stl_vec3, ft_vec3);
}

TEST(Vector, CopyConstructor) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  stl_vec_type stl_vec_for_copy;
  ft_vec_type ft_vec_for_copy;
  add_nums_into_vector(stl_vec_for_copy, ft_vec_for_copy);

  stl_vec_type stl_vec1(stl_vec_for_copy);
  ft_vec_type ft_vec1(ft_vec_for_copy);
  expect_same_data_in_vector(stl_vec1, ft_vec1);

  stl_vec_type stl_vec2(stl_empty_vec);
  ft_vec_type ft_vec2(ft_empty_vec);
  expect_same_data_in_vector(stl_vec2, ft_vec2);
}

TEST(Vector, Destructor) {
  typedef int value_type;
  typedef std::vector<value_type> stl_1d_vec_type;
  typedef ft::vector<value_type> ft_1d_vec_type;
  typedef std::vector<stl_1d_vec_type> stl_2d_vec_type;
  typedef ft::vector<ft_1d_vec_type> ft_2d_vec_type;

  stl_2d_vec_type stl_2d_vec;
  ft_2d_vec_type ft_2d_vec;

  for (int i = 0; i < 10; ++i) {
    stl_1d_vec_type stl_tmp_vec;
    ft_1d_vec_type ft_tmp_vec;
    add_nums_into_vector(stl_tmp_vec, ft_tmp_vec, 10);

    stl_2d_vec.push_back(stl_tmp_vec);
    ft_2d_vec.push_back(ft_tmp_vec);
  }

  stl_2d_vec_type::iterator stl_2d_it = stl_2d_vec.begin();
  ft_2d_vec_type::iterator ft_2d_it = ft_2d_vec.begin();
  for (; stl_2d_it != stl_2d_vec.end() && ft_2d_it != ft_2d_vec.end();
       ++stl_2d_it, ++ft_2d_it) {
    stl_1d_vec_type::iterator stl_1d_it = (*stl_2d_it).begin();
    ft_1d_vec_type::iterator ft_1d_it = (*ft_2d_it).begin();
    for (; stl_1d_it != (*stl_2d_it).end() && ft_1d_it != (*ft_2d_it).end();
         ++stl_1d_it, ++ft_1d_it) {
      EXPECT_EQ(*stl_1d_it, *ft_1d_it);
    }
  }

  // デストラクタが正しく内部データを破棄していない場合はメモリリークが発生する
}

TEST(Vector, AssignmentOperator) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  stl_vec_type stl_vec_for_copy;
  ft_vec_type ft_vec_for_copy;
  add_nums_into_vector(stl_vec_for_copy, ft_vec_for_copy);

  stl_vec_type stl_vec1;
  ft_vec_type ft_vec1;
  stl_vec1 = stl_vec_for_copy;
  ft_vec1 = ft_vec_for_copy;
  expect_same_data_in_vector(stl_vec1, ft_vec1);

  stl_vec_type stl_vec2;
  ft_vec_type ft_vec2;
  stl_vec2 = stl_empty_vec;
  ft_vec2 = ft_empty_vec;
  expect_same_data_in_vector(stl_vec2, ft_vec2);
}

TEST(Vector, assign) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  stl_vec_type stl_vec_for_copy;
  ft_vec_type ft_vec_for_copy;
  add_nums_into_vector(stl_vec_for_copy, ft_vec_for_copy);

  stl_vec_type stl_vec1(0, 100);
  ft_vec_type ft_vec1(0, 100);
  stl_vec1.assign(100, 100);
  ft_vec1.assign(100, 100);
  expect_same_data_in_vector(stl_vec1, ft_vec1);

  stl_vec_type stl_vec2(0, 100);
  ft_vec_type ft_vec2(0, 100);
  stl_vec2.assign(stl_vec_for_copy.begin(), stl_vec_for_copy.end());
  ft_vec2.assign(ft_vec_for_copy.begin(), ft_vec_for_copy.end());
  expect_same_data_in_vector(stl_vec2, ft_vec2);

  stl_vec_type stl_vec3(0, 100);
  ft_vec_type ft_vec3(0, 100);
  stl_vec3.assign(stl_empty_vec.begin(), stl_empty_vec.end());
  ft_vec3.assign(ft_empty_vec.begin(), ft_empty_vec.end());
  expect_same_data_in_vector(stl_vec3, ft_vec3);
}

TEST(Vector, get_allocator) {
  typedef int value_type;
  typedef ft::test::MyAllocator<value_type> allocator_type;
  typedef ft::vector<value_type, allocator_type> ft_vec_type;

  ft_vec_type ft_vec1;
  ft_vec_type::allocator_type allocator = ft_vec1.get_allocator();
  EXPECT_TRUE(dynamic_cast<allocator_type*>(&allocator) != NULL);
}

TEST(Vector, ElementAccess) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  stl_vec_type stl_vec;
  ft_vec_type ft_vec;
  add_nums_into_vector(stl_vec, ft_vec, 10);

  // at
  EXPECT_EQ(stl_vec.at(0), ft_vec.at(0));
  EXPECT_EQ(stl_vec.at(1), ft_vec.at(1));
  EXPECT_EQ(stl_vec.at(9), ft_vec.at(9));
  EXPECT_THROW(stl_vec.at(100), std::out_of_range);
  EXPECT_THROW(ft_vec.at(100), std::out_of_range);
  EXPECT_THROW(stl_empty_vec.at(0), std::out_of_range);
  EXPECT_THROW(ft_empty_vec.at(0), std::out_of_range);

  // operator[]
  EXPECT_EQ(stl_vec[0], ft_vec[0]);
  EXPECT_EQ(stl_vec[1], ft_vec[1]);
  EXPECT_EQ(stl_vec[9], ft_vec[9]);

  // front
  EXPECT_EQ(stl_vec.front(), ft_vec.front());

  // back
  EXPECT_EQ(stl_vec.back(), ft_vec.back());

  // data
  EXPECT_EQ(stl_vec.data()[0], ft_vec.data()[0]);
  EXPECT_EQ(stl_vec.data()[1], ft_vec.data()[1]);
  EXPECT_EQ(stl_vec.data()[9], ft_vec.data()[9]);
}

TEST(Vector, ForwardIterator) {
  typedef std::string value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  EXPECT_EQ(stl_empty_vec.begin(), stl_empty_vec.end());
  EXPECT_EQ(ft_empty_vec.begin(), ft_empty_vec.end());

  stl_vec_type stl_vec;
  ft_vec_type ft_vec;
  for (int i = 0; i < 10; ++i) {
    stl_vec.push_back(std::string(10, 'A' + i));
    ft_vec.push_back(std::string(10, 'A' + i));
  }

  // iterate forward
  stl_vec_type::iterator stl_it = stl_vec.begin();
  ft_vec_type::iterator ft_it = ft_vec.begin();
  for (; stl_it != stl_vec.begin() && ft_it != ft_vec.end();
       ++stl_it, ++ft_it) {
    EXPECT_EQ(*stl_it, *ft_it);
  }

  // iterator operations

  // copy constructive
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  EXPECT_EQ(*(stl_vec_type::iterator(stl_it)), *stl_it);
  EXPECT_EQ(*(ft_vec_type::iterator(ft_it)), *ft_it);

  // can be incremented
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  ++stl_it;
  ++ft_it;
  stl_it++;
  ft_it++;
  EXPECT_EQ(*stl_it, *ft_it);

  // Supports equality/inequality comparisons
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();

  stl_vec_type::iterator stl_it2 = stl_vec.begin();
  ft_vec_type::iterator ft_it2 = ft_vec.begin();
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);
  stl_it2++;
  ft_it2++;
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);
  ++stl_it;
  ++ft_it;
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);

  // Can be dereferenced as an rvalue
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  value_type stl_val = *stl_it;
  value_type ft_val = *ft_it;
  EXPECT_EQ(stl_val, ft_val);
  char stl_char = stl_it->at(0);
  char ft_char = ft_it->at(0);
  EXPECT_EQ(stl_char, ft_char);

  // Can be dereferenced as an lvalue
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  *stl_it = "HOGE";
  *ft_it = "HOGE";
  EXPECT_EQ(*stl_it, *ft_it);
  *stl_it++ = "FUGA";
  *ft_it++ = "FUGA";
  EXPECT_EQ(*stl_it, *ft_it);

  // default-constructible
  stl_it2 = stl_vec.begin();
  ft_it2 = ft_vec.begin();
  EXPECT_EQ(*stl_it2, *ft_it2);

  // Multi-pass
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it2 = stl_it;
  ft_it2 = ft_it;
  EXPECT_EQ(*stl_it++, *ft_it++);
  EXPECT_EQ(*stl_it2, *ft_it2);

  // Can be decremented
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it++;
  ft_it++;
  stl_it++;
  ft_it++;
  EXPECT_EQ(*stl_it--, *ft_it--);
  EXPECT_EQ(*--stl_it, *--ft_it);

  // Supports arithmetic operators + and -
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it2 = stl_it + 5;
  ft_it2 = ft_it + 5;
  EXPECT_EQ(*stl_it2, *ft_it2);
  stl_it2 = 1 + stl_it2;
  ft_it2 = 1 + ft_it2;
  EXPECT_EQ(*stl_it2, *ft_it2);
  stl_it2 = stl_it2 - 1;
  ft_it2 = ft_it2 - 1;
  EXPECT_EQ(*stl_it2, *ft_it2);
  EXPECT_EQ(stl_it2 - stl_it, ft_it2 - ft_it);

  // Supports inequality comparisons between iterators
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it2 = stl_vec.begin();
  ft_it2 = ft_vec.begin();
  EXPECT_EQ(stl_it > stl_it2, ft_it > ft_it2);
  EXPECT_EQ(stl_it < stl_it2, ft_it < ft_it2);
  EXPECT_EQ(stl_it >= stl_it2, ft_it >= ft_it2);
  EXPECT_EQ(stl_it <= stl_it2, ft_it <= ft_it2);
  stl_it2++;
  ft_it2++;
  EXPECT_EQ(stl_it > stl_it2, ft_it > ft_it2);
  EXPECT_EQ(stl_it < stl_it2, ft_it < ft_it2);
  EXPECT_EQ(stl_it >= stl_it2, ft_it >= ft_it2);
  EXPECT_EQ(stl_it <= stl_it2, ft_it <= ft_it2);

  // Supports compound assignment operations += and -=
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it += 2;
  ft_it += 2;
  EXPECT_EQ(*stl_it, *ft_it);

  // Supports offset dereference operator ([])
  stl_it = stl_vec.begin();
  ft_it = ft_vec.begin();
  stl_it += 2;
  ft_it += 2;
  EXPECT_EQ(stl_it[2], ft_it[2]);
}

TEST(Vector, ReverseIterator) {
  typedef std::string value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_empty_vec;
  ft_vec_type ft_empty_vec;
  EXPECT_EQ(stl_empty_vec.rbegin(), stl_empty_vec.rend());
  EXPECT_EQ(ft_empty_vec.rbegin(), ft_empty_vec.rend());

  stl_vec_type stl_vec;
  ft_vec_type ft_vec;
  for (int i = 0; i < 10; ++i) {
    stl_vec.push_back(std::string(10, 'A' + i));
    ft_vec.push_back(std::string(10, 'A' + i));
  }

  // iterate forward
  stl_vec_type::reverse_iterator stl_it = stl_vec.rbegin();
  ft_vec_type::reverse_iterator ft_it = ft_vec.rbegin();
  for (; stl_it != stl_vec.rbegin() && ft_it != ft_vec.rend();
       ++stl_it, ++ft_it) {
    EXPECT_EQ(*stl_it, *ft_it);
  }

  // iterator operations

  // copy constructive
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  EXPECT_EQ(*(stl_vec_type::reverse_iterator(stl_it)), *stl_it);
  EXPECT_EQ(*(ft_vec_type::reverse_iterator(ft_it)), *ft_it);

  // can be incremented
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  ++stl_it;
  ++ft_it;
  stl_it++;
  ft_it++;
  EXPECT_EQ(*stl_it, *ft_it);

  // Supports equality/inequality comparisons
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();

  stl_vec_type::reverse_iterator stl_it2 = stl_vec.rbegin();
  ft_vec_type::reverse_iterator ft_it2 = ft_vec.rbegin();
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);
  stl_it2++;
  ft_it2++;
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);
  ++stl_it;
  ++ft_it;
  EXPECT_EQ(stl_it == stl_it2, ft_it == ft_it2);
  EXPECT_EQ(stl_it != stl_it2, ft_it != ft_it2);

  // Can be dereferenced as an rvalue
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  value_type stl_val = *stl_it;
  value_type ft_val = *ft_it;
  EXPECT_EQ(stl_val, ft_val);
  char stl_char = stl_it->at(0);
  char ft_char = ft_it->at(0);
  EXPECT_EQ(stl_char, ft_char);

  // Can be dereferenced as an lvalue
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  *stl_it = "HOGE";
  *ft_it = "HOGE";
  EXPECT_EQ(*stl_it, *ft_it);
  *stl_it++ = "FUGA";
  *ft_it++ = "FUGA";
  EXPECT_EQ(*stl_it, *ft_it);

  // default-constructible
  stl_it2 = stl_vec.rbegin();
  ft_it2 = ft_vec.rbegin();
  EXPECT_EQ(*stl_it2, *ft_it2);

  // Multi-pass
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it2 = stl_it;
  ft_it2 = ft_it;
  EXPECT_EQ(*stl_it++, *ft_it++);
  EXPECT_EQ(*stl_it2, *ft_it2);

  // Can be decremented
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it++;
  ft_it++;
  stl_it++;
  ft_it++;
  EXPECT_EQ(*stl_it--, *ft_it--);
  EXPECT_EQ(*--stl_it, *--ft_it);

  // Supports arithmetic operators + and -
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it2 = stl_it + 5;
  ft_it2 = ft_it + 5;
  EXPECT_EQ(*stl_it2, *ft_it2);
  stl_it2 = 1 + stl_it2;
  ft_it2 = 1 + ft_it2;
  EXPECT_EQ(*stl_it2, *ft_it2);
  stl_it2 = stl_it2 - 1;
  ft_it2 = ft_it2 - 1;
  EXPECT_EQ(*stl_it2, *ft_it2);
  EXPECT_EQ(stl_it2 - stl_it, ft_it2 - ft_it);

  // Supports inequality comparisons between iterators
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it2 = stl_vec.rbegin();
  ft_it2 = ft_vec.rbegin();
  EXPECT_EQ(stl_it > stl_it2, ft_it > ft_it2);
  EXPECT_EQ(stl_it < stl_it2, ft_it < ft_it2);
  EXPECT_EQ(stl_it >= stl_it2, ft_it >= ft_it2);
  EXPECT_EQ(stl_it <= stl_it2, ft_it <= ft_it2);
  stl_it2++;
  ft_it2++;
  EXPECT_EQ(stl_it > stl_it2, ft_it > ft_it2);
  EXPECT_EQ(stl_it < stl_it2, ft_it < ft_it2);
  EXPECT_EQ(stl_it >= stl_it2, ft_it >= ft_it2);
  EXPECT_EQ(stl_it <= stl_it2, ft_it <= ft_it2);

  // Supports compound assignment operations += and -=
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it += 2;
  ft_it += 2;
  EXPECT_EQ(*stl_it, *ft_it);

  // Supports offset dereference operator ([])
  stl_it = stl_vec.rbegin();
  ft_it = ft_vec.rbegin();
  stl_it += 2;
  ft_it += 2;
  EXPECT_EQ(stl_it[2], ft_it[2]);
}

TEST(Vector, Capacity) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec;
  ft_vec_type ft_vec;

  EXPECT_EQ(stl_vec.empty(), ft_vec.empty());
  EXPECT_EQ(stl_vec.size(), ft_vec.size());
  // max_size() は実装や環境により異なるので
  // STLと同じ値になることを重要視していない
  EXPECT_TRUE(stl_vec.max_size());
  EXPECT_TRUE(ft_vec.max_size());
  EXPECT_EQ(stl_vec.capacity(), ft_vec.capacity());

  stl_vec.reserve(10);
  ft_vec.reserve(10);
  EXPECT_EQ(stl_vec.capacity(), ft_vec.capacity());

  for (int i = 0; i < 100; ++i) {
    stl_vec.push_back(i);
    ft_vec.push_back(i);
  }
  EXPECT_EQ(stl_vec.empty(), ft_vec.empty());
  EXPECT_EQ(stl_vec.size(), ft_vec.size());
  EXPECT_EQ(stl_vec.capacity(), ft_vec.capacity());
}

TEST(Vector, Modifiers) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec_for_copy;
  for (int i = 0; i < 10; ++i) {
    stl_vec_for_copy.push_back(i);
  }

  stl_vec_type stl_vec;
  ft_vec_type ft_vec;

  // push_back
  for (int i = 0; i < 10; ++i) {
    stl_vec.push_back(i);
    ft_vec.push_back(i);
  }
  expect_same_data_in_vector(stl_vec, ft_vec);
  // pop_back
  for (int i = 0; i < 10; ++i) {
    stl_vec.pop_back();
    ft_vec.pop_back();
  }
  expect_same_data_in_vector(stl_vec, ft_vec);

  // clear
  add_nums_into_vector(stl_vec, ft_vec);
  stl_vec.clear();
  ft_vec.clear();
  expect_same_data_in_vector(stl_vec, ft_vec);

  // insert
  stl_vec_type::iterator stl_it = stl_vec.begin();
  ft_vec_type::iterator ft_it = ft_vec.begin();
  stl_it = stl_vec.insert(stl_it, 0);
  ft_it = ft_vec.insert(ft_it, 0);
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_it = stl_vec.end();
  ft_it = ft_vec.end();
  stl_it = stl_vec.insert(stl_it, 1);
  ft_it = ft_vec.insert(ft_it, 1);
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_vec.insert(stl_vec.begin(), 10, 10);
  ft_vec.insert(ft_vec.begin(), 10, 10);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_vec.insert(stl_vec.begin(), stl_vec_for_copy.begin(),
                 stl_vec_for_copy.end());
  ft_vec.insert(ft_vec.begin(), stl_vec_for_copy.begin(),
                stl_vec_for_copy.end());
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_vec.insert(stl_vec.end(), stl_vec_for_copy.begin(),
                 stl_vec_for_copy.end());
  ft_vec.insert(ft_vec.end(), stl_vec_for_copy.begin(), stl_vec_for_copy.end());
  expect_same_data_in_vector(stl_vec, ft_vec);

  // erase
  stl_it = stl_vec.erase(stl_vec.begin());
  ft_it = ft_vec.erase(ft_vec.begin());
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_it = stl_vec.erase(stl_vec.end() - 1);
  ft_it = ft_vec.erase(ft_vec.end() - 1);
  EXPECT_EQ(stl_it, stl_vec.end());
  EXPECT_EQ(ft_it, ft_vec.end());
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_it = stl_vec.erase(stl_vec.begin(), stl_vec.begin() + stl_vec.size() / 2);
  ft_it = ft_vec.erase(ft_vec.begin(), ft_vec.begin() + ft_vec.size() / 2);
  EXPECT_EQ(*stl_it, *ft_it);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_it = stl_vec.erase(stl_vec.begin() + stl_vec.size() / 2, stl_vec.end());
  ft_it = ft_vec.erase(ft_vec.begin() + ft_vec.size() / 2, ft_vec.end());
  EXPECT_EQ(stl_it, stl_vec.end());
  EXPECT_EQ(ft_it, ft_vec.end());
  expect_same_data_in_vector(stl_vec, ft_vec);

  // resize
  stl_vec.clear();
  ft_vec.clear();
  add_nums_into_vector(stl_vec, ft_vec, 10);

  stl_vec.resize(5);
  ft_vec.resize(5);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_vec.resize(10);
  ft_vec.resize(10);
  expect_same_data_in_vector(stl_vec, ft_vec);

  stl_vec.resize(100, 100);
  ft_vec.resize(100, 100);
  expect_same_data_in_vector(stl_vec, ft_vec);
}

TEST(Vector, NonMemberFunctions) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec1;
  stl_vec_type stl_vec2;
  ft_vec_type ft_vec1;
  ft_vec_type ft_vec2;

  EXPECT_EQ(stl_vec1 == stl_vec2, ft_vec1 == ft_vec2);
  EXPECT_EQ(stl_vec1 != stl_vec2, ft_vec1 != ft_vec2);
  EXPECT_EQ(stl_vec1 < stl_vec2, ft_vec1 < ft_vec2);
  EXPECT_EQ(stl_vec1 <= stl_vec2, ft_vec1 <= ft_vec2);
  EXPECT_EQ(stl_vec1 >= stl_vec2, ft_vec1 >= ft_vec2);
  EXPECT_EQ(stl_vec1 > stl_vec2, ft_vec1 > ft_vec2);

  stl_vec1.push_back(1);
  ft_vec1.push_back(1);
  EXPECT_EQ(stl_vec1 == stl_vec2, ft_vec1 == ft_vec2);
  EXPECT_EQ(stl_vec1 != stl_vec2, ft_vec1 != ft_vec2);
  EXPECT_EQ(stl_vec1 < stl_vec2, ft_vec1 < ft_vec2);
  EXPECT_EQ(stl_vec1 <= stl_vec2, ft_vec1 <= ft_vec2);
  EXPECT_EQ(stl_vec1 >= stl_vec2, ft_vec1 >= ft_vec2);
  EXPECT_EQ(stl_vec1 > stl_vec2, ft_vec1 > ft_vec2);

  stl_vec2.push_back(1);
  ft_vec2.push_back(1);
  EXPECT_EQ(stl_vec1 == stl_vec2, ft_vec1 == ft_vec2);
  EXPECT_EQ(stl_vec1 != stl_vec2, ft_vec1 != ft_vec2);
  EXPECT_EQ(stl_vec1 < stl_vec2, ft_vec1 < ft_vec2);
  EXPECT_EQ(stl_vec1 <= stl_vec2, ft_vec1 <= ft_vec2);
  EXPECT_EQ(stl_vec1 >= stl_vec2, ft_vec1 >= ft_vec2);
  EXPECT_EQ(stl_vec1 > stl_vec2, ft_vec1 > ft_vec2);

  stl_vec1.push_back(2);
  ft_vec1.push_back(2);
  stl_vec2.push_back(3);
  ft_vec2.push_back(3);
  EXPECT_EQ(stl_vec1 == stl_vec2, ft_vec1 == ft_vec2);
  EXPECT_EQ(stl_vec1 != stl_vec2, ft_vec1 != ft_vec2);
  EXPECT_EQ(stl_vec1 < stl_vec2, ft_vec1 < ft_vec2);
  EXPECT_EQ(stl_vec1 <= stl_vec2, ft_vec1 <= ft_vec2);
  EXPECT_EQ(stl_vec1 >= stl_vec2, ft_vec1 >= ft_vec2);
  EXPECT_EQ(stl_vec1 > stl_vec2, ft_vec1 > ft_vec2);
}

TEST(Vector, swap) {
  typedef int value_type;
  typedef std::vector<value_type> stl_vec_type;
  typedef ft::vector<value_type> ft_vec_type;

  stl_vec_type stl_vec1(10, 10);
  stl_vec_type stl_vec2(2, 2);
  stl_vec1.swap(stl_vec2);
  ft_vec_type ft_vec1(10, 10);
  ft_vec_type ft_vec2(2, 2);
  ft_vec1.swap(ft_vec2);
  expect_same_data_in_vector(stl_vec1, ft_vec1);
  expect_same_data_in_vector(stl_vec2, ft_vec2);

  std::swap(stl_vec1, stl_vec2);
  std::swap(ft_vec1, ft_vec2);
  expect_same_data_in_vector(stl_vec1, ft_vec1);
  expect_same_data_in_vector(stl_vec2, ft_vec2);
}

class VectorTest : public ::testing::Test {
 protected:
  typedef int value_type;
  typedef std::vector<value_type> stl_container;
  typedef ft::vector<value_type> ft_container;
  typedef stl_container::iterator stl_iterator;
  typedef ft_container::iterator ft_iterator;

  static const size_t kDefaultSize = 10;

  virtual void SetUp() {
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

TEST_F(VectorTest, AssignmentOperator) {
  ft_container ft_vector2;

  ft_vector2 = ft_vector;
  ft_vector.push_back(1);
  ft_vector2.push_back(2);
  EXPECT_EQ(ft_vector.back(), 1);
  EXPECT_EQ(ft_vector2.back(), 2);
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
  for (stl_container::size_type i = 0; i < stl_vector.size(); ++i) {
    EXPECT_EQ(stl_vector[i], ft_vector[i]);
  }
}

TEST_F(VectorTest, AccessElementByAt) {
  for (stl_container::size_type i = 0; i < stl_vector.size(); ++i) {
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
  stl_vector.insert(stl_vector.begin(), 10, 10);
  ft_vector.insert(ft_vector.begin(), 10, 10);
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  for (stl_container::size_type i = 0; i < stl_vector.capacity(); ++i) {
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
  stl_vector.insert(stl_vector.begin(), 5, 10);
  ft_vector.insert(ft_vector.begin(), 5, 10);
  EXPECT_EQ(*(stl_vector.insert(stl_vector.begin(), 10)), 10);
  EXPECT_EQ(*(ft_vector.insert(ft_vector.begin(), 10)), 10);

  // 途中に挿入
  stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 5, 10);
  ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 5, 10);
  EXPECT_EQ(
      *(stl_vector.insert(stl_vector.begin() + stl_vector.size() / 2, 10)), 10);
  EXPECT_EQ(*(ft_vector.insert(ft_vector.begin() + ft_vector.size() / 2, 10)),
            10);

  // 最後に挿入
  stl_vector.insert(stl_vector.end(), 5, 10);
  ft_vector.insert(ft_vector.end(), 5, 10);
  EXPECT_EQ(*(stl_vector.insert(stl_vector.end(), 10)), 10);
  EXPECT_EQ(*(ft_vector.insert(ft_vector.end(), 10)), 10);

  expect_same_data_in_vector(stl_vector, ft_vector);
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
  EXPECT_EQ(stl_it, stl_vector.end());
  EXPECT_EQ(ft_it, ft_vector.end());
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
  EXPECT_EQ(stl_it, stl_vector.end());
  EXPECT_EQ(ft_it, ft_vector.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseWithTwoIteratorBetweenBeginAndLast) {
  stl_iterator stl_it = stl_vector.erase(
      stl_vector.begin() + stl_vector.size() / 2, stl_vector.end());
  ft_iterator ft_it = ft_vector.erase(ft_vector.begin() + ft_vector.size() / 2,
                                      ft_vector.end());
  EXPECT_EQ(std::distance(stl_vector.begin(), stl_it),
            std::distance(ft_vector.begin(), ft_it));
  EXPECT_EQ(stl_it, stl_vector.end());
  EXPECT_EQ(ft_it, ft_vector.end());
  expect_same_data_in_vector(stl_vector, ft_vector);
}

TEST_F(VectorTest, EraseDuringIterating) {
  // Iteratorでループ回している間に要素を削除する場合は erase() を使う.
  // http://marupeke296.com/TIPS_No12_ListElementErase.html
  ft_vector[ft_vector.size() / 2] = 1;
  for (ft_iterator ft_it = ft_vector.begin(); ft_it != ft_vector.end();) {
    if (*ft_it == 1) {
      ft_it = ft_vector.erase(ft_it);
      continue;
    }
    ++ft_it;
  }
  for (ft_iterator ft_it = ft_vector.begin(); ft_it != ft_vector.end();
       ++ft_it) {
    // 1は消したから存在しないはず
    EXPECT_NE(*ft_it, 1);
  }
}

TEST_F(VectorTest, EraseAtTheEndDuringIterating) {
  // イテレーターでループしながら最後の要素を erase() した時の挙動
  ft_vector.push_back(1);
  ft_vector.push_back(1);
  for (ft_iterator ft_it = ft_vector.begin(); ft_it != ft_vector.end();) {
    if (*ft_it == 1) {
      ft_it = ft_vector.erase(ft_it);
      continue;
    }
    ++ft_it;
  }
  for (ft_iterator ft_it = ft_vector.begin(); ft_it != ft_vector.end();
       ++ft_it) {
    // 1は消したから存在しないはず
    EXPECT_NE(*ft_it, 1);
  }
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

TEST(VectorOperator, Equal) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_TRUE(str_vec1 == str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_TRUE(str_vec1 == str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("b");
  EXPECT_FALSE(str_vec1 == str_vec2);
}

TEST(VectorOperator, NotEqual) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_FALSE(str_vec1 != str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_FALSE(str_vec1 != str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("b");
  EXPECT_TRUE(str_vec1 != str_vec2);
}

TEST(VectorOperator, LessThan) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_FALSE(str_vec1 < str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_FALSE(str_vec1 < str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("b");
  EXPECT_TRUE(str_vec1 < str_vec2);
}

TEST(VectorOperator, LessThanOrEqual) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_TRUE(str_vec1 <= str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_TRUE(str_vec1 <= str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("b");
  EXPECT_TRUE(str_vec1 <= str_vec2);
  str_vec1.pop_back();
  str_vec2.pop_back();
  str_vec1.push_back("b");
  str_vec2.push_back("a");
  EXPECT_FALSE(str_vec1 <= str_vec2);
}

TEST(VectorOperator, GreaterThan) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_FALSE(str_vec1 > str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_FALSE(str_vec1 > str_vec2);
  str_vec1.push_back("b");
  str_vec2.push_back("a");
  EXPECT_TRUE(str_vec1 > str_vec2);
}

TEST(VectorOperator, GreaterThanOrEqual) {
  ft::vector<std::string> str_vec1;
  ft::vector<std::string> str_vec2;

  EXPECT_TRUE(str_vec1 >= str_vec2);
  str_vec1.push_back("a");
  str_vec2.push_back("a");
  EXPECT_TRUE(str_vec1 >= str_vec2);
  str_vec1.push_back("b");
  str_vec2.push_back("a");
  EXPECT_TRUE(str_vec1 >= str_vec2);
  str_vec1.pop_back();
  str_vec2.pop_back();
  str_vec1.push_back("a");
  str_vec2.push_back("b");
  EXPECT_FALSE(str_vec1 >= str_vec2);
}

TEST(Vector2D, TestVectorWith2Dimensions) {
  typedef ft::vector<int> vector1d_type;
  typedef ft::vector<vector1d_type> vector2d_type;

  vector2d_type vec2d;

  for (int i = 0; i < 10; ++i) {
    vec2d.push_back(ft::vector<int>(i, i));
  }

  int i = 0;
  for (vector2d_type::iterator it2d = vec2d.begin(); it2d != vec2d.end();
       ++it2d, ++i) {
    EXPECT_EQ((*it2d).size(), vector2d_type::size_type(i));
    for (vector1d_type::iterator it1d = (*it2d).begin(); it1d != (*it2d).end();
         ++it1d) {
      EXPECT_EQ(*it1d, i);
    }
    std::cout << std::endl;
  }
}
