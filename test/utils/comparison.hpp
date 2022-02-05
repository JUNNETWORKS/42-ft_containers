#ifndef TEST_UTILS_COMPARISON_H_
#define TEST_UTILS_COMPARISON_H_

#include <functional>

namespace ft {
namespace test {

template <class T>
struct less_or_greater {
  bool is_less_;

  less_or_greater(bool is_less = true) : is_less_(is_less) {}

  bool operator()(const T &lhs, const T &rhs) const {
    if (is_less_) {
      return std::less<T>()(lhs, rhs);
    } else {
      return std::greater<T>()(lhs, rhs);
    }
  }
};

}  // namespace test
}  // namespace ft

#endif