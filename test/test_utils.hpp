#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <iostream>

#define COL_RED "\u001b[31m"
#define COL_GREEN "\u001b[32m"
#define COL_YELLOW "\u001b[33m"
#define COL_BLUE "\u001b[34m"
#define COL_RESET "\u001b[0m"

#define TEST_SECTION(msg) \
  std::cout << "========== " msg " ==========" << std::endl
#define TEST_SUBSECTION(msg) std::cout << "-- " msg " --" << std::endl

#define ASSERT_TRUE(message, expected, actual)                                 \
  do {                                                                         \
    if (expected != actual) {                                                  \
      std::cerr << COL_RED << "failed: " << message << COL_RESET << std::endl; \
    } else {                                                                   \
      std::cout << COL_GREEN << message << COL_RESET << std::endl;             \
    }                                                                          \
  } while (0)

#define ASSERT_EQ(message, expected, actual) \
  do {                                       \
    ASSERT_TRUE(message, expected, actual);  \
  } while (0)

#endif