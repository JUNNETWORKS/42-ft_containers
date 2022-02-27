#ifndef TEST_UTILS_ALLOCATOR_H_
#define TEST_UTILS_ALLOCATOR_H_

#include <limits>
#include <memory>
namespace ft {
namespace test {

template <class T>
struct MyAllocator {
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  template <class U>
  struct rebind {
    typedef MyAllocator<U> other;
  };

  MyAllocator() {}

  MyAllocator(const MyAllocator& other) {
    (void)other;
  }

  template <class U>
  MyAllocator(const MyAllocator<U>& other) {
    (void)other;
  }

  ~MyAllocator() {}

  pointer address(reference x) const {
    // operator& がオーバーロードされていた場合は正しく動作しない
    return &x;
  }

  const_pointer address(const_reference x) const {
    // operator& がオーバーロードされていた場合は正しく動作しない
    return &x;
  }

  pointer allocate(size_type n, const void* hint = 0) {
    (void)hint;
    return (new value_type[n]);
  }

  void deallocate(T* p, size_type n) {
    (void)n;
    delete[] p;
  }

  size_type max_size() const {
    return 10000;
  }

  void construct(pointer p, const_reference val) {
    new ((void*)p) T(val);
  }

  void destroy(pointer p) {
    ((T*)p)->~T();
  }

  template <class U>
  void destroy(U* p) {
    p->~U();
  }
};

template <class T1, class T2>
bool operator==(const MyAllocator<T1>& lhs, const MyAllocator<T2>& rhs) {
  (void)lhs;
  (void)rhs;
  return true;
}

template <class T1, class T2>
bool operator!=(const MyAllocator<T1>& lhs, const MyAllocator<T2>& rhs) {
  return !(lhs == rhs);
}

}  // namespace test
}  // namespace ft
#endif