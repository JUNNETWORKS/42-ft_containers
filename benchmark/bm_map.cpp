#include <unistd.h>

#include <cstdlib>
#include <map>

#include "benchmarks.hpp"
#include "map.hpp"
#include "timer.hpp"

namespace {

inline void add_nums_to_map(std::map<int, int> &std_map,
                            ft::map<int, int> &ft_map, const int map_size) {
  for (int i = 0; i < map_size; ++i) {
    std_map[i] = i;
    ft_map[i] = i;
  }
}

void measure_map_constructor_and_assignation();
void measure_map_element_access_and_iterator();
void measure_map_capacity();
void measure_map_insert();
void measure_map_modifiers();
void measure_map_lookup();
}  // namespace

void measure_map() {
  measure_map_constructor_and_assignation();
  measure_map_element_access_and_iterator();
  measure_map_capacity();
  measure_map_insert();
  measure_map_modifiers();
  measure_map_lookup();
}

namespace {

void measure_map_constructor_and_assignation() {
  HEADER("measure_map_constructor_and_assignation");

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 100000;

  std_map_type std_map_for_copy;
  ft_map_type ft_map_for_copy;

  add_nums_to_map(std_map_for_copy, ft_map_for_copy, max_size);

  {
    TIMER("std::map constructor");
    std_map_type tmp;
  }
  {
    TIMER("ft::map constructor");
    ft_map_type tmp;
  }

  {
    TIMER("std::map copy constructor");
    std_map_type tmp(std_map_for_copy);
  }
  {
    TIMER("ft::map copy constructor");
    ft_map_type tmp(ft_map_for_copy);
  }

  {
    TIMER("std::map assignation");
    std_map_type tmp;
    tmp = std_map_for_copy;
  }
  {
    TIMER("ft::map assignation");
    ft_map_type tmp;
    tmp = ft_map_for_copy;
  }
}

void measure_map_element_access_and_iterator() {
  HEADER("measure_map_element_access_and_iterator");

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 1000000;

  std_map_type std_map;
  ft_map_type ft_map;

  add_nums_to_map(std_map, ft_map, max_size);

  {
    TIMER("std::map at");
    for (int j = 0; j < max_size; ++j) {
      std_map.at(j);
    }
  }
  {
    TIMER("ft::map at");
    for (int j = 0; j < max_size; ++j) {
      ft_map.at(j);
    }
  }

  {
    TIMER("std::map operator[]");
    for (int j = 0; j < max_size; ++j) {
      std_map[j];
    }
  }
  {
    TIMER("ft::map operator[]");
    for (int j = 0; j < max_size; ++j) {
      ft_map[j];
    }
  }

  {
    TIMER("std::map forward iterator");
    for (std_map_type::iterator it = std_map.begin(); it != std_map.end();
         ++it) {
      *it;
    }
  }
  {
    TIMER("ft::map forward iterator");
    for (ft_map_type::iterator it = ft_map.begin(); it != ft_map.end(); ++it) {
      *it;
    }
  }

  {
    TIMER("std::map reverse iterator");
    for (std_map_type::reverse_iterator it = std_map.rbegin();
         it != std_map.rend(); ++it) {
      *it;
    }
  }
  {
    TIMER("ft::map reverse iterator");
    for (ft_map_type::reverse_iterator it = ft_map.rbegin();
         it != ft_map.rend(); ++it) {
      *it;
    }
  }
}

void measure_map_capacity() {
  HEADER("measure_map_capacity");

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 1000000;

  std_map_type std_map;
  ft_map_type ft_map;

  add_nums_to_map(std_map, ft_map, max_size);

  {
    TIMER("std::map empty");
    std_map.empty();
  }
  {
    TIMER("ft::map empty");
    ft_map.empty();
  }

  {
    TIMER("std::map size");
    std_map.size();
  }
  {
    TIMER("ft::map size");
    ft_map.size();
  }

  {
    TIMER("std::map max_size");
    std_map.max_size();
  }
  {
    TIMER("ft::map max_size");
    ft_map.max_size();
  }
}

void measure_map_insert() {
  HEADER("measure_map_insert")
  srand(time(NULL));

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 1000000;

  std_map_type std_map;
  ft_map_type ft_map;

  {
    TIMER("std::map insert");
    for (int j = 0; j < max_size; ++j) {
      std_map[rand()] = rand();
    }
  }
  {
    TIMER("ft::map insert");
    for (int j = 0; j < max_size; ++j) {
      ft_map[rand()] = rand();
    }
  }
}

void measure_map_modifiers() {
  HEADER("measure_map_modifiers");

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 1000000;

  std_map_type std_map;
  ft_map_type ft_map;

  {
    TIMER("std::map insert");
    for (int j = 0; j < max_size; ++j) {
      std_map.insert(std_map_type::value_type(j, j));
    }
  }
  {
    TIMER("ft::map insert");
    for (int j = 0; j < max_size; ++j) {
      ft_map.insert(ft_map_type::value_type(j, j));
    }
  }

  {
    TIMER("std::map clear");
    for (int j = 0; j < max_size; ++j) {
      std_map.clear();
    }
  }
  {
    TIMER("ft::map clear");
    for (int j = 0; j < max_size; ++j) {
      ft_map.clear();
    }
  }

  add_nums_to_map(std_map, ft_map, max_size);

  {
    TIMER("std::map erase");
    for (int j = 0; j < max_size; ++j) {
      std_map.erase(j);
    }
  }
  {
    TIMER("ft::map erase");
    for (int j = 0; j < max_size; ++j) {
      ft_map.erase(j);
    }
  }
}

void measure_map_lookup() {
  HEADER("measure_map_lookup");

  typedef std::map<int, int> std_map_type;
  typedef ft::map<int, int> ft_map_type;

  const int max_size = 1000000;

  std_map_type std_map;
  ft_map_type ft_map;

  {
    TIMER("std::map count");
    for (int j = 0; j < max_size; ++j) {
      std_map.count(j);
    }
  }
  {
    TIMER("ft::map count");
    for (int j = 0; j < max_size; ++j) {
      ft_map.count(j);
    }
  }

  {
    TIMER("std::map find");
    for (int j = 0; j < max_size; ++j) {
      std_map.find(j);
    }
  }
  {
    TIMER("ft::map find");
    for (int j = 0; j < max_size; ++j) {
      ft_map.find(j);
    }
  }

  {
    TIMER("std::map equal_range");
    for (int j = 0; j < max_size; ++j) {
      std_map.equal_range(j);
    }
  }
  {
    TIMER("ft::map equal_range");
    for (int j = 0; j < max_size; ++j) {
      ft_map.equal_range(j);
    }
  }

  {
    TIMER("std::map lower_bound");
    for (int j = 0; j < max_size; ++j) {
      std_map.lower_bound(j);
    }
  }
  {
    TIMER("ft::map lower_bound");
    for (int j = 0; j < max_size; ++j) {
      ft_map.lower_bound(j);
    }
  }

  {
    TIMER("std::map upper_bound");
    for (int j = 0; j < max_size; ++j) {
      std_map.upper_bound(j);
    }
  }
  {
    TIMER("ft::map upper_bound");
    for (int j = 0; j < max_size; ++j) {
      ft_map.upper_bound(j);
    }
  }
}

}  // namespace