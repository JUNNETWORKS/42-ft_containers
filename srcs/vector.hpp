#ifndef VECTOR_H_
#define VECTOR_H_
#include <memory>

#include "normal_iterator.hpp"

namespace ft {

template <typename>
class vector_iterator;

template <typename T, typename Allocator = std::allocator<T> >
class vector {
  friend class vector_iterator<T>;

 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef normal_iterator<pointer, vector> iterator;
  typedef normal_iterator<const_pointer, vector> const_iterator;
  typedef std::size_t size_type;

  vector(allocator_type alloc = allocator_type()) : cap_(kDefaultCap_) {
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; i < cap_; i++) {
      alloc.construct(start_ + i);
    }
    finish_ = start_;
    end_of_storage_ = start_ + cap_;
  }

  vector(size_type n, const value_type &val = value_type(),
         allocator_type alloc = allocator_type())
      : cap_(n) {
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; i < cap_; i++) {
      alloc.construct(start_ + i, val);
    }
    finish_ = start_ + n;
    end_of_storage_ = start_ + cap_;
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         allocator_type alloc = allocator_type(),
         typename disable_if<is_integral<InputIterator>::value>::type * = 0) {
    // TODO: Random Access Iterator じゃないと減算がサポートされてない
    int n = last - first;
    cap_ = n;
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; first != last; first++, i++) {
      alloc.construct(start_ + i, *first);
    }
    finish_ = start_ + n;
    end_of_storage_ = start_ + cap_;
  }

  vector(const vector &x) : cap_(cap_) {
    allocator_type alloc = allocator_type();
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; i < x.size(); i++) {
      alloc.construct(start_ + i, x[i]);
    }
    finish_ = start_ + x.size();
    end_of_storage_ = start_ + cap_;
  }

  vector &operator=(const vector &x) {
    allocator_type alloc = allocator_type();

    cap_ = x.cap_;
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; i < x.size(); i++) {
      alloc.construct(start_ + i, x[i]);
    }
    finish_ = start_ + x.size();
    end_of_storage_ = start_ + cap_;
  }

  ~vector() {
    allocator_type alloc = allocator_type();
    alloc.deallocate(start_, cap_);
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

  // reverse_iterator rbegin() {}

  // const_reverse_iterator rbegin() const;

  // reverse_iterator rend() {}

  // const_reverse_iterator rend() const;

  // Capacity

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

  // void reserve(size_type n);

  // Element access
  reference &operator[](int n) {
    return start_[n];
  }
  // const_reference &operator[](int n) const;
  reference &at(int n) {
    return start_[n];
  }
  // const_reference &at(int n) const;
  reference &front();
  // const_reference &front() const;
  reference &back();
  // const_reference &back() const;

  // Modifiers
  // template <class InputIterator>
  // void assign(InputIterator first, InputIterator last);
  // void assign(size_type n, const value_type &val);
  // void push_back(const value_type &val);
  // void pop_back();
  // iterator insert(iterator position, const value_type &val);
  // void insert(iterator position, size_type n, const value_type &val);
  // template <class InputIterator>
  // void insert(iterator position, InputIterator first, InputIterator last);
  // iterator erase(iterator position);
  // iterator erase(iterator first, iterator last);
  // void swap(vector &x);
  // void clear();

  // Allocator
  allocator_type get_allocator() const {
    return allocator_type();
  }

 private:
  static const size_type kDefaultCap_ = 1024;

  size_type cap_;
  pointer start_;
  pointer finish_;
  pointer end_of_storage_;
};
}  // namespace ft

#endif
