#ifndef MAP_H_
#define MAP_H_

#include <functional>

#include "pair.hpp"
#include "red_black_tree.hpp"

namespace ft {

template <typename Pair>
struct Select1st {
  /// @c argument_type is the type of the argument
  typedef Pair argument_type;

  /// @c result_type is the return type
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
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef Allocator::pointer pointer;
  typedef Allocator::const_pointer const_pointer;

 private:
  // This turns a red-black tree into a [multi]map.
  typedef typename Alloc::template rebind<value_type>::other pair_alloc_type;
  typedef RedBlackTree<key_type, value_type, Select1st<value_type>, key_compare,
                       pair_alloc_type>
      RepType;

 public:
  typedef typename RepType::iterator iterator;
  typedef typename RepType::const_iterator const_iterator;
  typedef typename RepType::size_type size_type;
  typedef typename RepType::difference_type difference_type;
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
  }

  /* コンストラクタ */
  map()
      : rbtree_() {
  }

  map();
  explicit map(const Compare& comp, const Allocator& alloc = Allocator());

  template <class InputIt>
  map(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator());

  map(const map& other) : rbtree_(other.rbtree_);

  /* 代入演算子 */
  map& operator=(const map& other);

  /* デストラクタ */
  ~map();

  // Get a copy of the memory allocation object.
  allocator_type get_allocator() const {
    return allocator_type(rbtree_.get_allocator());
  }

  // iterators
  /**
   *  Returns a read/write iterator that points to the first pair in the
   *  %map.
   *  Iteration is done in ascending order according to the keys.
   */
  iterator begin() {
    return rbtree_.begin();
  }

  /**
   *  Returns a read-only (constant) iterator that points to the first pair
   *  in the %map.  Iteration is done in ascending order according to the
   *  keys.
   */
  const_iterator begin() const {
    return rbtree_.begin();
  }

  /**
   *  Returns a read/write iterator that points one past the last
   *  pair in the %map.  Iteration is done in ascending order
   *  according to the keys.
   */
  iterator end() {
    return rbtree_.end();
  }

  /**
   *  Returns a read-only (constant) iterator that points one past the last
   *  pair in the %map.  Iteration is done in ascending order according to
   *  the keys.
   */
  const_iterator end() const {
    return rbtree_.end();
  }

  /**
   *  Returns a read/write reverse iterator that points to the last pair in
   *  the %map.  Iteration is done in descending order according to the
   *  keys.
   */
  reverse_iterator rbegin() {
    return rbtree_.rbegin();
  }

  /**
   *  Returns a read-only (constant) reverse iterator that points to the
   *  last pair in the %map.  Iteration is done in descending order
   *  according to the keys.
   */
  const_reverse_iterator rbegin() const {
    return rbtree_.rbegin();
  }

  /**
   *  Returns a read/write reverse iterator that points to one before the
   *  first pair in the %map.  Iteration is done in descending order
   *  according to the keys.
   */
  reverse_iterator rend() {
    return rbtree_.rend();
  }

  /**
   *  Returns a read-only (constant) reverse iterator that points to one
   *  before the first pair in the %map.  Iteration is done in descending
   *  order according to the keys.
   */
  const_reverse_iterator rend() const {
    return rbtree_.rend();
  }

  // capacity
  /** Returns true if the %map is empty.  (Thus begin() would equal
   *  end().)
   */
  bool empty() const {
    return rbtree_.empty();
  }

  /** Returns the size of the %map.  */
  size_type size() const {
    return rbtree_.size();
  }

  /** Returns the maximum size of the %map.  */
  size_type max_size() const {
    return rbtree_.max_size();
  }

  // [23.3.1.2] element access
  /**
   *  @brief  Subscript ( @c [] ) access to %map data.
   *  @param  __k  The key for which data should be retrieved.
   *  @return  A reference to the data of the (key,data) %pair.
   *
   *  Allows for easy lookup with the subscript ( @c [] )
   *  operator.  Returns data associated with the key specified in
   *  subscript.  If the key does not exist, a pair with that key
   *  is created using default values, which is then returned.
   *
   *  Lookup requires logarithmic time.
   */
  mapped_type& operator[](const key_type& key) {
    iterator i = lower_bound(key);
    // __i->first is greater than or equivalent to __k.
    if (i == end() || key_comp()(key, (*i).first)) {
      i = insert(i, value_type(key, mapped_type()));
    }
    return (*i).second;
  }

  // DR 464. Suggestion for new member functions in standard containers.
  /**
   *  @brief  Access to %map data.
   *  @param  key  The key for which data should be retrieved.
   *  @return  A reference to the data whose key is equivalent to @a key, if
   *           such a data is present in the %map.
   *  @throw  std::out_of_range  If no such data is present.
   */
  mapped_type& at(const key_type& key) {
    iterator i = lower_bound(key);
    if (i == end() || key_comp()(key, (*i).first))
      __throw_out_of_range(__N("map::at"));
    return (*i).second;
  }

  const mapped_type& at(const key_type& key) const {
    const_iterator i = lower_bound(key);
    if (i == end() || key_comp()(key, (*i).first))
      __throw_out_of_range(__N("map::at"));
    return (*i).second;
  }

