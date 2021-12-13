#include "vector.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#define TEST_SECTION(msg) std::cout << "===== " msg " =====" << std::endl
#define TEST_SUBSECTION(msg) std::cout << "-- " msg " --" << std::endl

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

    assert(is_same_vector(stl_vector, ft_vector));

    assert(stl_vector.size() == ft_vector.size());
    std::cout << "size: " << ft_vector.size() << std::endl;
    assert(stl_vector.max_size() == ft_vector.max_size());
    std::cout << "max_size: " << ft_vector.max_size() << std::endl;
    assert(stl_vector.capacity() == ft_vector.capacity());
    std::cout << "capacity: " << ft_vector.capacity() << std::endl;
    assert(stl_vector.empty() == ft_vector.empty());
  }

  {
    // デフォルトコンストラクタ
    TEST_SECTION("default constructor");

    std::vector<int> stl_vector;
    ft::vector<int> ft_vector;

    assert(is_same_vector(stl_vector, ft_vector));

    assert(stl_vector.size() == ft_vector.size());
    std::cout << "size: " << ft_vector.size() << std::endl;
    assert(stl_vector.max_size() == ft_vector.max_size());
    std::cout << "max_size: " << ft_vector.max_size() << std::endl;
    assert(stl_vector.capacity() == ft_vector.capacity());
    std::cout << "capacity: " << ft_vector.capacity() << std::endl;
    assert(stl_vector.empty() == ft_vector.empty());
  }

  { /* normal iterator */
    TEST_SECTION("normal iterator");

    std::vector<int> stl_vector(10);
    ft::vector<int> ft_vector(10);

    /* begin() can return iterator and const_iterator */
    std::vector<int>::iterator stl_it = stl_vector.begin();
    ft::vector<int>::iterator ft_it = ft_vector.begin();
    assert(*stl_it == *ft_it);
    std::vector<int>::const_iterator const_stl_it = stl_vector.begin();
    ft::vector<int>::const_iterator const_ft_it = ft_vector.begin();
    assert(*const_stl_it == *const_ft_it);

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
    assert(*stl_rit == *ft_rit);
    std::vector<int>::const_reverse_iterator const_stl_rit =
        stl_vector.rbegin();
    ft::vector<int>::const_reverse_iterator const_ft_rit = ft_vector.rbegin();
    assert(*const_stl_rit == *const_ft_rit);

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
      assert(stl_vector.size() == ft_vector.size());
      std::cout << "size: " << ft_vector.size() << std::endl;
      assert(stl_vector.max_size() == ft_vector.max_size());
      std::cout << "max_size: " << ft_vector.max_size() << std::endl;
      assert(stl_vector.capacity() == ft_vector.capacity());
      std::cout << "capacity: " << ft_vector.capacity() << std::endl;
      assert(stl_vector.empty() == ft_vector.empty());

      /* reserve */

      // 現在の要素数より少ない時は何も起きない
      stl_vector.reserve(0);
      ft_vector.reserve(0);
      assert(stl_vector.size() == ft_vector.size());
      assert(stl_vector.capacity() == ft_vector.capacity());

      // 現在の要素数より多い時は実行される
      stl_vector.reserve(100);
      ft_vector.reserve(100);
      assert(stl_vector.size() == ft_vector.size());
      assert(stl_vector.capacity() == ft_vector.capacity());

      is_same_vector(stl_vector, ft_vector);

      // allocator.max_size() を超えるのはエラー
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
      assert(std_has_caught_exception == ft_has_caught_exception);
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
      assert(stl_vector[i] == ft_vector[i]);
      assert(stl_vector.at(i) == ft_vector.at(i));
    }
    assert(stl_vector.front() == ft_vector.front());
    assert(stl_vector.back() == ft_vector.back());

    // vector.at() should throw out_of_range exception
    //   if argument n is larger than container size
    {
      bool has_caught_exception = false;
      try {
        stl_vector.at(3);
      } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
        has_caught_exception = true;
      }
      assert(has_caught_exception);  // STL vector の動作確認
    }
    {
      bool has_caught_exception = false;
      try {
        ft_vector.at(3);
      } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
        has_caught_exception = true;
      }
      assert(has_caught_exception);
    }
  }

  {
    // Modifications
    TEST_SECTION("Modifications");

    std::vector<std::string> stl_vector;
    ft::vector<std::string> ft_vector;

    TEST_SUBSECTION("assign(n, val)");
    {
      // capacityを超えるとき
      {
        std::vector<int> stl_assign_vector(5);
        ft::vector<int> ft_assign_vector(5);

        stl_assign_vector.assign(10, 2);
        ft_assign_vector.assign(10, 2);
        assert(stl_assign_vector.size() == ft_assign_vector.size());
        assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
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
        assert(stl_assign_vector.size() == ft_assign_vector.size());
        std::cout << "capacity of stl_assign_vector: "
                  << stl_assign_vector.capacity() << std::endl;
        std::cout << "capacity of ft_assign_vector: "
                  << ft_assign_vector.capacity() << std::endl;
        assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
        is_same_vector(stl_assign_vector, ft_assign_vector);
      }

      // sizeもcapacityも超えない時
      {
        std::vector<int> stl_assign_vector(100);
        ft::vector<int> ft_assign_vector(100);

        stl_assign_vector.assign(10, 2);
        ft_assign_vector.assign(10, 2);
        assert(stl_assign_vector.size() == ft_assign_vector.size());
        assert(stl_assign_vector.capacity() == ft_assign_vector.capacity());
        is_same_vector(stl_assign_vector, ft_assign_vector);
      }
    }

    TEST_SUBSECTION("push_back()");
    // size() == capacity() になったら自動拡張される
    for (int i = 0; i < 16; ++i) {
      stl_vector.push_back("hoge");
      ft_vector.push_back("hoge");
      assert(stl_vector.size() == ft_vector.size());
      assert(stl_vector.capacity() == ft_vector.capacity());
      is_same_vector(stl_vector, ft_vector);
    }

    TEST_SUBSECTION("pop_back()");
    for (int i = 0; i < 16; ++i) {
      stl_vector.pop_back();
      ft_vector.pop_back();
      assert(stl_vector.size() == ft_vector.size());
      assert(stl_vector.capacity() == ft_vector.capacity());
      is_same_vector(stl_vector, ft_vector);
    }
  }
  return 0;
}
