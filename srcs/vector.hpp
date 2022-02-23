#ifndef VECTOR_H_
#define VECTOR_H_
#include <iostream>
#include <memory>
#include <stdexcept>

#include "equal.hpp"
#include "lexicographical_compare.hpp"
#include "normal_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft {
template <typename T, typename Allocator = std::allocator<T> >
class vector {
 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef ft::normal_iterator<pointer, vector> iterator;
  typedef ft::normal_iterator<const_pointer, vector> const_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef std::size_t size_type;

 private:
  allocator_type allocator_;
  size_type cap_;
  pointer start_;
  pointer finish_;
  pointer end_of_storage_;

 public:
  vector(size_type n = 0, const value_type &val = value_type(),
         allocator_type alloc = allocator_type())
      : allocator_(alloc), cap_(n) {
    start_ = allocator_.allocate(cap_);
    end_of_storage_ = start_ + cap_;
    __uninitialized_fill_n(start_, n, val);
    finish_ = start_ + n;
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         allocator_type alloc = allocator_type(),
         typename disable_if<is_integral<InputIterator>::value>::type * = 0)
      : allocator_(alloc), cap_(), start_(), finish_(), end_of_storage_() {
    __range_initialize(
        first, last,
        typename iterator_traits<InputIterator>::iterator_category());
  }

  vector(const vector &other)
      : allocator_(other.allocator_),
        cap_(),
        start_(),
        finish_(),
        end_of_storage_() {
    operator=(other);
  }

  const vector<T, Allocator> &operator=(const vector<T, Allocator> &rhs) {
    if (this != &rhs) {
      size_type rhs_len = rhs.size();
      if (rhs_len > capacity()) {
        pointer tmp = allocator_.allocate(rhs_len);
        __uninitialized_copy(rhs.begin(), rhs.end(), tmp);
        __destroy(start_, finish_);
        allocator_.deallocate(start_, cap_);
        cap_ = rhs_len;
        start_ = tmp;
        finish_ = start_ + rhs_len;
        end_of_storage_ = start_ + rhs_len;
      } else if (rhs_len <= size()) {
        erase(std::copy(rhs.begin(), rhs.end(), begin()), end());
      } else {
        std::copy(rhs.start_, rhs.start_ + size(), start_);
        __uninitialized_copy(rhs.start_ + size(), rhs.finish_, finish_);
      }
      finish_ = start_ + rhs_len;
    }
    return *this;
  }

  ~vector() {
    __destroy(start_, finish_);
    allocator_.deallocate(start_, cap_);
  }

  // Iterators
  iterator begin() {
    return iterator(start_);
  }

  const_iterator begin() const {
    return const_iterator(start_);
  }

  iterator end() {
    return iterator(finish_);
  }

  const_iterator end() const {
    return const_iterator(finish_);
  }

  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // Size and Capacity

  size_type size() const {
    return size_type(finish_ - start_);
  }

  size_type max_size() const {
    return allocator_type().max_size();
  }

  size_type capacity() const {
    return cap_;
  }

  bool empty() const {
    return begin() == end();
  }

  void reserve(size_type n) {
    if (n > allocator_.max_size())
      throw std::length_error("vector::reserve");
    if (capacity() < n) {
      __expand_and_copy_storage(n);
    }
  }

  // Element access

  reference operator[](size_type n) {
    return start_[n];
  }

  const_reference operator[](size_type n) const {
    return start_[n];
  }

  reference at(size_type n) {
    if (n >= size())
      throw std::out_of_range("vector::at");
    return start_[n];
  }

  const_reference at(size_type n) const {
    if (n >= size())
      throw std::out_of_range("vector::at");
    return start_[n];
  }

  reference front() {
    return *begin();
  }

  const_reference front() const {
    return *begin();
  }

  reference back() {
    iterator tmp = end();
    --tmp;
    return *tmp;
  }

  const_reference back() const {
    const_iterator tmp = end();
    --tmp;
    return *tmp;
  }

  pointer data() {
    return start_;
  }

  const_pointer data() const {
    return start_;
  }

