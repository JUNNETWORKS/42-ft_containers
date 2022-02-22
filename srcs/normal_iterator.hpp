#ifndef NORMAL_ITERATOR_H_
#define NORMAL_ITERATOR_H_

#include "iterator_traits.hpp"
#include "type_traits.hpp"

namespace ft {

// This iterator adapter is @a normal in the sense that it does not
// change the semantics of any of the operators of its iterator
// parameter.  Its primary purpose is to convert an iterator that is
// not a class, e.g. a pointer, into an iterator that is a class.
// The Container parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// Iterator parameter is the same.
// このクラスはポインタなどのクラスではないイテレータをポインタに変換する.
// Iterator はポインタ, もしくは同等のインターフェースを持つものを想定する
// Containerは_Iteratorが同じでも別の型としてインスタンス化するためにある.
template <typename Iterator, typename Container>
class normal_iterator {
 protected:
  Iterator current_;

  typedef iterator_traits<Iterator> traits_type;

 public:
  typedef Iterator iterator_type;
  typedef typename traits_type::iterator_category iterator_category;
  typedef typename traits_type::value_type value_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::reference reference;
  typedef typename traits_type::pointer pointer;

  normal_iterator() : current_(Iterator()) {}

  explicit normal_iterator(const Iterator& it) : current_(it) {}

  // Allow iterator to const_iterator conversion
  template <typename Iter>
  normal_iterator(
      const normal_iterator<
          Iter,
          typename enable_if<is_same<Iter, typename Container::pointer>::value,
                             Container>::type>& it)
      : current_(it.base()) {}

  reference operator=(const reference rhs) {
    if (this != &rhs) {
      current_ = rhs.current_;
    }
    return *this;
  }

  // Forward iterator requirements
  reference operator*() const {
    return *current_;
  }

  pointer operator->() const {
    return current_;
  }

  normal_iterator& operator++() {
    ++current_;
    return *this;
  }

  normal_iterator operator++(int) {
    return normal_iterator(current_++);
  }

  // Bidirectional iterator requirements
  normal_iterator& operator--() {
    --current_;
    return *this;
  }

  normal_iterator operator--(int) {
    return normal_iterator(current_--);
  }

  // Random access iterator requirements
  reference operator[](difference_type __n) const {
    return current_[__n];
  }

  normal_iterator& operator+=(difference_type __n) {
    current_ += __n;
    return *this;
  }

  normal_iterator operator+(difference_type __n) const {
    return normal_iterator(current_ + __n);
  }

  normal_iterator& operator-=(difference_type __n) {
    current_ -= __n;
    return *this;
  }

  normal_iterator operator-(difference_type __n) const {
    return normal_iterator(current_ - __n);
  }

  const Iterator& base() const {
    return current_;
  }
};

// Note: In what follows, the left- and right-hand-side iterators are
// allowed to vary in types (conceptually in cv-qualification) so that
// comparison between cv-qualified and non-cv-qualified iterators be
// valid.  However, the greedy and unfriendly operators in std::rel_ops
// will make overload resolution ambiguous (when in scope) if we don't
// provide overloads whose operands are of the same type.  Can someone
// remind me what generic programming is about? -- Gaby

// Forward iterator requirements
template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator==(const normal_iterator<IteratorL, Container>& lhs,
                       const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator==(const normal_iterator<Iterator, Container>& lhs,
                       const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator!=(const normal_iterator<IteratorL, Container>& lhs,
                       const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator!=(const normal_iterator<Iterator, Container>& lhs,
                       const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() != rhs.base();
}

// Random access iterator requirements
template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator<(const normal_iterator<IteratorL, Container>& lhs,
                      const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator<(const normal_iterator<Iterator, Container>& lhs,
                      const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator>(const normal_iterator<IteratorL, Container>& lhs,
                      const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator>(const normal_iterator<Iterator, Container>& lhs,
                      const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator<=(const normal_iterator<IteratorL, Container>& lhs,
                       const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator<=(const normal_iterator<Iterator, Container>& lhs,
                       const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename IteratorL, typename IteratorR, typename Container>
inline bool operator>=(const normal_iterator<IteratorL, Container>& lhs,
                       const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() >= rhs.base();
}

template <typename Iterator, typename Container>
inline bool operator>=(const normal_iterator<Iterator, Container>& lhs,
                       const normal_iterator<Iterator, Container>& rhs) {
  return lhs.base() >= rhs.base();
}

template <typename Iterator, typename Container>
inline normal_iterator<Iterator, Container> operator+(
    const typename normal_iterator<Iterator, Container>::difference_type n,
    const normal_iterator<Iterator, Container>& it) {
  return it + n;
}

template <typename IteratorL, typename IteratorR, typename Container>
inline typename normal_iterator<IteratorL, Container>::difference_type
operator-(const normal_iterator<IteratorL, Container>& lhs,
          const normal_iterator<IteratorR, Container>& rhs) {
  return lhs.base() - rhs.base();
}

}  // namespace ft

#endif