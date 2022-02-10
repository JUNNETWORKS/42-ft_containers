#include "bm_map.cpp"
#include "bm_set.cpp"
#include "bm_stack.cpp"
#include "bm_vector.cpp"
#include "timer.hpp"
#include "unistd.h"

int main() {
  measure_vector();
  measure_stack();
  measure_map();
  measure_set();
  return 0;
}