  // Modifiers
  // TODO: assginが激遅！！
  template <class InputIterator>
  void assign(
      InputIterator first, InputIterator last,
      typename disable_if<is_integral<InputIterator>::value>::type * = 0) {
    __assign_range(
        first, last,
        typename iterator_traits<InputIterator>::iterator_category());
  }

  void assign(size_type n, const value_type &val) {
    if (n > capacity()) {
      vector<T> tmp(n, val);
      swap(tmp);
    } else if (n > size()) {
      std::fill(begin(), end(), val);
      size_type remain = n - size();
      __uninitialized_fill_n(finish_, remain, val);
      finish_ = start_ + n;
    } else {
      std::fill_n(start_, n, val);
      erase(begin() + n, end());
      finish_ = start_ + n;
    }
  }

  void push_back(const value_type &val) {
    if (size() == capacity()) {
      __expand_and_copy_storage(__calc_new_capacity(capacity()));
    }
    allocator_.construct(finish_, val);
    ++finish_;
  }

  void pop_back() {
    if (size() == 0) {
      return;
    }
    --finish_;
    allocator_.destroy(finish_);
  }

  void resize(size_type n, T value = T()) {
    if (n < size()) {
      erase(begin() + n, end());
    } else {
      for (size_type i = size(); i < n; ++i) {
        push_back(value);
      }
    }
  }

  iterator insert(iterator position, const value_type &val) {
    return __insert_n_val(position, 1, val);
  }

  void insert(iterator position, size_type n, const value_type &val) {
    __insert_n_val(position, n, val);
  }

  template <class InputIterator>
  void insert(
      iterator position, InputIterator first, InputIterator last,
      typename disable_if<is_integral<InputIterator>::value>::type * = 0) {
    __insert_range(
        position, first, last,
        typename iterator_traits<InputIterator>::iterator_category());
  }

  iterator erase(iterator position) {
    if (position + 1 != end())
      std::copy(position + 1, end(), position);
    --finish_;
    allocator_.destroy(finish_);
    return position;
  }

  iterator erase(iterator first, iterator last) {
    if (first != last) {
      if (last != end()) {
        std::copy(last, end(), first);
      }
      pointer new_finish = first.base() + (end() - last);
      __destroy(new_finish, finish_);
      finish_ = new_finish;
    }
    return first;
  }

  void swap(vector<T> &x) {
    std::swap(cap_, x.cap_);
    std::swap(start_, x.start_);
    std::swap(finish_, x.finish_);
    std::swap(end_of_storage_, x.end_of_storage_);
  }

  void clear() {
    while (size()) {
      pop_back();
    }
  }

  // Allocator
  allocator_type get_allocator() const {
    return allocator_type(allocator_);
  }

 private:
  void __destroy(pointer first, pointer last) {
    for (; first != last; ++first) {
      allocator_.destroy(first);
    }
  }

  template <class ForwardIterator, class Size, class _T>
  void __uninitialized_fill_n(ForwardIterator first, Size count,
                              const _T &value) {
    typedef typename iterator_traits<ForwardIterator>::value_type V;

    ForwardIterator current = first;
    try {
      for (size_type i = 0; i < count; ++i, ++current) {
        allocator_.construct(current, value);
      }
    } catch (...) {
      for (; first != current; ++first) {
        first->~V();
      }
      throw;
    }
  }

  template <class InputIt, class ForwardIterator>
  ForwardIterator __uninitialized_copy(InputIt first, InputIt last,
                                       ForwardIterator d_first) {
    typedef typename iterator_traits<ForwardIterator>::value_type V;

    ForwardIterator current = d_first;
    try {
      for (; first != last; ++first, ++current) {
        allocator_.construct(current, *first);
      }
      return current;
    } catch (...) {
      for (; d_first != current; ++d_first) {
        d_first->~V();
      }
      throw;
    }
  }

