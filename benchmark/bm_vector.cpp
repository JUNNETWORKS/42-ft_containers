#include <unistd.h>

#include <vector>

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
  const int loop_num = 10;

  std_vector_type std_vec;
  ft_vector_type ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.constructor", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vector_type tmp;
    }
  }
  {
    TIMER("ft::vector.constructor", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vector_type tmp;
    }
  }

  {
    TIMER("std::vector.constructor(other)", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vector_type tmp(std_vec);
    }
  }
  {
    TIMER("ft::vector.constructor(other)", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vector_type tmp(ft_vec);
    }
  }
}

void measure_vector_assignation() {
  HEADER("measure_vector_assignation");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;
  const int loop_num = 10;

  std_vector_type std_vec;
  std_vector_type std_vec_for_copy;
  ft_vector_type ft_vec;
  ft_vector_type ft_vec_for_copy;

  add_nums_into_vector(std_vec_for_copy, ft_vec_for_copy, default_vec_size);

  {
    TIMER("std::vector.operator=", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec = std_vec_for_copy;
    }
  }
  {
    TIMER("ft::vector.operator=", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec = ft_vec_for_copy;
    }
  }
  {
    TIMER("std::vector.assign", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.assign(std_vec_for_copy.begin(), std_vec_for_copy.end());
    }
  }
  {
    TIMER("ft::vector.assign", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.assign(ft_vec_for_copy.begin(), ft_vec_for_copy.end());
    }
  }
}

void measure_vector_modifiers() {
  HEADER("measure_vector_modifiers");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;
  const int loop_num = default_vec_size;

  std_vector_type std_vec;
  ft_vector_type ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.clear", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.clear();
    }
  }
  {
    TIMER("ft::vector.clear", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.clear();
    }
  }

  {
    TIMER("std::vector.push_back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.push_back(i);
    }
  }
  {
    TIMER("ft::vector.push_back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.push_back(i);
    }
  }

  {
    TIMER("std::vector.pop_back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.pop_back();
    }
  }
  {
    TIMER("ft::vector.pop_back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.pop_back();
    }
  }
}

void measure_vector_element_access() {
  HEADER("measure_vector_element_access");

  const int default_vec_size = 1000000;
  const int loop_num = default_vec_size;

  std::vector<int> std_vec;
  ft::vector<int> ft_vec;

  for (int i = 0; i < loop_num; ++i) {
    std_vec.push_back(i);
  }
  for (int i = 0; i < loop_num; ++i) {
    ft_vec.push_back(i);
  }

  {
    TIMER("std::vector.at", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.at(i);
    }
  }
  {
    TIMER("ft::vector.at", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.at(i);
    }
  }

  {
    TIMER("std::vector.operator[]", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec[i];
    }
  }
  {
    TIMER("ft::vector.operator[]", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec[i];
    }
  }

  {
    TIMER("std::vector.front", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.front();
    }
  }
  {
    TIMER("ft::vector.front", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.front();
    }
  }

  {
    TIMER("std::vector.back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.back();
    }
  }
  {
    TIMER("ft::vector.back", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.back();
    }
  }

  {
    TIMER("std::vector.data", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.data();
    }
  }
  {
    TIMER("ft::vector.data", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.data();
    }
  }
}

void measure_vector_iterator() {
  HEADER("measure_vector_iterator");

  typedef std::vector<int> std_vector_type;
  typedef ft::vector<int> ft_vector_type;

  const int default_vec_size = 1000000;
  const int loop_num = 10;

  std_vector_type std_vec;
  ft_vector_type ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector forward iterator", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      for (std_vector_type::const_iterator it = std_vec.begin();
           it != std_vec.end(); ++it) {
        *it;
      }
    }
  }
  {
    TIMER("ft::vector forward iterator", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      for (ft_vector_type::const_iterator it = ft_vec.begin();
           it != ft_vec.end(); ++it) {
        *it;
      }
    }
  }

  {
    TIMER("std::vector reverse iterator", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      for (std_vector_type::const_reverse_iterator it = std_vec.rbegin();
           it != std_vec.rend(); ++it) {
        *it;
      }
    }
  }
  {
    TIMER("ft::vector reverse iterator", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      for (ft_vector_type::const_reverse_iterator it = ft_vec.rbegin();
           it != ft_vec.rend(); ++it) {
        *it;
      }
    }
  }
}

void measure_vector_capacity() {
  HEADER("measure_vector_capacity");

  const int default_vec_size = 1000000;
  const int loop_num = default_vec_size;

  std::vector<int> std_vec;
  ft::vector<int> ft_vec;

  add_nums_into_vector(std_vec, ft_vec, default_vec_size);

  {
    TIMER("std::vector.empty", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.empty();
    }
  }
  {
    TIMER("ft::vector.empty", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.empty();
    }
  }

  {
    TIMER("std::vector.size", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.size();
    }
  }
  {
    TIMER("ft::vector.size", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.size();
    }
  }

  {
    TIMER("std::vector.max_size", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.max_size();
    }
  }
  {
    TIMER("ft::vector.max_size", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.max_size();
    }
  }

  {
    TIMER("std::vector.reserve", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.reserve(i);
    }
  }
  {
    TIMER("ft::vector.reserve", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.reserve(i);
    }
  }

  {
    TIMER("std::vector.capacity", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      std_vec.capacity();
    }
  }
  {
    TIMER("ft::vector.capacity", loop_num);
    for (int i = 0; i < loop_num; ++i) {
      ft_vec.capacity();
    }
  }
}

}  // namespace