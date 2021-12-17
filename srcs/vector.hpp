#ifndef VECTOR_H_
#define VECTOR_H_
#include <iostream>
#include <memory>
#include <stdexcept>

#include "normal_iterator.hpp"
#include "reverse_iterator.hpp"

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
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;
  typedef std::size_t size_type;

  vector(size_type n = 0, const value_type &val = value_type(),
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

  vector(const vector &x) : cap_(x.cap_) {
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

  void reserve(size_type n) {
    if (n > allocator.max_size())
      throw std::length_error("vector::reserve");
    if (capacity() < n) {
      expand_and_copy_storage(n);
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
      throw std::out_of_range("vector::at out of range!");
    return start_[n];
  }
  const_reference at(size_type n) const {
    if (n >= size())
      throw std::out_of_range("vector::at out of range!");
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

  // Modifiers
  // template <class InputIterator>
  // void assign(InputIterator first, InputIterator last) {

  // }
  void assign(size_type n, const value_type &val) {
    // *this の要素を全てn個のvalのコピーに置き換える
    if (n > capacity()) {
      // 新しく領域を確保し, そこにn個のvalのコピーを作成する
      // 古い領域は破棄
      vector<T> tmp(n, val);
      swap(tmp);
    } else if (n > size()) {
      // 現在の領域に上書きする形でvalのコピーを現在のsize()個分作成
      std::fill(begin(), end(), val);
      // size()を超えた分は明示的にコンストラクタを呼ぶ必要がある
      size_type remain = n - size();
      for (size_type i = 0; i < remain; ++i) {
        allocator.construct(finish_ + i, val);
      }
      finish_ = start_ + n;
    } else {
      // 現在の領域に上書きする形でvalのコピーをn個作成
      std::fill_n(start_, n, val);
      // storage_[n] 以降の領域のデータは不要なので破棄する
      size_type remain = size() - n;
      for (size_type i = 0; i < remain; ++i) {
        allocator.destroy(start_ + n + i);
      }
      finish_ = start_ + n;
    }
  }
  void push_back(const value_type &val) {
    if (size() == capacity()) {
      expand_and_copy_storage(calc_new_capacity(capacity()));
    }
    allocator.construct(finish_, val);
    ++finish_;
  }
  void pop_back() {
    if (size() == 0) {
      return;
    }
    --finish_;
    allocator.destroy(finish_);
  }
  // iterator insert(iterator position, const value_type &val);
  // void insert(iterator position, size_type n, const value_type &val);
  // template <class InputIterator>
  // void insert(iterator position, InputIterator first, InputIterator last);
  // iterator erase(iterator position);
  // iterator erase(iterator first, iterator last);

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
    return allocator_type();
  }

 private:
  void expand_and_copy_storage(size_type new_cap) {
    // std::cout << "expand_and_copy_storage is called!!" << std::endl;

    // 新しい領域の確保と先頭を記録.
    pointer new_start = allocator.allocate(new_cap);

    size_type len = finish_ - start_;

    // 新しい領域にデータをコピーする
    for (size_type i = 0; i < len; i++) {
      allocator.construct(new_start + i, *(start_ + i));

      /* 古い領域のデータは不要なのでデストラクタを呼び出す */
      allocator.destroy(start_ + i);
    }
    /* 古い領域を破棄 */
    allocator.deallocate(start_, cap_);

    /* 各種メンバー変数を更新 */
    cap_ = new_cap;
    start_ = new_start;
    finish_ = start_ + len;
    end_of_storage_ = start_ + cap_;
  }

  inline size_type calc_new_capacity(size_type current_capacity) {
    if (current_capacity == 0) {
      return 1;
    }
    return current_capacity * 2;
  }

  static allocator_type allocator;
  size_type cap_;
  pointer start_;
  pointer finish_;
  pointer end_of_storage_;
};

template <typename T, typename Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::allocator =
    vector<T, Allocator>::allocator_type();

}  // namespace ft

#endif
