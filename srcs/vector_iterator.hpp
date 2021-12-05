#ifndef VECTOR_ITERATOR_H_
#define VECTOR_ITERATOR_H_

// explanation of iterator is on http://www.cplusplus.com/reference/iterator/

namespace ft {

template <typename T>
class vector_iterator {
  vector_iterator(vector_iterator<T> &it) {
    *this = it;
  }

  vector_iterator<T> &operator=(vector_iterator<T> &rhs) {
    if (this != &rhs) {
    }
  }

  ~vector_iterator();

  /* All iterator type has these properties */
  vector_iterator<T> &operator++();     // ++it
  vector_iterator<T> &operator++(int);  // it++

  /* Input iterator properties */
  bool operator==(vector_iterator<T> &rhs);  // it1 == it2
  bool operator!=(vector_iterator<T> &rhs);  // it1 != it2

  /* Output iterator properties */
  T &operator*();  // *it

  /* Forward iterator = Input iterator + Output iterator + default constructor
   */
  vector_iterator();

  /* Bidirectional iterator has all properties of forward iterator and these
  properties below */
  vector_iterator<T> &operator--();     // --it
  vector_iterator<T> &operator--(int);  // it--

  /* Random Access iterator has all properties of Bidirectional iterator and
  these properties below. */
  bool operator<(vector_iterator<T> &rhs);
  bool operator>(vector_iterator<T> &rhs);
  bool operator<=(vector_iterator<T> &rhs);
  bool operator>=(vector_iterator<T> &rhs);
  vector_iterator &operator+=(int n);
  vector_iterator &operator-=(int n);
  vector_iterator &operator[](int n);
};

/* Futhermore, Random Access iterator has some non member operator overloads
below */
template <typename T>
vector_iterator<T> &operator+(vector_iterator<T> &lhs, int n);
template <typename T>
vector_iterator<T> &operator+(int n, vector_iterator<T> &lhs);
template <typename T>
vector_iterator<T> &operator-(vector_iterator<T> &lhs, int n);
template <typename T>
vector_iterator<T> &operator-(int n, vector_iterator<T> &lhs);

}  // namespace ft

#endif