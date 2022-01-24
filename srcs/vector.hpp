#ifndef VECTOR_H_
#define VECTOR_H_
#include <iostream>
#include <memory>
#include <stdexcept>

#include "lexicographical_compare.hpp"
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
  typedef ft::normal_iterator<pointer, vector> iterator;
  typedef ft::normal_iterator<const_pointer, vector> const_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
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
    int n = std::distance(first, last);
    cap_ = n;
    start_ = alloc.allocate(cap_);
    for (size_type i = 0; first != last; first++, i++) {
      alloc.construct(start_ + i, *first);
    }
    finish_ = start_ + n;
    end_of_storage_ = start_ + cap_;
  }

  vector(const vector &x) : cap_(), start_(), finish_(), end_of_storage_() {
    operator=(x);
  }

  const vector<T, Allocator> &operator=(const vector<T, Allocator> &x) {
    if (this != &x) {
      // 自分の配列を捨てる
      for (size_type i = 0; size(); i++) {
        allocator.destroy(start_ + i);
      }
      allocator.deallocate(start_, cap_);

      // 新たに配列を作成し, データをxからコピー
      cap_ = x.cap_;
      start_ = allocator.allocate(cap_);
      for (size_type i = 0; i < x.size(); i++) {
        allocator.construct(start_ + i, x[i]);
      }
      finish_ = start_ + x.size();
      end_of_storage_ = start_ + cap_;
    }
    return *this;
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
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    size_type n = std::distance(first, last);
    // *this の要素を全てn個のvalのコピーに置き換える
    if (n > capacity()) {
      // 新しく領域を確保し, そこにn個のvalのコピーを作成する
      // 古い領域は破棄
      vector<T> tmp(first, last);
      swap(tmp);
    } else if (n > size()) {
      for (size_type i = 0; first != last; ++first, ++i) {
        allocator.construct(start_ + i, *first);
      }
      finish_ = start_ + n;
    } else {
      // 現在の領域に上書きする形でvalのコピーをn個作成
      size_type i = 0;
      for (; first != last; ++first, ++i) {
        allocator.construct(start_ + i, *first);
      }
      // storage_[n] 以降の領域のデータは不要なので破棄する
      for (; i < size(); ++i) {
        allocator.destroy(start_ + i);
      }
      finish_ = start_ + n;
    }
  }

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
    return insert_n_val(position, 1, val);
  }

  void insert(iterator position, size_type n, const value_type &val) {
    insert_n_val(position, n, val);
  }

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {
    insert_range(position, first, last);
  }

  iterator erase(iterator position) {
    size_type pos_idx = std::distance(begin(), position);
    size_type end_idx = size();

    // positionとそれ以降の要素を1つずつ前にずらす
    size_type idx;
    for (idx = pos_idx; idx + 1 < end_idx; ++idx) {
      start_[idx] = start_[idx + 1];
    }

    size_type new_end_idx = end_idx - 1;
    // 移動後に残った末尾のデータのデストラクタを呼ぶ
    allocator.destroy(start_ + new_end_idx);
    finish_ = start_ + new_end_idx;

    return iterator(start_ + pos_idx);
  }

  iterator erase(iterator first, iterator last) {
    size_type range = std::distance(first, last);
    size_type first_idx = std::distance(begin(), first);
    size_type end_idx = size();

    size_type idx;
    for (idx = first_idx; idx + range < end_idx; ++idx) {
      start_[idx] = start_[idx + range];
    }
    size_type new_end_idx = idx;

    // 余ったやつはデストラクタ呼ぶ
    for (; idx < end_idx; ++idx) {
      allocator.destroy(start_ + idx);
    }
    finish_ = start_ + new_end_idx;
    return iterator(start_ + first_idx);
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

  iterator insert_n_val(iterator position, size_type n, const value_type &val) {
    size_type new_size = size() + n;
    if (new_size >= capacity()) {
      vector<T> new_vec;
      new_vec.reserve(new_size);
      iterator new_it = new_vec.begin();
      // positionの前までコピーする
      new_it = std::copy(begin(), position, new_it);
      // positionにvalをコピー
      std::fill_n(new_it, n, val);
      new_it += n;
      iterator val_it = new_it;
      // positionの後に元の配列のpositionの後ろの部分をコピー
      std::copy(position, end(), new_it);

      // 古い配列を破棄(new_vecのデストラクタで自動的に破棄される)
      swap(new_vec);
      return val_it;
    } else {
      // positionの直後まで後ろから1つ後ろにずらしてコピーする
      reverse_iterator position_rit = reverse_iterator(position);
      for (reverse_iterator rit = rbegin(); rit != position_rit; ++rit) {
        *rit = *(rit + 1);
      }
      // positionにvalをコピー
      *position = val;
      // positionのより前はそのまま
      return position;
    }
  }

  template <class InputIterator>
  iterator insert_range(iterator position, InputIterator first,
                        InputIterator last) {
    size_type n = std::distance(first, last);
    size_type new_size = size() + n;
    if (new_size >= capacity()) {
      vector<T> new_vec;
      new_vec.reserve(new_size);
      iterator new_it = new_vec.begin();
      // positionの前までコピーする
      new_it = std::copy(begin(), position, new_it);
      iterator val_it = new_it;
      // new_itの後ろにfirst~last-1をコピー
      new_it = std::copy(first, last, new_it);
      // positionの後に元の配列のpositionの後ろの部分をコピー
      new_it = std::copy(position, end(), new_it);
      new_vec.finish_ = new_vec.start_ + new_size;
      // 古い配列を破棄(new_vecのデストラクタで自動的に破棄される)
      swap(new_vec);
      return val_it;
    } else {
      // position+nまで後ろからnつ後ろにずらしてコピーする
      reverse_iterator position_rit = reverse_iterator(position);
      for (reverse_iterator rit = rbegin() - n; rit != position_rit - n;
           ++rit) {
        *rit = *(rit + n);
      }
      // positionにfirst~last-1をコピー
      std::copy(first, last, position);
      finish_ = start_ + new_size;
      // positionのより前はそのまま
      return position;
    }
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

template <class T, class Alloc>
bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
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

#endif
