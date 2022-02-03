#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

#include <cstddef>
#include <iostream>

#include "iterator_traits.hpp"
#include "pair.hpp"
#include "reverse_iterator.hpp"

namespace ft {

template <class Value>
struct RBTNode;

template <class Value>
RBTNode<Value> *find_minimum_node(const RBTNode<Value> *root);

template <class Value>
RBTNode<Value> *find_maximum_node(const RBTNode<Value> *root);

template <class Value>
RBTNode<Value> *get_next_node(const RBTNode<Value> *current);

template <class Value>
RBTNode<Value> *get_prev_node(const RBTNode<Value> *current);

template <class Value>
struct rbtree_iterator {
  typedef Value value_type;
  typedef Value &reference;
  typedef Value *pointer;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef rbtree_iterator<Value> self_type;
  typedef RBTNode<Value> node_type;
  typedef node_type *node_pointer;

  node_pointer node_;

  rbtree_iterator() : node_() {}

  explicit rbtree_iterator(node_pointer ptr) : node_(ptr) {}

  node_type *get_node_ptr() {
    return node_;
  }

  self_type &operator=(const self_type &other) {
    if (this != &other) {
      node_ = other.node_;
    }
    return *this;
  }

  reference operator*() const {
    return node_->value_;
  }

  pointer operator->() const {
    return &node_->value_;
  }

  self_type &operator++() {
    node_ = get_next_node<Value>(node_);
    return *this;
  }

  self_type operator++(int) {
    self_type tmp = *this;
    node_ = get_next_node<Value>(node_);
    return tmp;
  }

  self_type &operator--() {
    node_ = get_prev_node<Value>(node_);
    return *this;
  }

  self_type operator--(int) {
    self_type tmp = *this;
    node_ = get_prev_node<Value>(node_);
    return tmp;
  }

  bool operator==(const self_type &rhs) const {
    return node_ == rhs.node_;
  }

  bool operator!=(const self_type &rhs) const {
    return node_ != rhs.node_;
  }
};

template <class Value>
struct rbtree_const_iterator {
  typedef Value value_type;
  typedef const Value &reference;
  typedef const Value *pointer;

  typedef rbtree_iterator<Value> iterator;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef rbtree_const_iterator<Value> self_type;
  typedef RBTNode<Value> node_type;
  typedef const node_type *node_pointer;

  rbtree_const_iterator() : node_() {}

  explicit rbtree_const_iterator(node_pointer ptr) : node_(ptr) {}

  self_type &operator=(const self_type &other) {
    if (this != &other) {
      node_ = other.node_;
    }
    return *this;
  }

  iterator cast_nonconst() const {
    return iterator(const_cast<typename iterator::node_pointer>(node_));
  }

  reference operator*() const {
    return node_->value_;
  }

  pointer operator->() const {
    return &node_->value_;
  }

  self_type &operator++() {
    node_ = get_next_node<Value>(node_);
    return *this;
  }

  self_type operator++(int) {
    self_type tmp = *this;
    node_ = get_next_node<Value>(node_);
    return tmp;
  }

  self_type &operator--() {
    node_ = get_prev_node<Value>(node_);
    return *this;
  }

  self_type operator--(int) {
    self_type tmp = *this;
    node_ = get_prev_node<Value>(node_);
    return tmp;
  }

  bool operator==(const self_type &rhs) const {
    return node_ == rhs.node_;
  }

  bool operator!=(const self_type &rhs) const {
    return node_ != rhs.node_;
  }

  node_pointer node_;
};

template <class Value>
struct RBTNode {
  enum Color { BLACK = 0, RED = 1 };

  Value value_;
  RBTNode *parent_;
  RBTNode *left_;
  RBTNode *right_;
  Color color_;
  const bool is_nil_node_;

  RBTNode(Value value = Value(), bool is_nil_node = false)
      : value_(value),
        parent_(),
        left_(),
        right_(),
        color_(BLACK),
        is_nil_node_(is_nil_node) {}

  RBTNode(const RBTNode &other)
      : value_(other.value_),
        parent_(other.parent_),
        left_(other.left_),
        right_(other.right_),
        color_(other.color_),
        is_nil_node_(other.is_nil_node_) {}

  ~RBTNode() {}

