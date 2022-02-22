#ifndef REVERSE_ITERATOR
#define REVERSE_ITERATOR

#include <iterator>

namespace ft {

// 24.4.1 Reverse iterators
/**
 *  Bidirectional and random access iterators have corresponding reverse
 *  %iterator adaptors that iterate through the data structure in the
 *  opposite direction.  They have the same signatures as the corresponding
 *  iterators.  The fundamental relation between a reverse %iterator and its
 *  corresponding %iterator @c i is established by the identity:
 *  @code
 *      &*(reverse_iterator(i)) == &*(i - 1)
 *  @endcode
 *
 *  <em>This mapping is dictated by the fact that while there is always a
 *  pointer past the end of an array, there might not be a valid pointer
 *  before the beginning of an array.</em> [24.4.1]/1,2
 *
 *  Reverse iterators can be tricky and surprising at first.  Their
 *  semantics make sense, however, and the trickiness is a side effect of
 *  the requirement that the iterators must be safe.
 */

template <typename Iterator>
class reverse_iterator {
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

  /**
   *  The default constructor value-initializes member @p current.
   *  If it is a pointer, that means it is zero-initialized.
   */
  reverse_iterator() : current_(Iterator()) {}

  /**
   *  This %iterator will move in the opposite direction that @p x does.
   */
  explicit reverse_iterator(iterator_type it) : current_(it) {}

  /**
   *  The copy constructor is normal.
   */
  reverse_iterator(const reverse_iterator& it) : current_(it.current_) {}

  reference operator=(const reference rhs) {
    if (this != &rhs) {
      current_ = rhs.current_;
    }
    return *this;
  }

  /**
   *  A %reverse_iterator across other types can be copied if the
   *  underlying %iterator can be converted to the type of @c current.
   */
  template <typename Iter>
  reverse_iterator(const reverse_iterator<Iter>& it) : current_(it.base()) {}

  iterator_type base() const {
    return current_;
  }

  /**
   *  @return  A reference to the value at @c --current
   *
   *  This requires that @c --current is dereferenceable.
   *
   *  @warning This implementation requires that for an iterator of the
   *           underlying iterator type, @c x, a reference obtained by
   *           @c *x remains valid after @c x has been modified or
   *           destroyed. This is a bug: http://gcc.gnu.org/PR51823
   */
  reference operator*() const {
    Iterator tmp = current_;
    return *--tmp;
  }

  /**
   *  @return  A pointer to the value at @c --current
   *
   *  This requires that @c --current is dereferenceable.
   */
  pointer operator->() const {
    Iterator tmp = current_;
    --tmp;
    return to_pointer(tmp);
  }

  /**
   *  @return  @c *this
   *
   *  Decrements the underlying iterator.
   */
  reverse_iterator& operator++() {
    --current_;
    return *this;
  }

  /**
   *  @return  The original value of @c *this
   *
   *  Decrements the underlying iterator.
   */
  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    --current_;
    return tmp;
  }

  /**
   *  @return  @c *this
   *
   *  Increments the underlying iterator.
   */
  reverse_iterator& operator--() {
    ++current_;
    return *this;
  }

  /**
   *  @return  A reverse_iterator with the previous value of @c *this
   *
   *  Increments the underlying iterator.
   */
  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    ++current_;
    return tmp;
  }

  /**
   *  @return  A reverse_iterator that refers to @c current - @a __n
   *
   *  The underlying iterator must be a Random Access Iterator.
   */
  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(current_ - n);
  }

  /**
   *  @return  *this
   *
   *  Moves the underlying iterator backwards @a __n steps.
   *  The underlying iterator must be a Random Access Iterator.
   */
  reverse_iterator& operator+=(difference_type n) {
    current_ -= n;
    return *this;
  }

  /**
   *  @return  A reverse_iterator that refers to @c current - @a __n
   *
   *  The underlying iterator must be a Random Access Iterator.
   */
  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(current_ + n);
  }

  /**
   *  @return  *this
   *
   *  Moves the underlying iterator forwards @a __n steps.
   *  The underlying iterator must be a Random Access Iterator.
   */
  reverse_iterator& operator-=(difference_type n) {
    current_ += n;
    return *this;
  }

  /**
   *  @return  The value at @c current - @a __n - 1
   *
   *  The underlying iterator must be a Random Access Iterator.
   */
  reference operator[](difference_type n) const {
    return *(*this + n);
  }

 private:
  template <typename T>
  static T* to_pointer(T* p) {
    return p;
  }

  template <typename T>
  static pointer to_pointer(T t) {
    return t.operator->();
  }
};

template <typename IteratorL, typename IteratorR>
inline bool operator==(const reverse_iterator<IteratorL>& lhs,
                       const reverse_iterator<IteratorR>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename IteratorL, typename IteratorR>
inline bool operator<(const reverse_iterator<IteratorL>& lhs,
                      const reverse_iterator<IteratorR>& rhs) {
  return rhs.base() < lhs.base();
}

template <typename IteratorL, typename IteratorR>
inline bool operator!=(const reverse_iterator<IteratorL>& lhs,
                       const reverse_iterator<IteratorR>& rhs) {
  return !(lhs == rhs);
}

template <typename IteratorL, typename IteratorR>
inline bool operator>(const reverse_iterator<IteratorL>& lhs,
                      const reverse_iterator<IteratorR>& rhs) {
  return rhs < lhs;
}

template <typename IteratorL, typename IteratorR>
inline bool operator<=(const reverse_iterator<IteratorL>& lhs,
                       const reverse_iterator<IteratorR>& rhs) {
  return !(rhs < lhs);
}

template <typename IteratorL, typename IteratorR>
inline bool operator>=(const reverse_iterator<IteratorL>& lhs,
                       const reverse_iterator<IteratorR>& rhs) {
  return !(lhs < rhs);
}

template <typename Iterator, typename Container>
inline reverse_iterator<Iterator> operator+(
    const typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& it) {
  return it + n;
}

template <typename IteratorL, typename IteratorR, typename Container>
inline typename reverse_iterator<IteratorL>::difference_type operator-(
    const reverse_iterator<IteratorL>& lhs,
    const reverse_iterator<IteratorR>& rhs) {
  return lhs.base() - rhs.base();
}

}  // namespace ft

#endif