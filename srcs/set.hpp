#ifndef SET_H_
#define SET_H_

#include <functional>
#include <memory>

#include "pair.hpp"
#include "red_black_tree.hpp"

namespace ft {

template <typename T>
struct Identity {
  typedef T argument_type;

  typedef T result_type;

  T& operator()(T& x) const {
    return x;
  }

  const T& operator()(const T& x) const {
    return x;
  }
};

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key> >
class set {
 public:
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef Allocator allocator_type;

 private:
  // This turns a red-black tree into a set.
  typedef typename Allocator::template rebind<value_type>::other key_alloc_type;
  typedef RedBlackTree<key_type, value_type, Identity<value_type>, key_compare,
                       key_alloc_type>
      RepType;

  // The actual tree structure.
  RepType rbtree_;

 public:
  typedef typename key_alloc_type::reference reference;
  typedef typename key_alloc_type::const_reference const_reference;
  typedef typename key_alloc_type::pointer pointer;
  typedef typename key_alloc_type::const_pointer const_pointer;
  typedef typename RepType::size_type size_type;
  typedef typename RepType::difference_type difference_type;
  typedef typename RepType::iterator iterator;
  typedef typename RepType::const_iterator const_iterator;
  typedef typename RepType::reverse_iterator reverse_iterator;
  typedef typename RepType::const_reverse_iterator const_reverse_iterator;

  /********** Constructor, Assignation and Destructor **********/
  set() : rbtree_() {}

  explicit set(const Compare& comp, const Allocator& alloc = Allocator())
      : rbtree_(comp, alloc) {}

  template <class InputIt>
  set(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator())
      : rbtree_(first, last, comp, alloc) {}

  set(const set& other) : rbtree_(other.rbtree_) {}

  set& operator=(const set& other) {
    if (this != &other) {
      rbtree_ = other.rbtree_;
    }
    return *this;
  }

  ~set() {}

  /********** Get allocator **********/
  allocator_type get_allocator() const {
    return allocator_type(rbtree_.get_allocator());
  }

  /********** Iterators **********/
  iterator begin() {
    return rbtree_.begin();
  }

  const_iterator begin() const {
    return rbtree_.begin();
  }

  iterator end() {
    return rbtree_.end();
  }

  const_iterator end() const {
    return rbtree_.end();
  }

  reverse_iterator rbegin() {
    return rbtree_.rbegin();
  }

  const_reverse_iterator rbegin() const {
    return rbtree_.rbegin();
  }

  reverse_iterator rend() {
    return rbtree_.rend();
  }

  const_reverse_iterator rend() const {
    return rbtree_.rend();
  }

  /********** Capacity **********/
  bool empty() const {
    return size() == 0;
  }

  size_type size() const {
    return rbtree_.size();
  }

  size_type max_size() const {
    return rbtree_.max_size();
  }

  /********** Modifiers **********/
  void clear() {
    rbtree_.clear();
  }

  ft::pair<iterator, bool> insert(const value_type& value) {
    return rbtree_.insert_unique(value);
  }

  iterator insert(iterator hint, const value_type& value) {
    return rbtree_.insert_unique(hint, value);
  }

  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    rbtree_.insert_range_unique(first, last);
  }

  void erase(iterator pos) {
    rbtree_.erase(pos);
  }

  void erase(iterator first, iterator last) {
    rbtree_.erase(first, last);
  }

  size_type erase(const Key& key) {
    return rbtree_.erase(key);
  }

  void swap(set& other) {
    rbtree_.swap(other.rbtree_);
  }

  /********** Lookup **********/
  size_type count(const Key& key) const {
    return rbtree_.find(key) == rbtree_.end() ? 0 : 1;
  }

  iterator find(const Key& key) {
    return rbtree_.find(key);
  }

  const_iterator find(const Key& key) const {
    return rbtree_.find(key);
  }

  ft::pair<iterator, iterator> equal_range(const Key& key) {
    return rbtree_.equal_range(key);
  }

  ft::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    return rbtree_.equal_range(key);
  }

  iterator lower_bound(const Key& key) {
    return rbtree_.lower_bound(key);
  }

  const_iterator lower_bound(const Key& key) const {
    return rbtree_.lower_bound(key);
  }

  iterator upper_bound(const Key& key) {
    return rbtree_.upper_bound(key);
  }

  const_iterator upper_bound(const Key& key) const {
    return rbtree_.upper_bound(key);
  }

  /********** Observers **********/
  key_compare key_comp() const {
    return rbtree_.key_comp();
  }

  value_compare value_comp() const {
    return value_compare(rbtree_.key_comp());
  }

  /********** Basic comparison operators **********/
  template <typename K1, typename C1, typename A>
  friend bool operator==(const set<K1, C1, A>&, const set<K1, C1, A>&);

  template <typename K1, typename C1, typename A>
  friend bool operator<(const set<K1, C1, A>&, const set<K1, C1, A>&);
};

template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return lhs.rbtree_ == rhs.rbtree_;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
  return lhs.rbtree_ < rhs.rbtree_;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

namespace std {  // specializes the std::swap algorithm
template <typename Key, typename Compare, typename Alloc>
inline void swap(const ft::set<Key, Compare, Alloc>& lhs,
                 const ft::set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}
}  // namespace std

#endif