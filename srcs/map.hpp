#ifndef MAP_H_
#define MAP_H_

#include <functional>

#include "pair.hpp"
#include "red_black_tree.hpp"

namespace ft {

template <typename Pair>
struct Select1st {
  // argument_type is the type of the argument
  typedef Pair argument_type;

  // result_type is the return type
  typedef typename Pair::first_type result_type;

  typename Pair::first_type& operator()(Pair& p) const {
    return p.first;
  }

  const typename Pair::first_type& operator()(const Pair& p) const {
    return p.first;
  }
};

template <class Key, class Val, class Compare = std::less<Key>,
          class Allocator = std::allocator<ft::pair<const Key, Val> > >
class map {
 public:
  typedef Key key_type;
  typedef Val mapped_type;
  typedef ft::pair<const Key, Val> value_type;
  typedef Compare key_compare;
  typedef Allocator allocator_type;

 private:
  // This turns a red-black tree into a map.
  typedef
      typename Allocator::template rebind<value_type>::other pair_alloc_type;
  typedef RedBlackTree<key_type, value_type, Select1st<value_type>, key_compare,
                       pair_alloc_type>
      RepType;

  // The actual tree structure.
  RepType rbtree_;

 public:
  typedef typename pair_alloc_type::reference reference;
  typedef typename pair_alloc_type::const_reference const_reference;
  typedef typename pair_alloc_type::pointer pointer;
  typedef typename pair_alloc_type::const_pointer const_pointer;
  typedef typename RepType::size_type size_type;
  typedef typename RepType::difference_type difference_type;
  typedef typename RepType::iterator iterator;
  typedef typename RepType::const_iterator const_iterator;
  typedef typename RepType::reverse_iterator reverse_iterator;
  typedef typename RepType::const_reverse_iterator const_reverse_iterator;

  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
    friend class map<Key, Val, Compare, Allocator>;

   public:
    bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(lhs.first, rhs.first);
    }

   protected:
    Compare comp;

    value_compare(Compare c) : comp(c) {}
  };

  /********** Constructor and Assignation **********/
  map() : rbtree_() {}

  explicit map(const Compare& comp, const Allocator& alloc = Allocator())
      : rbtree_(comp, alloc) {}

  template <class InputIt>
  map(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator())
      : rbtree_(first, last, comp, alloc) {}

  map(const map& other) : rbtree_(other.rbtree_) {}

  map& operator=(const map& other) {
    if (this != &other) {
      rbtree_ = other.rbtree_;
    }
    return *this;
  }

  /********** Destructor **********/
  ~map() {}

  // Get a copy of the memory allocation object.
  allocator_type get_allocator() const {
    return allocator_type(rbtree_.get_allocator());
  }

  /********** Element access **********/

  mapped_type& operator[](const key_type& key) {
    iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, (*it).first)) {
      it = insert(it, value_type(key, mapped_type()));
    }
    return (*it).second;
  }

  mapped_type& at(const key_type& key) {
    iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, (*it).first)) {
      throw std::out_of_range("map::at");
    }
    return (*it).second;
  }

  const mapped_type& at(const key_type& key) const {
    const_iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, (*it).first)) {
      throw std::out_of_range("map::at");
    }
    return (*it).second;
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
    return rbtree_.empty();
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

  iterator insert(iterator position, const value_type& value) {
    return rbtree_.insert_unique(position, value);
  }

  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    rbtree_.insert_range_unique(first, last);
  }

  size_type erase(const key_type& key) {
    return rbtree_.erase(key);
  }

  void erase(iterator first, iterator last) {
    rbtree_.erase(first, last);
  }

  void swap(map& other) {
    rbtree_.swap(other.rbtree_);
  }

  /********** Lookup **********/

  iterator find(const key_type& key) {
    return rbtree_.find(key);
  }

  const_iterator find(const key_type& key) const {
    return rbtree_.find(key);
  }

  size_type count(const key_type& key) const {
    return rbtree_.find(key) == rbtree_.end() ? 0 : 1;
  }

  ft::pair<iterator, iterator> equal_range(const key_type& key) {
    return rbtree_.equal_range(key);
  }

  ft::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return rbtree_.equal_range(key);
  }

  iterator lower_bound(const key_type& key) {
    return rbtree_.lower_bound(key);
  }

  const_iterator lower_bound(const key_type& key) const {
    return rbtree_.lower_bound(key);
  }

  iterator upper_bound(const key_type& key) {
    return rbtree_.upper_bound(key);
  }

  const_iterator upper_bound(const key_type& key) const {
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
  template <typename K1, typename T1, typename C1, typename A>
  friend bool operator==(const map<K1, T1, C1, A>&, const map<K1, T1, C1, A>&);

  template <typename K1, typename T1, typename C1, typename A>
  friend bool operator<(const map<K1, T1, C1, A>&, const map<K1, T1, C1, A>&);
};

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator==(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return lhs.rbtree_ == rhs.rbtree_;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<(const map<Key, Value, Compare, Alloc>& lhs,
                      const map<Key, Value, Compare, Alloc>& rhs) {
  return lhs.rbtree_ < rhs.rbtree_;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator!=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>(const map<Key, Value, Compare, Alloc>& lhs,
                      const map<Key, Value, Compare, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

// specializes the std::swap algorithm
template <typename Key, typename Value, typename Compare, typename Alloc>
inline void std::swap(const ft::map<Key, Value, Compare, Alloc>& lhs,
                      const ft::map<Key, Value, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

#endif