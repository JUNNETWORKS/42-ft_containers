#ifndef MAP_H_
#define MAP_H_

#include <functional>
#include <memory>

#include "pair.hpp"

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
  set();

  explicit set(const Compare& comp, const Allocator& alloc = Allocator());

  template <class InputIt>
  set(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator());

  set(const set& other);

  set& operator=(const set& other);

  ~set();

  /********** Get allocator **********/
  allocator_type get_allocator() const;

  /********** Iterators **********/
  iterator begin();

  const_iterator begin() const;

  iterator end();

  const_iterator end() const;

  reverse_iterator rbegin();

  const_reverse_iterator rbegin() const;

  reverse_iterator rend();

  const_reverse_iterator rend() const;

  /********** Capacity **********/
  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  /********** Modifiers **********/
  void clear();

  ft::pair<iterator, bool> insert(const value_type& value);

  iterator insert(iterator hint, const value_type& value);

  template <class InputIt>
  void insert(InputIt first, InputIt last);

  void erase(iterator pos);

  void erase(iterator first, iterator last);

  size_type erase(const Key& key);

  void swap(set& other);

  /********** Lookup **********/
  size_type count(const Key& key) const;

  iterator find(const Key& key);

  const_iterator find(const Key& key) const;

  ft::pair<iterator, iterator> equal_range(const Key& key);

  ft::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

  iterator lower_bound(const Key& key);

  const_iterator lower_bound(const Key& key) const;

  iterator upper_bound(const Key& key);

  const_iterator upper_bound(const Key& key) const;

  /********** Observers **********/
  key_compare key_comp() const;

  value_compare value_comp() const;

  /********** Basic operators **********/
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

// specializes the std::swap algorithm
template <typename Key, typename Compare, typename Alloc>
inline void std::swap(const ft::set<Key, Compare, Alloc>& lhs,
                      const ft::set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

#endif