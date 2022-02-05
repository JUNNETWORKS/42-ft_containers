#ifndef TEST_UTILS_STRING_H_
#define TEST_UTILS_STRING_H_

#include <string>

namespace ft {
namespace test {

// https://stackoverflow.com/a/440240/8128376
std::string generate_random_string(const int len) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

}  // namespace test

}  // namespace ft

#endif