  template <class InputIterator>
  void __range_initialize(InputIterator first, InputIterator last,
                          std::input_iterator_tag) {
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  template <class ForwardIterator>
  void __range_initialize(ForwardIterator first, ForwardIterator last,
                          std::forward_iterator_tag) {
    const size_type len = std::distance(first, last);
    if (len > max_size())
      throw std::length_error(
          "cannot create std::vector larger than max_size()");
    cap_ = len;
    start_ = allocator_.allocate(len);
    end_of_storage_ = start_ + len;
    __uninitialized_copy(first, last, start_);
    finish_ = start_ + len;
  }

  void __expand_and_copy_storage(size_type new_cap) {
    if (size() == 0) {
      // 要素がない場合は新しい領域を確保するだけ
      allocator_.deallocate(start_, cap_);
      cap_ = new_cap;
      start_ = allocator_.allocate(cap_);
      finish_ = start_;
      end_of_storage_ = start_ + cap_;
    } else {
      vector<T> tmp;
      tmp.reserve(new_cap);
      tmp = *this;
      swap(tmp);
    }
  }

  inline size_type __calc_new_capacity(size_type current_capacity) {
    if (current_capacity == 0) {
      return 1;
    }
    return current_capacity * 2;
  }

  template <class InputIterator>
  void __assign_range(InputIterator first, InputIterator last,
                      std::input_iterator_tag) {
    iterator current = begin();
    for (; first != last && current != end(); ++current, ++first) {
      *current = *first;
    }
    if (first == last) {
      erase(current, end());
    } else {
      insert(end(), first, last);
    }
  }

  template <class ForwardIterator>
  void __assign_range(ForwardIterator first, ForwardIterator last,
                      std::forward_iterator_tag) {
    size_type len = std::distance(first, last);
    if (len > capacity()) {
      vector tmp_vec(first, last);
      swap(tmp_vec);
    } else if (len <= size()) {
      erase(std::copy(first, last, begin()), end());
    } else {
      ForwardIterator mid = first;
      std::advance(mid, size());
      std::copy(first, mid, start_);
      finish_ = __uninitialized_copy(mid, last, finish_);
    }
  }

  iterator __insert_n_val(iterator position, size_type n,
                          const value_type &val) {
    vector<T> tmp_vec;
    size_type new_size = size() + n;
    tmp_vec.reserve(new_size);
    iterator tmp_vec_it = tmp_vec.begin();
    tmp_vec_it = std::copy(begin(), position, tmp_vec_it);
    tmp_vec.finish_ += std::distance(begin(), position);
    iterator first_inserted_element_it = tmp_vec_it;
    std::fill_n(tmp_vec_it, n, val);
    tmp_vec_it += n;
    tmp_vec.finish_ += n;
    std::copy(position, end(), tmp_vec_it);
    tmp_vec.finish_ += std::distance(position, end());
    swap(tmp_vec);
    return first_inserted_element_it;
  }

  template <class InputIterator>
  void __insert_range(iterator position, InputIterator first,
                      InputIterator last, std::input_iterator_tag) {
    if (position == end()) {
      for (; first != last; ++first) {
        insert(end(), *first);
      }
    } else if (first != last) {
      vector tmp_vec(first, last, get_allocator());
      insert(position, tmp_vec.begin(), tmp_vec.end());
    }
  }

  template <class InputIterator>
  void __insert_range(iterator position, InputIterator first,
                      InputIterator last, std::forward_iterator_tag) {
    vector<T> tmp_vec;
    size_type n = std::distance(first, last);
    size_type new_size = size() + n;
    tmp_vec.reserve(new_size);
    iterator tmp_vec_it = tmp_vec.begin();
    tmp_vec_it = std::copy(begin(), position, tmp_vec_it);
    tmp_vec.finish_ += std::distance(begin(), position);
    tmp_vec_it = std::copy(first, last, tmp_vec_it);
    tmp_vec.finish_ += n;
    std::copy(position, end(), tmp_vec_it);
    tmp_vec.finish_ += std::distance(position, end());
    swap(tmp_vec);
  }
};

template <class T, class Alloc>
bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return lhs.size() == rhs.size() &&
         ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return !(lhs > rhs);
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return rhs < lhs;
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

namespace std {
template <typename T, typename Alloc>
inline void swap(const ft::vector<T, Alloc> &lhs,
                 const ft::vector<T, Alloc> &rhs) {
  lhs.swap(rhs);
}
}  // namespace std

#endif
