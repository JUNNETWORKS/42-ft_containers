#include <vector>

#include "timer.hpp"
#include "unistd.h"

int main() {
  std::vector<int> vec;
  {
    Timer t("Test vector push_back");
    for (int i = 0; i < 100000000; ++i) {
      vec.push_back(i);
    }
  }
  return 0;
}
