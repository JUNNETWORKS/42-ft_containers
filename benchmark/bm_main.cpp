#include "bm_stack.cpp"
#include "bm_vector.cpp"
#include "timer.hpp"
#include "unistd.h"

int main() {
  measure_vector();
  measure_stack();
  return 0;
}
