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

  template <class T2, class Container2>
  friend bool operator==(const stack<T2, Container2>& lhs,
                         const stack<T2, Container2>& rhs);

  template <class T2, class Container2>
  friend bool operator<(const stack<T2, Container2>& lhs,
                        const stack<T2, Container2>& rhs);

  explicit stack(const container_type& container = container_type())
      : container_(container) {}

  stack(const stack<T, Container>& other) : container_(other.container_) {}

  const stack<T, Container>& operator=(const stack<T, Container>& rhs) {
    if (this != &rhs) {
      container_ = rhs.container_;
    }
    return *this;
  }

  ~stack() {}

  bool empty() const {
    return container_.empty();
  }

  size_type size() const {
    return container_.size();
  }

  value_type& top() {
    return container_.back();
  }

  const value_type& top() const {
    return container_.back();
  }

  void push(const value_type& val) {
    container_.push_back(val);
  }

  void pop() {
    container_.pop_back();
  }

 private:
  Container container_;
};

template <class T, class Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return lhs.container_ == rhs.container_;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return lhs.container_ < rhs.container_;
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