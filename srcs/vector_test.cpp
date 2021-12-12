#include "vector.hpp"

#include <cassert>
#include <iostream>
#include <vector>

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
  { /* sample test */
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

  { /* normal iterator */
    std::vector<int> stl_vector(10, 0);
    ft::vector<int> ft_vector(10, 0);

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
    for (int i = 0; stl_it != stl_vector.rend() && ft_it != ft_vector.rend();
         ++i, ++stl_rit, ++ft_rit) {
      *stl_rit = i;
      *ft_rit = i;
    }
    is_same_vector(stl_vector, ft_vector);

    std::cout << "reverse iterator implementation is correct!" << std::endl;
  }
  return 0;
}
