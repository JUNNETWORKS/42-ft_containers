#ifndef DEBUG_UTILS_H_
#define DEBUG_UTILS_H_

#include "vector.hpp"

template <typename T>
void printVectorInfo(std::vector<T>& vec) {
  std::cout << "STL Vector Information"
            << "\tsize: " << vec.size() << "\n"
            << "\tcap: " << vec.capacity() << std::endl;
}

template <typename T>
void printVector(std::vector<T>& vec) {
  std::cout << "STL Vector Data" << std::endl;
  for (typename std::vector<T>::const_iterator it = vec.begin();
       it != vec.end(); ++it) {
    std::cout << "\t" << *it << std::endl;
  }
}

template <typename T>
void printVectorInfo(ft::vector<T>& vec) {
  std::cout << "Ft Vector Information"
            << "\tsize: " << vec.size() << "\n"
            << "\tcap: " << vec.capacity() << std::endl;
}

template <typename T>
void printVector(ft::vector<T>& vec) {
  std::cout << "Ft Vector Data" << std::endl;
  for (typename ft::vector<T>::const_iterator it = vec.begin(); it != vec.end();
       ++it) {
    std::cout << "\t" << *it << std::endl;
  }
}

#endif