  /* Modifiers */

  /**
   *  @brief Attempts to insert a std::pair into the %map.
   *  @param __x Pair to be inserted (see std::make_pair for easy
   *	     creation of pairs).
   *
   *  @return  A pair, of which the first element is an iterator that
   *           points to the possibly inserted pair, and the second is
   *           a bool that is true if the pair was actually inserted.
   *
   *  This function attempts to insert a (key, value) %pair into the %map.
   *  A %map relies on unique keys and thus a %pair is only inserted if its
   *  first element (the key) is not already present in the %map.
   *
   *  Insertion requires logarithmic time.
   *  @{
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    return rbtree_.insert_unique(value);
  }

  /**
   *  @brief Attempts to insert a std::pair into the %map.
   *  @param  __position  An iterator that serves as a hint as to where the
   *                    pair should be inserted.
   *  @param  __x  Pair to be inserted (see std::make_pair for easy creation
   *               of pairs).
   *  @return An iterator that points to the element with key of
   *           @a __x (may or may not be the %pair passed in).
   *
   *  This function is not concerned about whether the insertion
   *  took place, and thus does not return a boolean like the
   *  single-argument insert() does.  Note that the first
   *  parameter is only a hint and can potentially improve the
   *  performance of the insertion process.  A bad hint would
   *  cause no gains in efficiency.
   *
   *  See
   * https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
   *  for more on @a hinting.
   *
   *  Insertion requires logarithmic time (if the hint is not taken).
   *  @{
   */
  iterator insert(iterator position, const value_type& value) {
    return rbtree_.insert_unique_(position, value);
  }

