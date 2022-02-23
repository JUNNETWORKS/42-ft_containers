#include <unistd.h>

#include <vector>

#include "benchmarks.hpp"
#include "timer.hpp"
#include "vector.hpp"

namespace {

template <class T>
inline void add_nums_into_vector(std::vector<T>& std_vec, ft::vector<T>& ft_vec,
                                 const int default_vec_size) {
  for (int i = 0; i < default_vec_size; ++i) {
    std_vec.push_back(i);
    ft_vec.push_back(i);
  }
}

}  // namespace

namespace {

void measure_vector_constructors();
void measure_vector_assignation();
void measure_vector_modifiers();
void measure_vector_element_access();
void measure_vector_iterator();
void measure_vector_capacity();

}  // namespace

void measure_vector() {
  measure_vector_constructors();
  measure_vector_assignation();
  measure_vector_modifiers();
  measure_vector_element_access();
  measure_vector_iterator();
  measure_vector_capacity();
}

namespace {

void measure_vector_constructors() {
  HEADER("measure_vector_constructors");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;

  std_vector_type std_vec;
  ft_vector_type ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.constructor");
    std_vector_type tmp;
  }
  {
    TIMER("ft::vector.constructor");
    ft_vector_type tmp;
  }

  {
    TIMER("std::vector.constructor(other)");
    std_vector_type tmp(std_vec);
  }
  {
    TIMER("ft::vector.constructor(other)");
    ft_vector_type tmp(ft_vec);
  }
}

void measure_vector_assignation() {
  HEADER("measure_vector_assignation");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;

  std_vector_type std_vec;
  std_vector_type std_vec_for_copy;
  ft_vector_type ft_vec;
  ft_vector_type ft_vec_for_copy;

  add_nums_into_vector(std_vec_for_copy, ft_vec_for_copy, default_vec_size);

  {
    TIMER("std::vector.operator=");
    std_vec = std_vec_for_copy;
  }
  {
    TIMER("ft::vector.operator=");
    ft_vec = ft_vec_for_copy;
  }
  {
    TIMER("std::vector.assign");
    std_vec.assign(std_vec_for_copy.begin(), std_vec_for_copy.end());
  }
  {
    TIMER("ft::vector.assign");
    ft_vec.assign(ft_vec_for_copy.begin(), ft_vec_for_copy.end());
  }
}

void measure_vector_modifiers() {
  HEADER("measure_vector_modifiers");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;

  {
    std_vector_type std_vec;
    ft_vector_type ft_vec;

    add_nums_into_vector(std_vec, ft_vec, default_vec_size);

    {
      TIMER("std::vector.clear");
      std_vec.clear();
    }
    {
      TIMER("ft::vector.clear");
      ft_vec.clear();
    }

    {
      TIMER("std::vector.push_back");
      for (int i = 0; i < default_vec_size; ++i) {
        std_vec.push_back(i);
      }
    }
    {
      TIMER("ft::vector.push_back");
      for (int i = 0; i < default_vec_size; ++i) {
        ft_vec.push_back(i);
      }
    }

    {
      TIMER("std::vector.pop_back");
      std_vec.pop_back();
    }
    {
      TIMER("ft::vector.pop_back");
      ft_vec.pop_back();
    }
  }

  {
    std_vector_type std_vec;
    ft_vector_type ft_vec;

    add_nums_into_vector(std_vec, ft_vec, default_vec_size);

    {
      TIMER("std::vector.erase(it, it)");
      std_vec.erase(std_vec.begin() + std_vec.size() / 2, std_vec.end());
    }
    {
      TIMER("ft::vector.erase(it, it)");
      ft_vec.erase(ft_vec.begin() + ft_vec.size() / 2, ft_vec.end());
    }
  }
}

void measure_vector_element_access() {
  HEADER("measure_vector_element_access");

  const int default_vec_size = 1000000;

  std::vector<int> std_vec;
  ft::vector<int> ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.at");
    for (int i = 0; i < default_vec_size; ++i) {
      std_vec.at(i);
    }
  }
  {
    TIMER("ft::vector.at");
    for (int i = 0; i < default_vec_size; ++i) {
      ft_vec.at(i);
    }
  }

  {
    TIMER("std::vector.operator[]");
    for (int i = 0; i < default_vec_size; ++i) {
      std_vec[i];
    }
  }
  {
    TIMER("ft::vector.operator[]");
    for (int i = 0; i < default_vec_size; ++i) {
      ft_vec[i];
    }
  }

  {
    TIMER("std::vector.front");
    std_vec.front();
  }
  {
    TIMER("ft::vector.front");
    ft_vec.front();
  }

  {
    TIMER("std::vector.back");
    std_vec.back();
  }
  {
    TIMER("ft::vector.back");
    ft_vec.back();
  }

  {
    TIMER("std::vector.data");
    std_vec.data();
  }
  {
    TIMER("ft::vector.data");
    ft_vec.data();
  }
}

void measure_vector_iterator() {
  HEADER("measure_vector_iterator");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;

  std_vector_type std_vec;
  ft_vector_type ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector forward iterator");
    for (std_vector_type::const_iterator it = std_vec.begin();
         it != std_vec.end(); ++it) {
      *it;
    }
  }
  {
    TIMER("ft::vector forward iterator");
    for (ft_vector_type::const_iterator it = ft_vec.begin(); it != ft_vec.end();
         ++it) {
      *it;
    }
  }

  {
    TIMER("std::vector reverse iterator");
    for (std_vector_type::const_reverse_iterator it = std_vec.rbegin();
         it != std_vec.rend(); ++it) {
      *it;
    }
  }
  {
    TIMER("ft::vector reverse iterator");
    for (ft_vector_type::const_reverse_iterator it = ft_vec.rbegin();
         it != ft_vec.rend(); ++it) {
      *it;
    }
  }
}

void measure_vector_capacity() {
  HEADER("measure_vector_capacity");

  const int default_vec_size = 1000000;

  std::vector<int> std_vec;
  ft::vector<int> ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.empty");
    std_vec.empty();
  }
  {
    TIMER("ft::vector.empty");
    ft_vec.empty();
  }

  {
    TIMER("std::vector.size");
    std_vec.size();
  }
  {
    TIMER("ft::vector.size");
    ft_vec.size();
  }

  {
    TIMER("std::vector.max_size");
    std_vec.max_size();
  }
  {
    TIMER("ft::vector.max_size");
    ft_vec.max_size();
  }

  {
    TIMER("std::vector.reserve");
    for (int cap = 1; cap < (1 << 10); cap <<= 1) {
      std_vec.reserve(cap);
    }
  }
  {
    TIMER("ft::vector.reserve");
    for (int cap = 1; cap < (1 << 10); cap <<= 1) {
      ft_vec.reserve(cap);
    }
  }

  {
    TIMER("std::vector.capacity");
    std_vec.capacity();
  }
  {
    TIMER("ft::vector.capacity");
    ft_vec.capacity();
  }
}

}  // namespace