#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <iomanip>
#include <iostream>

long long success_count = 0;
long long failed_count = 0;

#define COL_RED "\x1B[31m"
#define COL_GREEN "\x1B[32m"
#define COL_YELLOW "\x1B[33m"
#define COL_BLUE "\x1B[34m"
#define COL_RESET "\x1B[0m"

#define TEST_SECTION(msg)                                         \
  std::cout << "========== " << std::left << std::setw(20) << msg \
            << " ==========" << std::endl
#define TEST_SUBSECTION(msg) \
  std::cout << "-- " << std::left << std::setw(20) << msg << " --" << std::endl

/* ASSERTIONS */

#define ASSERT_TRUE(message, condition)                                      \
  do {                                                                       \
    if (condition) {                                                         \
      std::cout << COL_GREEN << "\t✔ " << message << COL_RESET << std::endl; \
      success_count++;                                                       \
    } else {                                                                 \
      std::cerr << COL_RED << "\t✘ " << message << COL_RESET << std::endl;   \
      failed_count++;                                                        \
    }                                                                        \
  } while (0)

#define ASSERT_FALSE(message, condition) \
  do {                                   \
    ASSERT_TRUE(message, !condition);    \
  } while (0)

#define ASSERT_EQ(message, expected, actual)  \
  do {                                        \
    ASSERT_TRUE(message, expected == actual); \
  } while (0)

void print_test_result() {
  std::cout << COL_GREEN << "passed: " << success_count << COL_RESET
            << std::endl;
  std::cout << COL_RED << "failed: " << failed_count << COL_RESET << std::endl;
}

#endif