  /**
   *  @brief Template function that attempts to insert a range of elements.
   *  @param  __first  Iterator pointing to the start of the range to be
   *                   inserted.
   *  @param  __last  Iterator pointing to the end of the range.
   *
   *  Complexity similar to that of the range constructor.
   */
  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    rbtree_.insert_range_unique(first, last);
  }

  /**
   *  @brief Erases elements according to the provided key.
   *  @param  key  Key of element to be erased.
   *  @return  The number of elements erased.
   *
   *  This function erases all the elements located by the given key from
   *  a %map.
   *  Note that this function only erases the element, and that if
   *  the element is itself a pointer, the pointed-to memory is not touched
   *  in any way.  Managing the pointer is the user's responsibility.
   */
  size_type erase(const key_type& key) {
    return rbtree_.erase(key);
  }

  /**
   *  @brief Erases a [first,last) range of elements from a %map.
   *  @param  first  Iterator pointing to the start of the range to be
   *                   erased.
   *  @param last Iterator pointing to the end of the range to
   *                be erased.
   *
   *  This function erases a sequence of elements from a %map.
   *  Note that this function only erases the element, and that if
   *  the element is itself a pointer, the pointed-to memory is not touched
   *  in any way.  Managing the pointer is the user's responsibility.
   */
  void erase(iterator first, iterator last) {
    rbtree_.erase(first, last);
  }

  /**
   *  @brief  Swaps data with another %map.
   *  @param  __x  A %map of the same element and allocator types.
   *
   *  This exchanges the elements between two maps in constant
   *  time.  (It is only swapping a pointer, an integer, and an
   *  instance of the @c Compare type (which itself is often
   *  stateless and empty), so it should be quite fast.)  Note
   *  that the global std::swap() function is specialized such
   *  that std::swap(m1,m2) will feed to this function.
   *
   *  Whether the allocators are swapped depends on the allocator traits.
   */
  void swap(map& other){
    rbtree_.swap(other.rbtree_);
  }

  /**
   *  Erases all elements in a %map.  Note that this function only
   *  erases the elements, and that if the elements themselves are
   *  pointers, the pointed-to memory is not touched in any way.
   *  Managing the pointer is the user's responsibility.
   */
  void clear() {
    rbtree_.clear();
  }

  /* Observers */
  /**
   *  Returns the key comparison object out of which the %map was
   *  constructed.
   */
  key_compare key_comp() const {
    return rbtree_.key_comp();
  }

  /**
   *  Returns a value comparison object, built from the key comparison
   *  object out of which the %map was constructed.
   */
  value_compare value_comp() const {
    return value_compare(rbtree_.key_comp());
  }

  // [23.3.1.3] map operations

  //@{
  /**
   *  @brief Tries to locate an element in a %map.
   *  @param  key  Key of (key, value) %pair to be located.
   *  @return  Iterator pointing to sought-after element, or end() if not
   *           found.
   *
   *  This function takes a key and tries to locate the element with which
   *  the key matches.  If successful the function returns an iterator
   *  pointing to the sought after %pair.  If unsuccessful it returns the
   *  past-the-end ( @c end() ) iterator.
   */

  iterator find(const key_type& key) {
    return rbtree_.find(key);
  }

  //@{
  /**
   *  @brief Tries to locate an element in a %map.
   *  @param  key  Key of (key, value) %pair to be located.
   *  @return  Read-only (constant) iterator pointing to sought-after
   *           element, or end() if not found.
   *
   *  This function takes a key and tries to locate the element with which
   *  the key matches.  If successful the function returns a constant
   *  iterator pointing to the sought after %pair. If unsuccessful it
   *  returns the past-the-end ( @c end() ) iterator.
   */

  const_iterator find(const key_type& key) const {
    return rbtree_.find(key);
  }

  //@{
  /**
   *  @brief  Finds the number of elements with given key.
   *  @param  key  Key of (key, value) pairs to be located.
   *  @return  Number of elements with specified key.
   *
   *  This function only makes sense for multimaps; for map the result will
   *  either be 0 (not present) or 1 (present).
   */
  size_type count(const key_type& key) const {
    return rbtree_.find(key) == rbtree_.end() ? 0 : 1;
  }

  /**
   *  @brief Finds the beginning of a subsequence matching given key.
   *  @param  key  Key of (key, value) pair to be located.
   *  @return  Iterator pointing to first element equal to or greater
   *           than key, or end().
   *
   *  This function returns the first element of a subsequence of elements
   *  that matches the given key.  If unsuccessful it returns an iterator
   *  pointing to the first element that has a greater value than given key
   *  or end() if no such element exists.
   */
  iterator  (const key_type& key) {
    return rbtree_.lower_bound(key);
  }

  /**
   *  @brief Finds the beginning of a subsequence matching given key.
   *  @param  key  Key of (key, value) pair to be located.
   *  @return  Read-only (constant) iterator pointing to first element
   *           equal to or greater than key, or end().
   *
   *  This function returns the first element of a subsequence of elements
   *  that matches the given key.  If unsuccessful it returns an iterator
   *  pointing to the first element that has a greater value than given key
   *  or end() if no such element exists.
   */
  const_iterator lower_bound(const key_type& key) const {
    return rbtree_.lower_bound(key);
  }

  /**
   *  @brief Finds the end of a subsequence matching given key.
   *  @param  key  Key of (key, value) pair to be located.
   *  @return Iterator pointing to the first element
   *          greater than key, or end().
   */
  iterator upper_bound(const key_type& key) {
    return rbtree_.upper_bound(key);
  }

  /**
   *  @brief Finds the end of a subsequence matching given key.
   *  @param  key  Key of (key, value) pair to be located.
   *  @return  Read-only (constant) iterator pointing to first iterator
   *           greater than key, or end().
   */
  const_iterator upper_bound(const key_type& key) const {
    return rbtree_.upper_bound(key);
  }

  /**
   *  @brief Finds a subsequence matching given key.
   *  @param  key  Key of (key, value) pairs to be located.
   *  @return  Pair of iterators that possibly points to the subsequence
   *           matching given key.
   *
   *  This function is equivalent to
   *  @code
   *    std::make_pair(c.lower_bound(val),
   *                   c.upper_bound(val))
   *  @endcode
   *  (but is faster than making the calls separately).
   *
   *  This function probably only makes sense for multimaps.
   */
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return rbtree_.equal_range(key);
  }

  //@{
  /**
   *  @brief Finds a subsequence matching given key.
   *  @param  key  Key of (key, value) pairs to be located.
   *  @return  Pair of read-only (constant) iterators that possibly points
   *           to the subsequence matching given key.
   *
   *  This function is equivalent to
   *  @code
   *    std::make_pair(c.lower_bound(val),
   *                   c.upper_bound(val))
   *  @endcode
   *  (but is faster than making the calls separately).
   *
   *  This function probably only makes sense for multimaps.
   */
  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return rbtree_.equal_range(key);
  }

  template <typename K1, typename T1, typename C1, typename A>
  friend bool operator==(const map<K1, T1, C1, A>&,
                         const map<K1, T1, C1, A>&);

  template <typename K1, typename T1, typename C1, typename A>
  friend bool operator<(const map<K1, T1, C1, A>&,
                        const map<K1, T1, C1, A>&);

 private:
  // The actual tree structure.
  RepType rbtree_;
};

/**
 *  @brief  Map equality comparison.
 *  @param  lhs  A %map.
 *  @param  rhs  A %map of the same type as @a x.
 *  @return  True iff the size and elements of the maps are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of the
 *  maps.  Maps are considered equivalent if their sizes are equal,
 *  and if corresponding elements compare equal.
 */
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator==(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return lhs.rbtree_ == rhs.rbtree_;
}

/**
 *  @brief  Map ordering relation.
 *  @param  lhs  A %map.
 *  @param  rhs  A %map of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  maps.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
 */
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<(const map<Key, Value, Compare, Alloc>& lhs,
                      const map<Key, Value, Compare, Alloc>& rhs) {
  return lhs.rbtree_ < rhs.rbtree_;
}

/// Based on operator==
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator!=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

/// Based on operator<
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>(const map<Key, Value, Compare, Alloc>& lhs,
                      const map<Key, Value, Compare, Alloc>& rhs) {
  return rhs < lhs;
}

/// Based on operator<
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

/// Based on operator<
template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>=(const map<Key, Value, Compare, Alloc>& lhs,
                       const map<Key, Value, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

#endif