#ifndef VECTOR_H_
#define VECTOR_H_

#include <memory>

namespace ft {

template <typename T>
class vector_iterator;

template <typename T, typename Allocator = std::allocator<T> >
class vector {
  template <typename T>
  friend class vector_iterator;

 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef vector_iterator iterator;
  typedef const vector_iterator const_iterator;
  typedef std::size_t size_type;

  vector(const allocator_type &alloc = allocator_type());
  vector(size_type n, const value_type &val = value_type(),
         const allocator_type &alloc = allocator_type());
  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &alloc = allocator_type());
  vector(const vector &x);
  ~vector();

  vector &operator=(const vector &x);

  // Iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  iterator rbegin();
  const_iterator rbegin() const;
  iterator rend();
  const_iterator rend() const;

  // Capacity
  size_type size() const;
  size_type max_size() const;
  size_type capacity() const;
  bool empty() const;
  void reserve(size_type n);

  // Element access
  reference &operator[]();
  const_reference &operator[]() const;
  reference &at();
  const_reference &at() const;
  reference &front();
  const_reference &front() const;
  reference &back();
  const_reference &back() const;

  // Modifiers
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const value_type &val);
  void push_back(const value_type &val);
  void pop_back;
  iterator insert(iterator position, const value_type &val);
  void insert(iterator position, size_type n, const value_type &val);
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void swap(vector &x);
  void clear();

  // Allocator
  allocator_type get_allocator() const;

 private:
  value_type *store_;
};
}  // namespace ft

#endif