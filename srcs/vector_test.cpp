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
  {
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

    std::vector<int>::iterator stl_it = stl_vector.begin();
    std::vector<int>::const_iterator const_stl_it = stl_vector.begin();
    ft::vector<int>::iterator ft_it = ft_vector.begin();
    ft::vector<int>::const_iterator const_ft_it = ft_vector.begin();
    // assert(*stl_it == *ft_it);

    (void)stl_it;
    (void)ft_it;
  }
  return 0;
}
