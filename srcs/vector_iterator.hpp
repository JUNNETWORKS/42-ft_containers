#ifndef VECTOR_ITERATOR_H_
#define VECTOR_ITERATOR_H_

#include <cstddef>
#include <iterator>

#include "vector.hpp"

// explanation of iterator is on http://www.cplusplus.com/reference/iterator/

namespace ft {

template <typename, typename>
class vector;

template <typename T>
class vector_iterator
    : public std::iterator<std::random_access_iterator_tag, T> {
  template <typename U>
  friend bool operator==(const vector_iterator<U> &lhs,
                         const vector_iterator<U> &rhs);
  template <typename U>
  friend bool operator!=(const vector_iterator<U> &lhs,
                         const vector_iterator<U> &rhs);
  template <typename U>
  friend vector_iterator<U> &operator+(vector_iterator<U> &lhs, int n);
  template <typename U>
  friend vector_iterator<U> &operator+(int n, vector_iterator<U> &lhs);
  template <typename U>
  friend vector_iterator<U> &operator-(vector_iterator<U> &lhs, int n);
  template <typename U>
  friend vector_iterator<U> &operator-(int n, vector_iterator<U> &lhs);

 public:
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type &reference;
  typedef value_type *pointer;

  vector_iterator() : ptr_(NULL) {}

  vector_iterator(pointer ptr) : ptr_(ptr) {}

  vector_iterator(const vector_iterator<value_type> &it) {
    *this = it;
  }

  vector_iterator<value_type> &operator=(
      const vector_iterator<value_type> &rhs) {
    if (this != &rhs) {
      ptr_ = rhs.ptr_;
    }
    return *this;
  }

  ~vector_iterator() {}

  vector_iterator<T> &operator++() {  // ++it
    ptr_++;
    return *this;
  }
  vector_iterator<T> operator++(int) {  // it++
    vector_iterator<T> old_value = ptr_;
    ptr_++;
    return old_value;
  }

  T &operator*() {  // *it
    return *ptr_;
  }
  T &operator->() {  // it->hoge
    return *ptr_;
  }

  /* Bidirectional iterator has all properties of forward iterator and these
  properties below */
  vector_iterator<T> &operator--() {  // --it
    ptr_--;
    return *this;
  }
  vector_iterator<T> &operator--(int) {  // it--
    value_type old_value = *ptr_;
    ptr_--;
    return old_value;
  }

  /* Random Access iterator has all properties of Bidirectional iterator and
  these properties below. */
  bool operator<(vector_iterator<T> &rhs) {
    return ptr_ < rhs.ptr_;
  }
  bool operator>(vector_iterator<T> &rhs) {
    return ptr_ > rhs.ptr_;
  }
  bool operator<=(vector_iterator<T> &rhs) {
    return this < rhs || this == rhs;
  }
  bool operator>=(vector_iterator<T> &rhs) {
    return this > rhs || this == rhs;
  }
  vector_iterator &operator+=(int n) {
    ptr_ += n;
    return ptr_;
  }
  vector_iterator &operator-=(int n) {
    ptr_ -= n;
    return ptr_;
  }
  vector_iterator &operator[](int n) {
    return ptr_[n];
  }

 private:
  pointer ptr_;
};

template <typename T>
bool operator==(const vector_iterator<T> &lhs,
                const vector_iterator<T> &rhs) {  // it1 == it2
  return lhs.ptr_ == rhs.ptr_;
}
template <typename T>
bool operator!=(const vector_iterator<T> &lhs,
                const vector_iterator<T> &rhs) {  // it1 != it2
  return !(lhs == rhs);
}
template <typename T>
vector_iterator<T> &operator+(vector_iterator<T> &lhs, int n) {
  return lhs.ptr_ + n;
}
template <typename T>
vector_iterator<T> &operator+(int n, vector_iterator<T> &rhs) {
  return rhs.ptr_ + n;
}
template <typename T>
vector_iterator<T> &operator-(vector_iterator<T> &lhs, int n) {
  return lhs.ptr_ + n;
}
template <typename T>
vector_iterator<T> &operator-(int n, vector_iterator<T> &rhs) {
  return rhs.ptr_ + n;
}

}  // namespace ft

#endif