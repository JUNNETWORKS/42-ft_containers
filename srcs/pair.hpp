#ifndef PAIR_H_
#define PAIR_H_

namespace ft {
template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  pair() : first(), second() {}

  pair(const T1& x, const T2& y) : first(x), second(y) {}

  pair(const pair& other) : first(other.first), second(other.second) {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& other) : first(other.first), second(other.second) {}

  pair& operator=(const pair& other) {
    if (this != &other) {
      first = other.first;
      second = other.second;
    }
    return *this;
  }

  T1 first;
  T2 second;
};

template <class T1, class T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs == rhs);
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  if (lhs.first < rhs.first)
    return true;
  if (lhs.first > rhs.first)
    return false;
  if (lhs.second < rhs.second)
    return true;
  return false;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs > rhs);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return rhs < lhs;
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs < rhs);
}

template <class T1, class T2>
pair<T1, T2> make_pair(T1 t, T2 u) {
  return pair<T1, T2>(t, u);
}

}  // namespace ft

#endif