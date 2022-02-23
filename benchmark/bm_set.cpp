#include <unistd.h>

#include <cstdlib>
#include <set>

#include "benchmarks.hpp"
#include "set.hpp"
#include "timer.hpp"

namespace {

inline void add_nums_to_set(std::set<int> &std_set, ft::set<int> &ft_set,
                            const int set_size) {
  for (int i = 0; i < set_size; ++i) {
    std_set.insert(i);
    ft_set.insert(i);
  }
}

void measure_set_constructor_and_assignation();
void measure_set_iterator();
void measure_set_capacity();
void measure_set_modifiers();
void measure_set_lookup();

}  // namespace

void measure_set() {
  measure_set_constructor_and_assignation();
  measure_set_iterator();
  measure_set_capacity();
  measure_set_modifiers();
  measure_set_lookup();
}

namespace {

void measure_set_constructor_and_assignation() {
  HEADER("measure_set_constructor_and_assignation");

  typedef std::set<int> std_set_type;
  typedef ft::set<int> ft_set_type;

  const int max_size = 100000;

  std_set_type std_set_for_copy;
  ft_set_type ft_set_for_copy;

  add_nums_to_set(std_set_for_copy, ft_set_for_copy, max_size);

  {
    TIMER("std::set constructor");
    std_set_type tmp;
  }
  {
    TIMER("ft::set constructor");
    ft_set_type tmp;
  }

  {
    TIMER("std::set copy constructor");
    std_set_type tmp(std_set_for_copy);
  }
  {
    TIMER("ft::set copy constructor");
    ft_set_type tmp(ft_set_for_copy);
  }

  {
    TIMER("std::set assignation");
    std_set_type tmp;
    tmp = std_set_for_copy;
  }
  {
    TIMER("ft::set assignation");
    ft_set_type tmp;
    tmp = ft_set_for_copy;
  }
}

void measure_set_iterator() {
  HEADER("measure_set_iterator");

  typedef std::set<int> std_set_type;
  typedef ft::set<int> ft_set_type;

  const int max_size = 1000000;

  std_set_type std_set;
  ft_set_type ft_set;

  add_nums_to_set(std_set, ft_set, max_size);

  {
    TIMER("std::set forward iterator");
    for (std_set_type::iterator it = std_set.begin(); it != std_set.end();
         ++it) {
      *it;
    }
  }
  {
    TIMER("ft::set forward iterator");
    for (ft_set_type::iterator it = ft_set.begin(); it != ft_set.end(); ++it) {
      *it;
    }
  }

  {
    TIMER("std::set reverse iterator");
    for (std_set_type::reverse_iterator it = std_set.rbegin();
         it != std_set.rend(); ++it) {
      *it;
    }
  }
  {
    TIMER("ft::set reverse iterator");
    for (ft_set_type::reverse_iterator it = ft_set.rbegin();
         it != ft_set.rend(); ++it) {
      *it;
    }
  }
}

void measure_set_capacity() {
  HEADER("measure_set_capacity");

  typedef std::set<int> std_set_type;
  typedef ft::set<int> ft_set_type;

  const int max_size = 1000000;

  std_set_type std_set;
  ft_set_type ft_set;

  add_nums_to_set(std_set, ft_set, max_size);

  {
    TIMER("std::set empty");
    std_set.empty();
  }
  {
    TIMER("ft::set empty");
    ft_set.empty();
  }

  {
    TIMER("std::set size");
    std_set.size();
  }
  {
    TIMER("ft::set size");
    ft_set.size();
  }

  {
    TIMER("std::set max_size");
    std_set.max_size();
  }
  {
    TIMER("ft::set max_size");
    ft_set.max_size();
  }
}

void measure_set_modifiers() {
  HEADER("measure_set_modifiers");

  typedef std::set<int> std_set_type;
  typedef ft::set<int> ft_set_type;

  const int max_size = 1000000;

  std_set_type std_set;
  ft_set_type ft_set;

  {
    TIMER("std::set insert");
    for (int j = 0; j < max_size; ++j) {
      std_set.insert(j);
    }
  }
  {
    TIMER("ft::set insert");
    for (int j = 0; j < max_size; ++j) {
      ft_set.insert(j);
    }
  }

  {
    TIMER("std::set clear");
    for (int j = 0; j < max_size; ++j) {
      std_set.clear();
    }
  }
  {
    TIMER("ft::set clear");
    for (int j = 0; j < max_size; ++j) {
      ft_set.clear();
    }
  }

  add_nums_to_set(std_set, ft_set, max_size);

  {
    TIMER("std::set erase");
    for (int j = 0; j < max_size; ++j) {
      std_set.erase(j);
    }
  }
  {
    TIMER("ft::set erase");
    for (int j = 0; j < max_size; ++j) {
      ft_set.erase(j);
    }
  }
}

void measure_set_lookup() {
  HEADER("measure_set_lookup");

  typedef std::set<int> std_set_type;
  typedef ft::set<int> ft_set_type;

  const int max_size = 1000000;

  std_set_type std_set;
  ft_set_type ft_set;

  {
    TIMER("std::set count");
    for (int j = 0; j < max_size; ++j) {
      std_set.count(j);
    }
  }
  {
    TIMER("ft::set count");
    for (int j = 0; j < max_size; ++j) {
      ft_set.count(j);
    }
  }

  {
    TIMER("std::set find");
    for (int j = 0; j < max_size; ++j) {
      std_set.find(j);
    }
  }
  {
    TIMER("ft::set find");
    for (int j = 0; j < max_size; ++j) {
      ft_set.find(j);
    }
  }

  {
    TIMER("std::set equal_range");
    for (int j = 0; j < max_size; ++j) {
      std_set.equal_range(j);
    }
  }
  {
    TIMER("ft::set equal_range");
    for (int j = 0; j < max_size; ++j) {
      ft_set.equal_range(j);
    }
  }

  {
    TIMER("std::set lower_bound");
    for (int j = 0; j < max_size; ++j) {
      std_set.lower_bound(j);
    }
  }
  {
    TIMER("ft::set lower_bound");
    for (int j = 0; j < max_size; ++j) {
      ft_set.lower_bound(j);
    }
  }

  {
    TIMER("std::set upper_bound");
    for (int j = 0; j < max_size; ++j) {
      std_set.upper_bound(j);
    }
  }
  {
    TIMER("ft::set upper_bound");
    for (int j = 0; j < max_size; ++j) {
      ft_set.upper_bound(j);
    }
  }
}

}  // namespace