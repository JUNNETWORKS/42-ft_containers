#include "benchmarks.hpp"
#include "timer.hpp"
#include "unistd.h"

int main() {
  measure_vector();
  measure_stack();
  measure_map();
  measure_set();
  return 0;
}
