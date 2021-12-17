#include "vector.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "test_utils.hpp"

template <typename T>
bool is_same_vector(std::vector<T> &stl_vector, ft::vector<T> &ft_vector) {
  typename std::vector<T>::iterator stl_it = stl_vector.begin();
  typename ft::vector<T>::iterator ft_it = ft_vector.begin();

  while (stl_it != stl_vector.end()) {
    if (*stl_it++ != *ft_it++) {
      return false;
    }
  }
  if (ft_it != ft_vector.end()) {
    return false;
  }
  return true;
}

int main() {
  srand(time(NULL));

  { /* sample test */
    TEST_SECTION("sample test");

    std::vector<int> stl_vector(10, 0);
    ft::vector<int> ft_vector(10, 0);

    ASSERT_TRUE("is_same_vector(stl_vector, ft_vector)",
                is_same_vector(stl_vector, ft_vector));

    ASSERT_EQ("ft_vector.size() should be equal to stl_vector.size()",
              stl_vector.size(), ft_vector.size());
    std::cout << "size: " << ft_vector.size() << std::endl;
    ASSERT_EQ("ft_vector.max_size() should be equal to stl_vector.size()",
              stl_vector.max_size(), ft_vector.max_size());
    std::cout << "max_size: " << ft_vector.max_size() << std::endl;
    ASSERT_EQ("ft_vector.capacity() should be equal to stl_vector.size()",
              stl_vector.capacity(), ft_vector.capacity());
    std::cout << "capacity: " << ft_vector.capacity() << std::endl;
    ASSERT_EQ("ft_vector.empty() should be equal to stl_vector.size()",
              stl_vector.empty(), ft_vector.empty());
  }

  {
    // デフォルトコンストラクタ
    TEST_SECTION("default constructor");

    std::vector<int> stl_vector;
    ft::vector<int> ft_vector;

    ASSERT_TRUE("is_same_vector(stl_vector, ft_vector)",
                is_same_vector(stl_vector, ft_vector));

    ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
              ft_vector.size());
    std::cout << "size: " << ft_vector.size() << std::endl;
    ASSERT_EQ("stl_vector.max_size() == ft_vector.max_size()",
              stl_vector.max_size(), ft_vector.max_size());
    std::cout << "max_size: " << ft_vector.max_size() << std::endl;
    ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
              stl_vector.capacity(), ft_vector.capacity());
    std::cout << "capacity: " << ft_vector.capacity() << std::endl;
    ASSERT_EQ("stl_vector.empty() == ft_vector.empty()", stl_vector.empty(),
              ft_vector.empty());
  }

  { /* normal iterator */
    TEST_SECTION("normal iterator");

    std::vector<int> stl_vector(10);
    ft::vector<int> ft_vector(10);

    /* begin() can return iterator and const_iterator */
    std::vector<int>::iterator stl_it = stl_vector.begin();
    ft::vector<int>::iterator ft_it = ft_vector.begin();
    ASSERT_EQ("*stl_it == *ft_it", *stl_it, *ft_it);
    std::vector<int>::const_iterator const_stl_it = stl_vector.begin();
    ft::vector<int>::const_iterator const_ft_it = ft_vector.begin();
    ASSERT_EQ("*const_stl_it == *const_ft_it", *const_stl_it, *const_ft_it);

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
    is_same_vector(stl_vector, ft_vector);

    std::cout << "normal iterator implementation is correct!" << std::endl;
  }

  { /* reverse iterator */
    TEST_SECTION("reverse iterator");

    std::vector<int> stl_vector(10, 0);
    ft::vector<int> ft_vector(10, 0);

    /* begin() can return iterator and const_iterator */
    std::vector<int>::reverse_iterator stl_rit = stl_vector.rbegin();
    ft::vector<int>::reverse_iterator ft_rit = ft_vector.rbegin();
    ASSERT_EQ("*stl_rit == *ft_rit", *stl_rit, *ft_rit);
    std::vector<int>::const_reverse_iterator const_stl_rit =
        stl_vector.rbegin();
    ft::vector<int>::const_reverse_iterator const_ft_rit = ft_vector.rbegin();
    ASSERT_EQ("*const_stl_rit == *const_ft_rit", *const_stl_rit, *const_ft_rit);

    /* for loop ends until it reach rend() */
    const_stl_rit = stl_vector.rbegin();
    const_ft_rit = ft_vector.rbegin();
    for (;
         const_stl_rit != stl_vector.rend() && const_ft_rit != ft_vector.rend();
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
    is_same_vector(stl_vector, ft_vector);

    std::cout << "reverse iterator implementation is correct!" << std::endl;
  }

  { /* Capacity */
    TEST_SECTION("Capacity");

    std::vector<int> test_lens;
    test_lens.push_back(0);
    test_lens.push_back(1);
    test_lens.push_back(2);
    test_lens.push_back(10);

    for (std::vector<int>::const_iterator it = test_lens.begin();
         it != test_lens.end(); ++it) {
      int len = *it;
      std::vector<int> stl_vector(len);
      ft::vector<int> ft_vector(len);
      ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
                ft_vector.size());
      std::cout << "size: " << ft_vector.size() << std::endl;
      ASSERT_EQ("stl_vector.max_size() == ft_vector.max_size()",
                stl_vector.max_size(), ft_vector.max_size());
      std::cout << "max_size: " << ft_vector.max_size() << std::endl;
      ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
                stl_vector.capacity(), ft_vector.capacity());
      std::cout << "capacity: " << ft_vector.capacity() << std::endl;
      ASSERT_EQ("stl_vector.empty() == ft_vector.empty()", stl_vector.empty(),
                ft_vector.empty());

      /* reserve */

      TEST_SUBSECTION("現在の要素数より少ない時は何も起きない");
      stl_vector.reserve(0);
      ft_vector.reserve(0);
      ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
                ft_vector.size());
      ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
                stl_vector.capacity(), ft_vector.capacity());

      TEST_SUBSECTION("現在の要素数より多い時は実行される");
      stl_vector.reserve(100);
      ft_vector.reserve(100);
      ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
                ft_vector.size());
      ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
                stl_vector.capacity(), ft_vector.capacity());

      is_same_vector(stl_vector, ft_vector);

      bool std_has_caught_exception = false;
      try {
        stl_vector.reserve(std::allocator<int>().max_size() + 1);
      } catch (const std::length_error &e) {
        std::cerr << e.what() << std::endl;
        std_has_caught_exception = true;
      }
      bool ft_has_caught_exception = false;
      try {
        ft_vector.reserve(std::allocator<int>().max_size() + 1);
      } catch (const std::length_error &e) {
        std::cerr << e.what() << std::endl;
        ft_has_caught_exception = true;
      }
      ASSERT_EQ("allocator.max_size() を超えるのはエラー",
                std_has_caught_exception, ft_has_caught_exception);
    }
  }

  {
    // Element Access
    TEST_SECTION("Element Access");

    std::vector<int> stl_vector(3);
    ft::vector<int> ft_vector(3);

    for (int i = 0; i < 3; ++i) {
      stl_vector[i] = i;
      ft_vector[i] = i;
    }
    for (int i = 0; i < 3; ++i) {
      ASSERT_EQ("stl_vector[i] == ft_vector[i]", stl_vector[i], ft_vector[i]);
      ASSERT_EQ("stl_vector.at(i) == ft_vector.at(i)", stl_vector.at(i),
                ft_vector.at(i));
    }
    ASSERT_EQ("stl_vector.front() == ft_vector.front()", stl_vector.front(),
              ft_vector.front());
    ASSERT_EQ("stl_vector.back() == ft_vector.back()", stl_vector.back(),
              ft_vector.back());

    {
      bool has_caught_exception = false;
      try {
        stl_vector.at(3);
      } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
        has_caught_exception = true;
      }
      ASSERT_TRUE(
          "std::vector.at() should throw out_of_range exception  "
          "if argument n is larger than container size",
          has_caught_exception);  // STL vector の動作確認
    }
    {
      bool has_caught_exception = false;
      try {
        ft_vector.at(3);
      } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
        has_caught_exception = true;
      }
      ASSERT_TRUE(
          "ft::vector.at() should throw out_of_range exception  "
          "if argument n is larger than container size",
          has_caught_exception);
    }
  }

  {
    // Modifications
    TEST_SECTION("Modifications");

    // TEST_SUBSECTION("assign(first_it, last_it"));
    // {
    //   // capacityを超えるとき
    //   {
    //     std::vector<int> stl_assign_vector(5);
    //     ft::vector<int> ft_assign_vector(5);

    //     std::vector<int> new_values(10, 1);

    //     stl_assign_vector.assign(new_values.begin(), new_values.end());
    //     ft_assign_vector.assign(new_values.begin(), new_values.end());
    //     assert(stl_assign_vector.size() == ft_assign_vector.size());
    //     assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
    //     is_same_vector(stl_assign_vector, ft_assign_vector);
    //   }
    //   // sizeを超える時
    //   {
    //     std::vector<int> stl_assign_vector(5);
    //     ft::vector<int> ft_assign_vector(5);
    //     stl_assign_vector.reserve(100);
    //     ft_assign_vector.reserve(100);

    //     std::vector<int> new_values(10, 1);

    //     stl_assign_vector.assign(new_values.begin(), new_values.end());
    //     ft_assign_vector.assign(new_values.begin(), new_values.end());
    //     assert(stl_assign_vector.size() == ft_assign_vector.size());
    //     std::cout << "capacity of stl_assign_vector: "
    //               << stl_assign_vector.capacity() << std::endl;
    //     std::cout << "capacity of ft_assign_vector: "
    //               << ft_assign_vector.capacity() << std::endl;
    //     assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
    //     is_same_vector(stl_assign_vector, ft_assign_vector);
    //   }

    //   // sizeもcapacityも超えない時
    //   {
    //     std::vector<int> stl_assign_vector(100);
    //     ft::vector<int> ft_assign_vector(100);

    //     std::vector<int> new_values(10, 1);

    //     stl_assign_vector.assign(new_values.begin(), new_values.end());
    //     ft_assign_vector.assign(new_values.begin(), new_values.end());
    //     assert(stl_assign_vector.size() == ft_assign_vector.size());
    //     assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
    //     is_same_vector(stl_assign_vector, ft_assign_vector);
    //   }
    // }

    TEST_SUBSECTION("assign(n, val)");
    {
      // capacityを超えるとき
      {
        std::vector<int> stl_assign_vector(5);
        ft::vector<int> ft_assign_vector(5);

        stl_assign_vector.assign(10, 2);
        ft_assign_vector.assign(10, 2);
        ASSERT_EQ("stl_assign_vector.size() == ft_assign_vector.size()",
                  stl_assign_vector.size(), ft_assign_vector.size());
        ASSERT_EQ("stl_assign_vector.capacity() == ft_assign_vector.capacity()",
                  stl_assign_vector.capacity(), ft_assign_vector.capacity());
        is_same_vector(stl_assign_vector, ft_assign_vector);
      }
      // sizeを超える時
      {
        std::vector<int> stl_assign_vector(5);
        ft::vector<int> ft_assign_vector(5);
        stl_assign_vector.reserve(100);
        ft_assign_vector.reserve(100);

        stl_assign_vector.assign(10, 2);
        ft_assign_vector.assign(10, 2);
        ASSERT_EQ("stl_assign_vector.size() == ft_assign_vector.size()",
                  stl_assign_vector.size(), ft_assign_vector.size());
        std::cout << "capacity of stl_assign_vector: "
                  << stl_assign_vector.capacity() << std::endl;
        std::cout << "capacity of ft_assign_vector: "
                  << ft_assign_vector.capacity() << std::endl;
        ASSERT_EQ("stl_assign_vector.capacity() == ft_assign_vector.capacity()",
                  stl_assign_vector.capacity(), ft_assign_vector.capacity());
        is_same_vector(stl_assign_vector, ft_assign_vector);
      }

      // sizeもcapacityも超えない時
      {
        std::vector<int> stl_assign_vector(100);
        ft::vector<int> ft_assign_vector(100);

        stl_assign_vector.assign(10, 2);
        ft_assign_vector.assign(10, 2);
        ASSERT_EQ("stl_assign_vector.size() == ft_assign_vector.size()",
                  stl_assign_vector.size(), ft_assign_vector.size());
        ASSERT_EQ("stl_assign_vector.capacity() == ft_assign_vector.capacity()",
                  stl_assign_vector.capacity(), ft_assign_vector.capacity());
        is_same_vector(stl_assign_vector, ft_assign_vector);
      }
    }

    TEST_SUBSECTION("push_back()");
    {
      std::vector<std::string> stl_vector;
      ft::vector<std::string> ft_vector;
      // size() == capacity() になったら自動拡張される
      for (int i = 0; i < 16; ++i) {
        stl_vector.push_back("hoge");
        ft_vector.push_back("hoge");
        ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
                  ft_vector.size());
        ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
                  stl_vector.capacity(), ft_vector.capacity());
        is_same_vector(stl_vector, ft_vector);
      }
    }

    TEST_SUBSECTION("pop_back()");
    {
      std::vector<std::string> stl_vector(16);
      ft::vector<std::string> ft_vector(16);
      for (int i = 0; i < 16; ++i) {
        stl_vector.pop_back();
        ft_vector.pop_back();
        ASSERT_EQ("stl_vector.size() == ft_vector.size()", stl_vector.size(),
                  ft_vector.size());
        ASSERT_EQ("stl_vector.capacity() == ft_vector.capacity()",
                  stl_vector.capacity(), ft_vector.capacity());
        is_same_vector(stl_vector, ft_vector);
      }
    }

    TEST_SUBSECTION("clear()");
    {
      std::vector<std::string> stl_vector(16);
      ft::vector<std::string> ft_vector(16);
      stl_vector.clear();
      ft_vector.clear();
      assert(stl_vector.size() == ft_vector.size());
      assert(stl_vector.capacity() == ft_vector.capacity());
      assert(stl_vector.empty() == ft_vector.empty());
    }
  }

  print_test_result();
  return 0;
}
