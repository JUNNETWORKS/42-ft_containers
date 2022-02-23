#ifndef BENCHMARK_TIMER_H_
#define BENCHMARK_TIMER_H_

#include <stdint.h>
#include <time.h>

#include <iostream>
#include <string>

class Timer;

#define HEADER(title) \
  std::cout << "==========  " << title << "  ==========" << std::endl;

#define TIMER(title)          \
  std::cout << title << "\n"; \
  Timer t;

class Timer {
 public:
  Timer(const int loop_num = 1);
  ~Timer();

 private:
  const int loop_num_;
  timespec real_start_;
  timespec cpu_start_;

  Timer(const Timer &other);
  Timer &operator=(const Timer &other);
};

#endif