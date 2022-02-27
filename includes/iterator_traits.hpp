#ifndef ITERATOR_TRAITS_H_
#define ITERATOR_TRAITS_H_

#include <cstddef>
#include <iterator>

template <typename _Iterator>
struct iterator_traits {
  typedef typename _Iterator::iterator_category iterator_category;
  typedef typename _Iterator::value_type value_type;
  typedef typename _Iterator::difference_type difference_type;
  typedef typename _Iterator::pointer pointer;
  typedef typename _Iterator::reference reference;
};

/// Partial specialization for pointer types.
template <typename _Tp>
struct iterator_traits<_Tp*> {
  typedef std::random_access_iterator_tag iterator_category;
  typedef _Tp value_type;
  typedef std::ptrdiff_t difference_type;
  typedef _Tp* pointer;
  typedef _Tp& reference;
};

/// Partial specialization for const pointer types.
template <typename _Tp>
struct iterator_traits<const _Tp*> {
  typedef std::random_access_iterator_tag iterator_category;
  typedef _Tp value_type;
  typedef std::ptrdiff_t difference_type;
  typedef const _Tp* pointer;
  typedef const _Tp& reference;
};

#endif