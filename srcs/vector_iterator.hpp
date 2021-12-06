#ifndef VECTOR_ITERATOR_H_
#define VECTOR_ITERATOR_H_

#include <iterator>

// explanation of iterator is on http://www.cplusplus.com/reference/iterator/

namespace ft {

  template <typename T>
  class vector;

template <typename T>
class vector_iterator
    : public std::iterator<std::random_access_iterator_tag, T> {
  template <typename U>
  friend vector_iterator<U> &operator+(vector_iterator<U> &lhs, int n);
  template <typename U>
  friend vector_iterator<U> &operator+(int n, vector_iterator<U> &lhs);
  template <typename U>
  friend vector_iterator<U> &operator-(vector_iterator<U> &lhs, int n);
  template <typename U>
  friend vector_iterator<U> &operator-(int n, vector_iterator<U> &lhs);

  vector_iterator(pointer ptr) : (ptr_ = ptr) {}

  vector_iterator(vector_iterator<value_type> &it) {
    *this = it;
  }

  vector_iterator<value_type> &operator=(vector_iterator<value_type> &rhs) {
    if (this != &rhs) {
      ptr_ = rhs.ptr_;
    }
  }

  ~vector_iterator() {}

  /* All iterator type has these properties */
  vector_iterator<T> &operator++() {  // ++it
    ptr_++;
    return *this;
  }
  vector_iterator<T> &operator++(int) {  // it++
    vector::value_type old_value = *ptr_;
    ptr_++;
    return old_value;
  }

  /* Input iterator properties */
  bool operator==(vector_iterator<T> &rhs) {  // it1 == it2
    return ptr_ == rhs.ptr_;
  }
  bool operator!=(vector_iterator<T> &rhs) {  // it1 != it2
    return !(this == rhs);
  }
  T &operator*() {  // *it
    return *ptr_;
  }
  T &operator->() {  // it->hoge
    return *ptr_;
  }

  /* Forward iterator = Input iterator + default constructor */
  vector_iterator() : ptr_(nullptr);

  /* Bidirectional iterator has all properties of forward iterator and these
  properties below */
  vector_iterator<T> &operator--() {  // --it
    ptr_--;
    return *this;
  }
  vector_iterator<T> &operator--(int) {  // it--
    vector::value_type old_value = *ptr_;
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
  vector::pointer ptr_;
};

/* Futhermore, Random Access iterator has some non member operator overloads
below */
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
