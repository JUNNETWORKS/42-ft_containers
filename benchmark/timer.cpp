#include "timer.hpp"

Timer::Timer(const int loop_num) : loop_num_(loop_num) {
  clock_gettime(CLOCK_MONOTONIC, &real_start_);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start_);
}

Timer::~Timer() {
  timespec real_end;
  timespec cpu_end;

  clock_gettime(CLOCK_MONOTONIC, &real_end);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end);

  int64_t real_diff_ns =
      (real_end.tv_sec - real_start_.tv_sec) * 1000000000 / loop_num_ +
      (real_end.tv_nsec - real_start_.tv_nsec) / loop_num_;
  int64_t cpu_diff_ns =
      (cpu_end.tv_sec - cpu_start_.tv_sec) * 1000000000 / loop_num_ +
      (cpu_end.tv_nsec - cpu_start_.tv_nsec) / loop_num_;

  std::cout << "Real: " << real_diff_ns << "[ns]\n";
  std::cout << "CPU:  " << cpu_diff_ns << "[ns]\n";
  std::cout << std::endl;
}