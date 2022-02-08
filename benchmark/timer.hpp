#ifndef BENCHMARK_TIMER_H_
#define BENCHMARK_TIMER_H_

#include <stdint.h>
#include <time.h>

#include <iostream>
#include <string>

class Timer {
 public:
  Timer(const std::string &name = "") : name_(name) {
    std::cout << "Start " << name_ << std::endl;
    clock_gettime(CLOCK_MONOTONIC, &real_start_);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start_);
  }

  ~Timer() {
    timespec real_end;
    timespec cpu_end;

    clock_gettime(CLOCK_MONOTONIC, &real_end);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end);

    std::cout << "Real: " << (real_end.tv_sec - real_start_.tv_sec) << "."
              << (real_end.tv_nsec - real_start_.tv_nsec) << "\n";
    std::cout << "CPU:  " << (cpu_end.tv_sec - cpu_start_.tv_sec) << "."
              << (cpu_end.tv_nsec - cpu_start_.tv_nsec) << "\n";
  }

 private:
  std::string name_;
  timespec real_start_;
  timespec cpu_start_;
};

#endif