 private:
  // Assignment operator is forbidden.
  // Because node may have const type in value_ as key.
  RBTNode &operator=(const RBTNode &rhs) {}
};

template <class Value>
RBTNode<Value> *find_minimum_node(const RBTNode<Value> *root) {
  if (!root) {
    return NULL;
  }

  const RBTNode<Value> *current = root;
  while (!current->left_->is_nil_node_) {
    current = current->left_;
  }
  return const_cast<RBTNode<Value> *>(current);
}

template <class Value>
RBTNode<Value> *find_maximum_node(const RBTNode<Value> *root) {
  if (!root) {
    return NULL;
  }

  const RBTNode<Value> *current = root;
  while (!current->right_->is_nil_node_) {
    current = current->right_;
  }
  return const_cast<RBTNode<Value> *>(current);
}

// 中間順木巡回の順序での次の節点のポインタを返す
template <class Value>
RBTNode<Value> *get_next_node(const RBTNode<Value> *current) {
  if (!current->right_->is_nil_node_) {
    // currentが右の子を持っている時は右の子の中の最小が次のノード
    return find_minimum_node(current->right_);
  } else {
    if (current == current->parent_->left_) {
      // currentが親の左の子で, なおかつ右に子を持っていない場合,
      // 次のノードは親である
      return current->parent_;
    } else {
      // currentが親の右の子で, なおかつ右に子を持たない
      // currentが左の子になるまで親を遡る.
      const RBTNode<Value> *next_node = current->parent_;
      while (!next_node->is_nil_node_ && current == next_node->right_) {
        current = next_node;
        next_node = next_node->parent_;
      }
      return const_cast<RBTNode<Value> *>(next_node);
    }
  }
}

// 中間順木巡回の逆順序での前の節点のポインタを返す
template <class Value>
RBTNode<Value> *get_prev_node(const RBTNode<Value> *current) {
  if (!current->left_->is_nil_node_) {
    // currentが左の子を持つ場合は左部分木の中の最大値
    // currentの次に小さい値
    return find_maximum_node(current->left_);
  } else {
    if (current == current->parent_->left_) {
      // currentが親の左の子で, なおかつ左に子を持たない
      // currentが右の子になるまで親を遡る.
      const RBTNode<Value> *next_node = current->parent_;
      while (!next_node->is_nil_node_ && current == next_node->left_) {
        current = next_node;
        next_node = next_node->parent_;
      }
      return const_cast<RBTNode<Value> *>(next_node);
    } else {
      // currentが親の右のノードで, currentが左の子を持たない場合,
      // 次のノードはcurrentの親
      return const_cast<RBTNode<Value> *>(current->parent_);
    }
  }
}

// Red Black Tree
//
// ノードを辿るルールは left < key <= right である.
//
// Red Black Tree の2色条件
// 1. 各ノードは赤か黒の色である
// 2. 根は黒である. (たまにこの条件は省かれる)
// 3. 葉(NIL)は全て黒である. 葉は全て根と同じ色である.
// 4. 赤のノードは黒ノードを2つ子に持つ.
// 5. 任意のノードについて,
//    そのノードから子孫の葉までの道に含まれる黒ノードの数は,
//    選んだ葉に依らず一定である.
//    (この条件は,
//    「根から葉までの道に含まれる黒いノードの数は、葉によらず一定である」
//    と言い換えることができる)
//
// テンプレートパラメータの説明
// Key: キー
// Val: ノードに格納するデータ? mapだとpair, setだと_Key.
// KeyOfValue: _Val型のデータからキーを取り出す
template <class Key, class Value, class KeyOfValue,
          class Compare = std::less<Key>, class Alloc = std::allocator<Value> >
class RedBlackTree {
 public:
  typedef RBTNode<Value> node_type;
  typedef typename Alloc::template rebind<node_type>::other node_allocator;

  typedef Key key_type;
  typedef Value value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Alloc allocator_type;

  typedef rbtree_iterator<value_type> iterator;
  typedef rbtree_const_iterator<value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

#ifdef DEBUG
 public:
#else
 private:
#endif
  // Members
  node_type *nil_node_;  // point to nil_node_object
  node_type *root_;
  size_type node_count_;
  // begin() と end() を O(1) でアクセスするためにメンバー変数にもたせておく
  // beginは常に最小のノードを示す。
  // beginは常に最大のノードを示す。
  // endは左の子としてroot_を持つ。
  node_type *begin_node_;
  node_type *end_node_;

 public:
  // Constructor, Descructor

  RedBlackTree()
      : nil_node_(__alloc_nil_node()),
        root_(nil_node_),
        node_count_(0),
        begin_node_(nil_node_),
        end_node_(nil_node_) {
    nil_node_->parent_ = nil_node_;
    nil_node_->left_ = nil_node_;
    nil_node_->right_ = nil_node_;
    nil_node_->color_ = node_type::BLACK;

    begin_node_ = root_;
    end_node_->left_ = root_;
    end_node_->right_ = root_;
  }

  RedBlackTree(const RedBlackTree &other)
      : nil_node_(__alloc_nil_node()),
        root_(nil_node_),
        node_count_(0),
        begin_node_(nil_node_),
        end_node_(nil_node_) {
    operator=(other);
  }

