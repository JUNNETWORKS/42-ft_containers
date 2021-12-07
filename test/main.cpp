#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
#include <vector>

#include "../srcs/type_traits.hpp"

// stdバージョン
// template <typename T>
// void thisIsInteger(
//     T x, typename std::enable_if<std::is_integral<T>::value>::type* = 0) {
//   std::cout << "This is integer" << std::endl;
// }

template <typename T>
void thisIsInteger(
    T x, typename ft::enable_if<ft::is_integral<T>::value>::type* = 0) {
  std::cout << "This is integer" << std::endl;
}

int main(int argc, char** argv) {
  /* enable_if, is_integral */
  { thisIsInteger<int>(10); }

  /* vector */
  {
    std::vector<int> a(10);
    std::cout << "----- a -----" << std::endl;
    std::cout << "size: " << a.size() << std::endl;
    std::cout << "max_size: " << a.max_size() << std::endl;
    std::cout << "capacity: " << a.capacity() << std::endl;
    std::cout << "empty: " << a.empty() << std::endl;
    std::vector<int>::iterator it = a.begin();

    std::vector<int> b;
    std::cout << "----- b -----" << std::endl;
    std::cout << "size: " << b.size() << std::endl;
    std::cout << "max_size: " << b.max_size() << std::endl;
    std::cout << "capacity: " << b.capacity() << std::endl;
    std::cout << "empty: " << b.empty() << std::endl;

    std::vector<int> c(0);
    std::cout << "----- c -----" << std::endl;
    std::cout << "size: " << c.size() << std::endl;
    std::cout << "max_size: " << c.max_size() << std::endl;
    std::cout << "capacity: " << c.capacity() << std::endl;
    std::cout << "empty: " << c.empty() << std::endl;
  }

  /* map */
  {}

  /* stack */
  {}

  /* set (bonus) */
  {}
}