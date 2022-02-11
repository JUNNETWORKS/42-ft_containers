#ifndef STACK_H_
#define STACK_H_

#include "vector.hpp"

namespace ft {
/*
The underlying container may be any of the standard container class templates or
some other specifically designed container class. The container shall support
the following operations:
  - empty
  - size
  - back
  - push_back
  - pop_back
*/
template <class T, class Container = vector<T> >
class stack {
 public:
  typedef T value_type;
  typedef Container container_type;
  typedef std::size_t size_type;

 protected:
  Container c;

 public:
  template <class T2, class Container2>
  friend bool operator==(const stack<T2, Container2>& lhs,
                         const stack<T2, Container2>& rhs);

  template <class T2, class Container2>
  friend bool operator<(const stack<T2, Container2>& lhs,
                        const stack<T2, Container2>& rhs);

  explicit stack(const container_type& container = container_type())
      : c(container) {}

  stack(const stack<T, Container>& other) : c(other.c) {}

  const stack<T, Container>& operator=(const stack<T, Container>& rhs) {
    if (this != &rhs) {
      c = rhs.c;
    }
    return *this;
  }

  ~stack() {}

  bool empty() const {
    return c.empty();
  }

  size_type size() const {
    return c.size();
  }

  value_type& top() {
    return c.back();
  }

  const value_type& top() const {
    return c.back();
  }

  void push(const value_type& val) {
    c.push_back(val);
  }

  void pop() {
    c.pop_back();
  }
};

template <class T, class Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return lhs.c == rhs.c;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return lhs.c < rhs.c;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs > rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return rhs < lhs;
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

#endif