  RedBlackTree &operator=(const RedBlackTree &rhs) {
    if (&rhs != this) {
      __delete_tree(root_);
      // ノードをディープコピー
      root_ = __copy_tree(rhs.root_, rhs.nil_node_);

      begin_node_ = find_minimum_node(root_);
      if (!root_->is_nil_node_) {
        root_->parent_ = end_node_;
      }
      end_node_->left_ = root_;
      end_node_->right_ = root_;
      node_count_ = rhs.node_count_;
    }
    return *this;
  }

  ~RedBlackTree() {
    // 全てのノードをdeleteする
    __delete_tree(root_);
    __delete_node(nil_node_);
  }

  /********** Insert **********/

  ft::pair<iterator, bool> insert_unique(const Value &value) {
    node_type *parent = nil_node_;
    node_type *current = root_;
    while (!current->is_nil_node_) {
      parent = current;
      if (__are_keys_equal(KeyOfValue()(value),
                           KeyOfValue()(current->value_))) {
        return ft::pair<iterator, bool>(iterator(current), false);
      } else if (Compare()(KeyOfValue()(value),
                           KeyOfValue()(current->value_))) {
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    node_type *new_node = __alloc_new_node(value);
    new_node->parent_ = parent;
    if (parent->is_nil_node_) {
      root_ = new_node;
    } else if (Compare()(KeyOfValue()(new_node->value_),
                         KeyOfValue()(parent->value_))) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
    __insert_fixup(new_node);
    __update_tree_info_based_on_new_node(new_node);

    return ft::pair<iterator, bool>(iterator(new_node), true);
  }

  iterator insert_unique(iterator hint_it, const Value &value) {
    iterator prev_it = iterator(hint_it);
    if (hint_it != begin()) {
      --prev_it;
    }
    iterator next_it = iterator(hint_it);
    if (hint_it != end()) {
      ++next_it;
    }

    if (__are_keys_equal(KeyOfValue()(value), KeyOfValue()(*hint_it))) {
      // hint_it == value
      return iterator(hint_it);
    } else if (__are_keys_equal(KeyOfValue()(value), KeyOfValue()(*prev_it))) {
      // --hint_it == value
      return iterator(prev_it);
    } else if (__are_keys_equal(KeyOfValue()(value), KeyOfValue()(*next_it))) {
      // ++hint_it == value
      return iterator(next_it);
    } else if (Compare()(KeyOfValue()(value), KeyOfValue()(*hint_it)) &&
               Compare()(KeyOfValue()(*prev_it), KeyOfValue()(value)) &&
               hint_it.node_->left_->is_nil_node_) {
      // --hint_it < value < hint_it なら hint_it の左に挿入
      node_type *new_node = __alloc_new_node(value);
      hint_it.node_->left_ = new_node;
      new_node->parent_ = hint_it.node_;
      __insert_fixup(new_node);
      __update_tree_info_based_on_new_node(new_node);
      return iterator(new_node);
    } else if (Compare()(KeyOfValue()(*hint_it), KeyOfValue()(value)) &&
               Compare()(KeyOfValue()(value), KeyOfValue()(*next_it)) &&
               hint_it.node_->right_->is_nil_node_) {
      // hint_it < value < ++hint_it なら hint_it の右に挿入
      node_type *new_node = __alloc_new_node(value);
      hint_it.node_->right_ = new_node;
      new_node->parent_ = hint_it.node_;
      __insert_fixup(new_node);
      __update_tree_info_based_on_new_node(new_node);
      return iterator(new_node);
    } else {
      // 入れられ無さそうなら普通にルートから入れる場所を探して入れる
      return insert_unique(value).first;
    }
  }

  template <class InputIt>
  void insert_range(InputIt first, InputIt last) {
    for (; first != last; ++first) {
      insert_unique(*first);
    }
  }

  /********** Search **********/

  node_type *search_key_node(const Key &key) const {
    node_type *current = root_;
    while (!current->is_nil_node_ &&
           !__are_keys_equal(KeyOfValue()(current->value_), key)) {
      if (Compare()(key, KeyOfValue()(current->value_))) {
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return current;
  }

  Value &operator[](const Key &key) const {
    node_type *node = search_key_node(key);
    if (node->is_nil_node_) {
      throw std::out_of_range("key isn't in the tree.");
    }
    return node->value_;
  }

  allocator_type get_allocator() const {
    return allocator_type(node_allocator());
  }

  /********** Iterators **********/

  iterator begin() {
    return iterator(begin_node_);
  }

  const_iterator begin() const {
    return const_iterator(begin_node_);
  }

  iterator end() {
    return iterator(end_node_);
  }

  const_iterator end() const {
    return const_iterator(end_node_);
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

  /********** Capacity **********/

  bool empty() const {
    return node_count_ == 0;
  }

  size_type size() const {
    return node_count_;
  }

  size_type max_size() const {
    return node_allocator().max_size();
  }

  /********** Modifiers **********/

  void clear() {
    __delete_tree(root_);
    root_ = nil_node_;
    begin_node_ = root_;
    node_count_ = 0;
  }

  void erase(iterator pos) {
    __delete_node_from_tree(pos.get_node_ptr());
  }

  void erase(iterator first, iterator last) {
    iterator it = first;
    for (; it != last;) {
      iterator next_it = it;
      ++next_it;
      erase(it);
      it = next_it;
    }
  }

  size_type erase(const Key &key) {
    // Search(key) の結果が nil_node だった場合には何もしない
    node_type *target_node = search_key_node(key);
    if (target_node->is_nil_node_) {
      return 0;
    }
    if (target_node == begin_node_) {
      begin_node_ = get_next_node<Value>(target_node);
    }
    __delete_node_from_tree(target_node);
    root_->parent_ = end_node_;
    end_node_->left_ = root_;
    end_node_->right_ = root_;
    return 1;
  }

  void swap(RedBlackTree &other) {
    std::swap(nil_node_, other.nil_node_);
    std::swap(root_, other.root_);
    std::swap(node_count_, other.node_count_);
    std::swap(begin_node_, other.begin_node_);
    std::swap(end_node_, other.end_node_);
  }

  /********** Lookup **********/

  size_type count(const Key &key) const {
    node_type *node = search_key_node(key);
    return !node->is_nil_node_;
  }

  iterator find(const Key &key) {
    node_type *node = search_key_node(key);
    if (node->is_nil_node_) {
      return end();
    }
    return iterator(node);
  }

  const_iterator find(const Key &key) const {
    node_type *node = search_key_node(key);
    if (node->is_nil_node_) {
      return end();
    }
    return const_iterator(node);
  }

  /* Returns an iterator pointing to the first element that is not less than
   * (i.e. greater or equal to) key.
   */
  iterator lower_bound(const key_type &key) {
    node_type *current = root_;
    node_type *low_node = end_node_;
    while (!current->is_nil_node_) {
      if (!Compare()(KeyOfValue()(current->value_), key)) {
        low_node = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return iterator(low_node);
  }

  const_iterator lower_bound(const key_type &key) const {
    node_type *current = root_;
    node_type *low_node = end_node_;
    while (!current->is_nil_node_) {
      if (!Compare()(KeyOfValue()(current->value_), key)) {
        low_node = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return const_iterator(low_node);
  }

  /* Returns an iterator pointing to the first element that is greater than
  key.
   */
  iterator upper_bound(const key_type &key) {
    node_type *current = root_;
    node_type *high_node = end_node_;
    while (!current->is_nil_node_) {
      if (Compare()(key, KeyOfValue()(current->value_))) {
        high_node = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return iterator(high_node);
  }

  const_iterator upper_bound(const key_type &key) const {
    node_type *current = root_;
    node_type *high_node = end_node_;
    while (!current->is_nil_node_) {
      if (Compare()(key, KeyOfValue()(current->value_))) {
        high_node = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return const_iterator(high_node);
  }

  ft::pair<iterator, iterator> equal_range(const Key &key) {
    return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
  }

  ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
    return ft::pair<const_iterator, const_iterator>(lower_bound(key),
                                                    upper_bound(key));
  }

  /********** Observers **********/

  Compare key_comp() const {
    return Compare();
  }

  /********** Debug **********/
  void print_tree_2D() const {
    __print_tree_2D_util(root_);
  }

  int get_height() {
    return get_height(root_);
  }

  int get_height(node_type *current, int count = 0) {
    if (current->is_nil_node_) {
      return count;
    }
    int left_count = get_height(current->left_, count + 1);
    int right_count = get_height(current->right_, count + 1);
    return left_count > right_count ? left_count : right_count;
  }

  void print_node_info(node_type *node) {
    std::cout << "Key: " << KeyOfValue()(node->value_)
              << "\n\tValue: " << node->value_ << "\n\tColor: " << node->color_
              << "\n\tParent: " << KeyOfValue()(node->parent_->value_)
              << "\n\tLeft: " << KeyOfValue()(node->left_->value_)
              << "\n\tRight: " << KeyOfValue()(node->right_->value_)
              << std::endl;
  }

#ifdef DEBUG
 public:
#else
 private:
#endif

  /********** Debug **********/
  void __print_tree_2D_util(node_type *root, int space = 0) const;

  /********** TreeOperation **********/
  void __rotate_left(node_type *x);
  void __rotate_right(node_type *x);

  /********** Insert **********/
  void __insert_fixup(node_type *new_node);
  void __update_tree_info_based_on_new_node(node_type *new_node);

  /********** Delete **********/
  void __delete_transplant(node_type *u, node_type *v);
  void __delete_fixup(node_type *x);

  /********** Node operations **********/
  void __delete_node_from_tree(node_type *z);
  void __delete_tree(node_type *root);
  void __delete_node(node_type *z);
  node_type *__copy_tree(node_type *other_root, node_type *other_nil_node);
  node_type *__alloc_new_node(value_type value);
  node_type *__alloc_nil_node();
  node_type *__copy_node(const node_type *z, const node_type *nil_node);

  /********** Utils **********/
  bool __are_keys_equal(const key_type &key1, const key_type &key2) const;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__print_tree_2D_util(
    node_type *root, int space) const {
  if (root->is_nil_node_)
    return;

  space += 10;

  // print right first
  __print_tree_2D_util(root->right_, space);

  // print current node after space
  std::cout << std::endl;
  for (int i = 10; i < space; i++) std::cout << " ";
  std::cout << KeyOfValue()(root->value_)
            << (root->color_ == node_type::RED ? "(R)" : "(B)") << std::endl;

  // print left
  __print_tree_2D_util(root->left_, space);
}

/*
 * RotateLeft
 *
 * xを11とし, xを中心に左回転すると以下のようになる. この場合yは18となる.
 *
 *    root                               root
 *     |                                  |
 *     7                                  7
 *   /   \                              /   \
 *  4     11                           4     18
 *       /  \     -- RotateLeft -->         /  \
 *      9   18                             11   19
 *         /  \                           /  \
 *        14  19                         9   14
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__rotate_left(
    node_type *x) {
  node_type *y = x->right_;  // y を x の右の子とする.
  x->right_ = y->left_;      // y の左部分木を x の右部分木にする.
  if (!y->left_->is_nil_node_) {
    // yの左の子の親が左回転後のxになるようにする
    y->left_->parent_ = x;
  }
  y->parent_ = x->parent_;  // xの親をyにする
  // xの親の左右どちらにyを付けるか
  if (x->parent_->is_nil_node_) {
    // xが根だった場合
    root_ = y;
  } else if (x == x->parent_->left_) {
    x->parent_->left_ = y;
  } else {
    x->parent_->right_ = y;
  }
  // xをyの左の子とする.
  y->left_ = x;
  x->parent_ = y;
}

/* RotateRight
 *
 * xを6とし, xを中心に右回転すると以下のようになる. この場合yは4となる.
 *
 *       root                                root
 *        |                                   |
 *        10                                  10
 *      /   \                               /   \
 *     6     12                            4     12
 *    /  \         -- RotateRight -->     /  \
 *   4    8                              2    6
 *  /  \                                     / \
 * 2    5                                   5   8
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__rotate_right(
    node_type *x) {
  node_type *y = x->left_;  // y を x の左の子とする.
  x->left_ = y->right_;     // y の右部分木を x の左部分木にする.
  if (!y->right_->is_nil_node_) {
    // yの右の子の親が右回転後のxになるようにする
    y->right_->parent_ = x;
  }
  y->parent_ = x->parent_;  // xの親をyにする
  // xの親の左右どちらにyを付けるか
  if (x->parent_->is_nil_node_) {
    // xが根だった場合
    root_ = y;
  } else if (x == x->parent_->right_) {
    x->parent_->right_ = y;
  } else {
    x->parent_->left_ = y;
  }
  // xをyの右の子とする.
  y->right_ = x;
  x->parent_ = y;
}

/* 挿入時にRBTreeの2色条件を維持するための関数
 *
 * 修正パターンは3通り * 左右2通り で合計6通りある.
 * 以下に左の修正パターン3つを示す. 右バージョンは左右逆にすれば良い.
 *
 * R: Red
 * B: Black
 *
 * g: grant parent
 * p: parent
 * u: uncle
 * n: new node
 *
 * 修正パターン1: 叔父ノードが赤色.
 *              g_B                                          g_R
 *             /   \                                        /   \
 *           p_R    u_R     -- Change color g,p,u -->     p_B   u_B
 *           /                                            /
 *         n_R                                          n_R
 *
 *                 MEMO: gを赤に彩色することで, 2色条件の5番が保たれる
 *
 *         -- Recheck parents colors from g_R to root -->     ...
 *
 * 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
 *              g_B                                     p_R
 *             /   \                                   /   \
 *           p_R    u_B     -- Rotate right g -->    n_R    g_B
 *           /                                                \
 *         n_R                                                u_B
 *
 *                                             p_B
 *                                            /   \
 *         -- Change color p,g -->          n_R   g_R
 *                                                  \
 *                                                  u_B
 *
 * 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の右の子
 *              g_B                                       g_B
 *             /   \                                     /   \
 *           p_R    u_B     -- Rotate Left p -->       n_R    u_B
 *             \                                       /
 *             n_R                                   p_R
 *
 *                                         n_B
 *                                        /   \
 *         -- Apply pattern2 -->        p_R    g_R
 *                                              \
 *                                               u_B
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__insert_fixup(
    node_type *new_node) {
  // 新しいノードは赤色であり, 赤ノードは赤ノードを子に持つことは出来ない.
  while (new_node->parent_->color_ == node_type::RED) {
    if (new_node->parent_ == new_node->parent_->parent_->left_) {
      // 新しいノードの親は祖父の左の子
      // 叔父ノード
      node_type *uncle_node = new_node->parent_->parent_->right_;
      if (uncle_node->color_ == node_type::RED) {
        // 修正パターン1: 親と叔父ノードが赤色
        // 親と叔父ノードを黒にし, 祖父を赤にする.
        new_node->parent_->color_ = node_type::BLACK;
        uncle_node->color_ = node_type::BLACK;
        new_node->parent_->parent_->color_ = node_type::RED;
        new_node = new_node->parent_->parent_;
      } else {
        // 修正パターン3 の後半部分の処理は パターン2 と同じなのでまとめられる

        if (new_node == new_node->parent_->right_) {
          // 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の右の子
          new_node = new_node->parent_;
          __rotate_left(new_node);
        }
        // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
        new_node->parent_->color_ = node_type::BLACK;
        new_node->parent_->parent_->color_ = node_type::RED;
        __rotate_right(new_node->parent_->parent_);
      }
    } else {
      // 新しいノードの親は祖父の右の子
      // 叔父ノード
      node_type *uncle_node = new_node->parent_->parent_->left_;
      if (uncle_node->color_ == node_type::RED) {
        // 修正パターン1: 親と叔父ノードが赤色
        // 親と叔父ノードを黒にし, 祖父を赤にする.
        new_node->parent_->color_ = node_type::BLACK;
        uncle_node->color_ = node_type::BLACK;
        new_node->parent_->parent_->color_ = node_type::RED;
        new_node = new_node->parent_->parent_;
      } else {
        // 修正パターン3 の後半部分の処理は パターン2 と同じなのでまとめられる

        if (new_node == new_node->parent_->left_) {
          // 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の左の子
          new_node = new_node->parent_;
          __rotate_right(new_node);
        }
        // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の右の子
        new_node->parent_->color_ = node_type::BLACK;
        new_node->parent_->parent_->color_ = node_type::RED;
        __rotate_left(new_node->parent_->parent_);
      }
    }
  }
  root_->color_ = node_type::BLACK;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::
    __update_tree_info_based_on_new_node(node_type *new_node) {
  // begin_node_ の更新
  if (begin_node_->is_nil_node_ ||
      Compare()(KeyOfValue()(new_node->value_),
                KeyOfValue()(begin_node_->value_))) {
    begin_node_ = new_node;
  }
  // end_node_ の子が新たなルートを指すようにする
  root_->parent_ = end_node_;
  end_node_->left_ = root_;
  end_node_->right_ = root_;
  ++node_count_;
}

/* ノードの削除
 *
 * q: 削除ノードの親ノード(左右は問わない)
 * z: 削除ノード
 * l: 削除ノードの左の子
 * r: 削除ノードの右の子
 * 1,2,3...: その他のノード
 *
 * パターン1: 削除ノードが左の子を持たない場合
 *      q                     q
 *      |                     |
 *      z          -->        r
 *       \
 *        r
 *
 * パターン2: 削除ノードが右の子を持たない場合
 *      q                     q
 *      |                     |
 *      z          -->        l
 *     /
 *    l
 *
 * パターン3: 削除ノードが2つの子を持ち,
 *            削除ノードの右の子が次節点の場合
 *      q                       q
 *      |                       |
 *      z          -->          r
 *     / \                     / \
 *    l   r                   l   1
 *         \
 *          1
 *
 * パターン4: 削除ノードが2つの子を持ち,
 *            次節点が削除ノードの右の部分木内にある場合
 *            (ただし削除ノードの右の子は次節点ではない)
 *      q                                                 q
 *      |                                                 |
 *      z        -- y(2)の場所をx(3)で置き換える -->      z
 *     / \         (次節点は左の子を持たない)            / \
 *    l   r                                             l   r     2
 *       /                                                 /
 *      2                                                 3
 *       \
 *        3
 *
 *                                      q
 *                                      |
 *  -- zの部分をy(2)に置き換える -->    2
 *                                     / \
 *                                    l   r
 *                                       /
 *                                      3
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare,
                  Alloc>::__delete_node_from_tree(node_type *z) {
  // yがもともと置かれていた場所に移動する節点
  node_type *x;
  // 木からの削除あるいは木の中の移動が想定される節点.
  // zの子が1つの場合は削除対象, zの子が2つの場合は移動対象.
  node_type *y = z;
  // 節点yを再彩色する可能性があるので元の色を保持する.
  // yの代入直後のyの色を覚えておく.
  typename node_type::Color y_original_color = y->color_;

  if (z->left_->is_nil_node_) {
    x = z->right_;
    __delete_transplant(z, z->right_);
  } else if (z->right_->is_nil_node_) {
    x = z->left_;
    __delete_transplant(z, z->left_);
  } else {
    // 削除ノードが2つ子を持つ場合
    y = find_minimum_node(z->right_);
    y_original_color = y->color_;
    x = y->right_;
    if (y->parent_ == z) {
      // 削除ノードの右の子が次節点の場合
      x->parent_ = y;
    } else {
      __delete_transplant(y, y->right_);
      y->right_ = z->right_;
      y->right_->parent_ = y;
    }
    __delete_transplant(z, y);
    y->left_ = z->left_;
    y->left_->parent_ = y;
    y->color_ = z->color_;
  }
  __delete_node(z);

  if (y_original_color == node_type::BLACK) {
    // yが黒ならば, Deleteの操作によって2色条件が崩れた可能性がある.
    // x(もともとyがあった場所)から上に2色条件を違反していないか見ていく
    //
    // yが赤ならば以下の3つの理由によってyの削除あるいは移動は2色条件を維持する.
    // 1. 木の黒高さは変化しない.
    // 2. 2つの赤節点が連続することは無い.
    //    なぜならyはzの色を継承した上でzの場所に移されるので,
    //    yの木の新しい場所で2つの赤節点が連続することはない.
    // 3. yが赤ならば, yが根であるはずはなく, 根は依然として黒である.
    //
    // yが黒ならば以下の3つの問題が発生する可能性がある.
    // 1. 元々はyが根であって, yの赤の子が新しい根になった時,
    //    2色条件その2(根は黒である)に違反する.
    // 2. xとx.parentが共に赤の時,
    //    2色条件その4(赤ノードは黒2つを子に持つ)に違反する.
    // 3. 木の中をyが移動すると,
    //    元々yを含んでいた単純道の黒節点数が1減少する可能性がある.
    //    これは2色条件その5(任意のノードは子孫の葉までに含まれる黒節点数は一定である)
    //    に違反する.
    __delete_fixup(x);
  }
  node_count_--;
}

// ある節点の子であるuを根とする部分木を別の節点の子のvを根とする部分木に置き換える.
// v.left_, v.right_ に対しては変更を行わないので注意.
// ノードを削除する際に使う
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__delete_transplant(
    node_type *u, node_type *v) {
  if (u->parent_->is_nil_node_) {
    // uが根のとき
    root_ = v;
  } else if (u == u->parent_->left_) {
    u->parent_->left_ = v;
  } else {
    u->parent_->right_ = v;
  }
  v->parent_ = u->parent_;
}

/* ノード削除によって2色条件が違反が発生したか調べ, 発生していたら修正する.
 *
 * R: 赤
 * B: 黒
 * p: 親
 * x: 今見ているノード
 * b: 兄弟
 * l: 兄弟の左の子
 * r: 兄弟の右の子
 *
 *
 * 修正するパターンは兄弟の色と兄弟の子の色によって以下の4つに分けられる.
 * (明示的に色指定されていないノードは何色でも良い(例えば親ノードとか))
 * 基本的にxが親の左の子という想定で書いてる.
 * 右バージョンは左右逆にすれば良い.
 *
 * 修正パターン1: 兄弟が赤
 *       p_B                                             p_R
 *      /   \                                           /   \
 *    x_B   b_R        -- 親と兄弟の色を変える -->    x_B   b_B
 *         /   \                                           /   \
 *       l_B   r_B                                       l_B   r_B
 *
 *                              b_B
 *                             /   \
 *    -- 親を左回転 -->      p_R   r_B
 *                          /   \
 *                        x_B   l_B
 *
 *    -- 修正パターン2,3,4で修正する -->
 *
 * 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
 *       p_B                                   p_B
 *      /   \                                 /   \
 *    x_B   b_B      -- 兄弟を赤にする -->  x_B   b_R
 *         /   \                                 /   \
 *       l_B   r_B                             l_B   r_B
 *
 *    -- xを親に設定し, ループを継続 -->
 *
 * 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
 *       p_B                                                        p_B
 *      /   \                                                      /   \
 *    x_B   b_B      -- 兄弟と兄弟の左の子で色を交換する -->     x_B   b_R
 *         /   \                                                      /   \
 *       l_R   r_B                                                  l_B   r_B
 *
 *       p_B                                 p_B
 *      /   \                               /   \
 *    x_B   b_R      -- 兄弟を右回転 -->  x_B   l_B
 *         /   \                                  \
 *       l_B   r_B                                b_R
 *                                                  \
 *                                                  r_B
 *
 *    -- 修正パターン4で修正する -->
 *
 * 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤 (左の子は赤でも黒でもいい)
 *       p_R                                                       p_B
 *      /   \                                                     /   \
 *    x_B   b_B     -- 親と兄弟の色を交換 & 兄弟の右を黒に -->  x_B   b_R
 *         /   \                                                     /   \
 *       l_B   r_R                                                 l_B   r_B
 *
 *                          b_R
 *                         /   \
 *    -- 親を左回転 -->  p_B   r_B
 *                      /   \
 *                    x_B   l_B
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__delete_fixup(
    node_type *x) {
  while (x != root_ && x->color_ == node_type::BLACK) {
    if (x == x->parent_->left_) {
      // 兄弟ノード
      node_type *w = x->parent_->right_;
      if (w->color_ == node_type::RED) {
        // 修正パターン1: 兄弟が赤
        w->color_ = node_type::BLACK;
        x->parent_->color_ = node_type::RED;
        __rotate_left(x->parent_);
        w = x->parent_->right_;
      }
      if (w->left_->color_ == node_type::BLACK &&
          w->right_->color_ == node_type::BLACK) {
        // 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
        w->color_ = node_type::RED;
        x = x->parent_;
      } else {
        if (w->right_->color_ == node_type::BLACK) {
          // 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
          w->left_->color_ = node_type::BLACK;
          w->color_ = node_type::RED;
          __rotate_right(w);
          w = x->parent_->right_;
        }
        // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
        w->color_ = x->parent_->color_;
        x->parent_->color_ = node_type::BLACK;
        w->right_->color_ = node_type::BLACK;
        __rotate_left(x->parent_);
        x = root_;
      }
    } else {
      // 兄弟ノード
      node_type *w = x->parent_->left_;
      if (w->color_ == node_type::RED) {
        // 修正パターン1: 兄弟が赤
        w->color_ = node_type::BLACK;
        x->parent_->color_ = node_type::RED;
        __rotate_right(x->parent_);
        w = x->parent_->left_;
      }
      if (w->right_->color_ == node_type::BLACK &&
          w->left_->color_ == node_type::BLACK) {
        // 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
        w->color_ = node_type::RED;
        x = x->parent_;
      } else {
        if (w->left_->color_ == node_type::BLACK) {
          // 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
          w->right_->color_ = node_type::BLACK;
          w->color_ = node_type::RED;
          __rotate_left(w);
          w = x->parent_->left_;
        }
        // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
        w->color_ = x->parent_->color_;
        x->parent_->color_ = node_type::BLACK;
        w->left_->color_ = node_type::BLACK;
        __rotate_right(x->parent_);
        x = root_;
      }
    }
  }
  x->color_ = node_type::BLACK;
}

/* 木の全てのノードを削除する.
 *
 * 注意: root_, node_count, begin_node_ などのメンバー変数は更新されない.
 */
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__delete_tree(
    node_type *root) {
  // 注意: NILノードは __delete_tree()
  // を呼び出した後に呼び出し側で呼ぶ必要がある
  if (root->is_nil_node_) {
    return;
  }
  __delete_tree(root->left_);
  __delete_tree(root->right_);
  __delete_node(root);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__delete_node(
    node_type *z) {
  node_allocator allocator = node_allocator();
  allocator.destroy(z);
  allocator.deallocate(z, 1);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::node_type *
RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__copy_tree(
    node_type *other_root, node_type *other_nil_node) {
  if (other_root == other_nil_node) {
    return nil_node_;
  }
  node_type *copy_root = __copy_node(other_root, other_nil_node);
  copy_root->parent_ = nil_node_;
  copy_root->left_ = __copy_tree(other_root->left_, other_nil_node);
  copy_root->left_->parent_ = copy_root;
  copy_root->right_ = __copy_tree(other_root->right_, other_nil_node);
  copy_root->right_->parent_ = copy_root;
  return copy_root;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::node_type *
RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__alloc_new_node(
    value_type value) {
  node_allocator allocator = node_allocator();
  node_type *new_node = allocator.allocate(1);
  allocator.construct(new_node, value);
  new_node->left_ = nil_node_;
  new_node->right_ = nil_node_;
  new_node->color_ = node_type::RED;  // 新しいノードの色は最初は赤に設定される
  return new_node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::node_type *
RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__alloc_nil_node() {
  node_allocator allocator = node_allocator();
  node_type *nil_node = allocator.allocate(1);
  allocator.construct(nil_node, Value(), true);
  nil_node->parent_ = nil_node;
  nil_node->left_ = nil_node;
  nil_node->right_ = nil_node;
  nil_node->color_ = node_type::BLACK;
  return nil_node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::node_type *
RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__copy_node(
    const node_type *z, const node_type *nil_node) {
  node_allocator allocator = node_allocator();
  if (z == nil_node) {
    return nil_node_;
  }
  node_type *new_node = allocator.allocate(1);
  allocator.construct(new_node, *z);
  return new_node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>::__are_keys_equal(
    const key_type &key1, const key_type &key2) const {
  return !Compare()(key1, key2) && !Compare()(key2, key1);
}
}  // namespace ft

#endif /* RED_BLACK_TREE_H_ */
