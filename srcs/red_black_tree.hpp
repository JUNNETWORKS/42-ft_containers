#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

#include <cstddef>
#include <iostream>

#include "stack.hpp"

namespace ft {

template <class Value>
struct RBTNode;

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

  rbtree_iterator() : node_() {}

  explicit rbtree_iterator(node_pointer ptr) : node_(ptr) {}

  self_type &operator=(const self_type &other) {
    if (this != &other) {
      node_ = other.node_;
    }
    return *this;
  }

  reference operator*() const {
    return node_;
  }

  pointer operator->() const {
    return node_;
  }

  self_type &operator++() {
    node_ = TreeSuccessor(node_);
    return node_;
  }

  self_type operator++(int) {
    self_type tmp = *this;
    node_ = TreeSuccessor(node_);
    return tmp;
  }

  self_type &operator--() {
    node_ = TreeSuccessor(node_);
    return node_;
  }

  self_type operator--(int) {
    self_type tmp = *this;
    node_ = TreeSuccessor(node_);
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
    return iterator(const_cast<iterator::node_pointer>(node_));
  }

  reference operator*() const {
    return node_;
  }

  pointer operator->() const {
    return node_;
  }

  self_type &operator++() {
    node_ = TreeSuccessor(node_);
    return node_;
  }

  self_type operator++(int) {
    self_type tmp = *this;
    node_ = TreeSuccessor(node_);
    return tmp;
  }

  self_type &operator--() {
    node_ = TreeSuccessor(node_);
    return node_;
  }

  self_type operator--(int) {
    self_type tmp = *this;
    node_ = TreeSuccessor(node_);
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

 private:
  // Members
  node_type nil_node_object_;  // NIL node isn't stored in heap area.
  node_type *nil_node_;        // point to nil_node_object
  node_type *root_;
  // begin() と end() を O(1) でアクセスするためにメンバー変数にもたせておく
  node_type *begin_node;
  node_type *end_node;

 public:
  // Constructor, Descructor

  RedBlackTree()
      : nil_node_object_(Value(), true),
        nil_node_(&nil_node_object_),
        root_(nil_node_) {
    nil_node_->parent_ = nil_node_;
    nil_node_->left_ = nil_node_;
    nil_node_->right_ = nil_node_;
    nil_node_->color_ = node_type::BLACK;
  }

  RedBlackTree(const RedBlackTree &other)
      : nil_node_object_(Value(), true),
        nil_node_(&nil_node_object_),
        root_(nil_node_) {
    operator=(other);
  }

  RedBlackTree &operator=(const RedBlackTree &rhs) {
    if (&rhs != this) {
      // ノードをディープコピー
      root_ = CopyTree(rhs.root_, rhs.nil_node_);
    }
    return *this;
  }

  ~RedBlackTree() {
    // 全てのノードをdeleteする
    DeleteTree(root_);
  }

  // Tree operations
  // InsertUniqueは現在木に挿入しようとしているkeyが木に無いという前提で動作する。
  // keyが既に木にあるか、value挿入するかそれとも既存のノードの値を更新するか
  // 判断するのはmapなどの利用側の責務とする。
  void InsertUnique(const Value &value) {
    node_type *parent = nil_node_;
    node_type *current = root_;
    while (!current->is_nil_node_) {
      parent = current;
      if (Compare()(KeyOfValue()(value), KeyOfValue()(current->value_))) {
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    node_type *new_node = AllocNewNode(value);
    new_node->parent_ = parent;
    if (parent->is_nil_node_) {
      root_ = new_node;
    } else if (Compare()(KeyOfValue()(new_node->value_),
                         KeyOfValue()(parent->value_))) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
    new_node->left_ = nil_node_;
    new_node->right_ = nil_node_;
    new_node->color_ =
        node_type::RED;  // 新しいノードの色は最初は赤に設定される
    InsertFixup(new_node);
  }

  void Delete(Key key) {
    // Search(key) の結果が nil_node だった場合には何もしない
    node_type *target_node = Search(key);
    if (!target_node->is_nil_node_)
      DeleteNodeFromTree(target_node);
  }

  Value &operator[](const Key &key) const {
    node_type *node = Search(key);
    if (node->is_nil_node_) {
      throw std::out_of_range("key isn't in the tree.");
    }
    return node->value_;
  }

  node_type *Search(const Key &key) const {
    node_type *current = root_;
    while (!current->is_nil_node_ &&
           !KeysAreEqual(KeyOfValue()(current->value_), key)) {
      if (Compare()(key, KeyOfValue()(current->value_))) {
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return current;
  }

  /******************************************************************
   Debug methods
  *******************************************************************/

  void PrintTree2D() const {
    PrintTree2DUtil(root_);
  }

  void PrintTree2DUtil(node_type *root, int space = 0) const {
    if (root->is_nil_node_)
      return;

    space += 10;

    // print right first
    PrintTree2DUtil(root->right_, space);

    // print current node after space
    std::cout << std::endl;
    for (int i = 10; i < space; i++) std::cout << " ";
    std::cout << KeyOfValue()(root->value_)
              << (root->color_ == node_type::RED ? "(R)" : "(B)") << std::endl;

    // print left
    PrintTree2DUtil(root->left_, space);
  }

  int GetHeight() {
    return GetHeight(root_);
  }

  int GetHeight(node_type *current, int count = 0) {
    if (current->is_nil_node_) {
      return count;
    }
    int left_count = GetHeight(current->left_, count + 1);
    int right_count = GetHeight(current->right_, count + 1);
    return left_count > right_count ? left_count : right_count;
  }

  void PrintNodeInfo(node_type *node) {
    std::cout << "Key: " << KeyOfValue()(node->value_)
              << "\n\tValue: " << node->value_ << "\n\tColor: " << node->color_
              << "\n\tParent: " << KeyOfValue()(node->parent_->value_)
              << "\n\tLeft: " << KeyOfValue()(node->left_->value_)
              << "\n\tRight: " << KeyOfValue()(node->right_->value_)
              << std::endl;
  }

  // Fix tree

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
  void RotateLeft(node_type *x) {
    node_type *y = x->right_;  // y を x の右の子とする.
    x->right_ = y->left_;  // y の左部分木を x の右部分木にする.
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
  void RotateRight(node_type *x) {
    node_type *y = x->left_;  // y を x の左の子とする.
    x->left_ = y->right_;  // y の右部分木を x の左部分木にする.
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
  void InsertFixup(node_type *new_node) {
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
            RotateLeft(new_node);
          }
          // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
          new_node->parent_->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          RotateRight(new_node->parent_->parent_);
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
            RotateRight(new_node);
          }
          // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の右の子
          new_node->parent_->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          RotateLeft(new_node->parent_->parent_);
        }
      }
    }
    root_->color_ = node_type::BLACK;
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
  void DeleteNodeFromTree(node_type *z) {
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
      DeleteTransplant(z, z->right_);
    } else if (z->right_->is_nil_node_) {
      x = z->left_;
      DeleteTransplant(z, z->left_);
    } else {
      // 削除ノードが2つ子を持つ場合
      y = TreeMinimum(z->right_);
      y_original_color = y->color_;
      x = y->right_;
      if (y->parent_ == z) {
        // 削除ノードの右の子が次節点の場合
        x->parent_ = y;
      } else {
        DeleteTransplant(y, y->right_);
        y->right_ = z->right_;
        y->right_->parent_ = y;
      }
      DeleteTransplant(z, y);
      y->left_ = z->left_;
      y->left_->parent_ = y;
      y->color_ = z->color_;
    }
    DeleteNode(z);

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
      DeleteFixup(x);
    }
  }

  // ある節点の子であるuを根とする部分木を別の節点の子のvを根とする部分木に置き換える.
  // v.left_, v.right_ に対しては変更を行わないので注意.
  // ノードを削除する際に使う
  void DeleteTransplant(node_type *u, node_type *v) {
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
  void DeleteFixup(node_type *x) {
    while (x != root_ && x->color_ == node_type::BLACK) {
      if (x == x->parent_->left_) {
        // 兄弟ノード
        node_type *w = x->parent_->right_;
        if (w->color_ == node_type::RED) {
          // 修正パターン1: 兄弟が赤
          w->color_ = node_type::BLACK;
          x->parent_->color_ = node_type::RED;
          RotateLeft(x->parent_);
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
            RotateRight(w);
            w = x->parent_->right_;
          }
          // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
          w->color_ = x->parent_->color_;
          x->parent_->color_ = node_type::BLACK;
          w->right_->color_ = node_type::BLACK;
          RotateLeft(x->parent_);
          x = root_;
        }
      } else {
        // 兄弟ノード
        node_type *w = x->parent_->left_;
        if (w->color_ == node_type::RED) {
          // 修正パターン1: 兄弟が赤
          w->color_ = node_type::BLACK;
          x->parent_->color_ = node_type::RED;
          RotateRight(x->parent_);
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
            RotateLeft(w);
            w = x->parent_->left_;
          }
          // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
          w->color_ = x->parent_->color_;
          x->parent_->color_ = node_type::BLACK;
          w->left_->color_ = node_type::BLACK;
          RotateRight(x->parent_);
          x = root_;
        }
      }
    }
    x->color_ = node_type::BLACK;
  }

  void DeleteTree(node_type *deleting_target) {
    // 木の全てのノードを削除する.
    // 注意: NILノードは静的確保で確保されているのでメモリ解法処理不要
    if (deleting_target->is_nil_node_) {
      return;
    }
    DeleteTree(deleting_target->left_);
    DeleteTree(deleting_target->right_);
    DeleteNode(deleting_target);
  }

  void DeleteNode(node_type *z) {
    if (z->is_nil_node_) {
      return;
    }
    node_allocator allocator = node_allocator();
    allocator.destroy(z);
    allocator.deallocate(z, 1);
  }

  node_type *CopyTree(node_type *other_root, node_type *other_nil_node) {
    if (other_root == other_nil_node) {
      return nil_node_;
    }
    node_type *copy_root = CopyNode(other_root, other_nil_node);
    copy_root->parent_ = nil_node_;
    copy_root->left_ = CopyTree(other_root->left_, other_nil_node);
    copy_root->left_->parent_ = copy_root;
    copy_root->right_ = CopyTree(other_root->right_, other_nil_node);
    copy_root->right_->parent_ = copy_root;
    return copy_root;
  }

  node_type *AllocNewNode(value_type value) {
    node_allocator allocator = node_allocator();
    node_type *p = allocator.allocate(1);
    allocator.construct(p, value);
    return p;
  }

  node_type *CopyNode(const node_type *z, const node_type *nil_node) {
    node_allocator allocator = node_allocator();
    if (z == nil_node) {
      return nil_node_;
    }
    node_type *new_node = allocator.allocate(1);
    allocator.construct(new_node, *z);
    return new_node;
  }

  bool KeysAreEqual(const key_type &key1, const key_type &key2) const {
    return !Compare()(key1, key2) && !Compare()(key2, key1);
  }
};

template <class Value>
RBTNode<Value> *TreeMinimum(const RBTNode<Value> *root) {
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
RBTNode<Value> *TreeMaximum(const RBTNode<Value> *root) {
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
const RBTNode<Value> *TreeSuccessor(const RBTNode<Value> *current) {
  if (!current->right_->is_nil_node_) {
    // currentが右の子を持っている時は右の子の中の最小が次のノード
    return TreeMinimum(current->right_);
  } else {
    if (current == current->parent_->left_) {
      // currentが親の左の子で, なおかつ右に子を持っていない場合,
      // 次のノードは親である
      return current->parent_;
    } else {
      // currentが親の右の子で, なおかつ右に子を持たない
      // currentが左の子になるまで親を遡る.
      // NIL_Nodeまで達したのならcurrentは最後のノード
      const RBTNode<Value> *next_node = current->parent_;
      while (!next_node->is_nil_node_ && current == next_node->right_) {
        current = next_node;
        next_node = next_node->parent_;
      }
      if (next_node->is_nil_node_) {
        // currentは最後のノードだった
        return NULL;
      }
      return next_node;
    }
  }
}

// 中間順木巡回の逆順序での前の節点のポインタを返す
template <class Value>
const RBTNode<Value> *TreePredecessor(const RBTNode<Value> *current) {
  if (!current->left_->is_nil_node_) {
    // currentが左の子を持つ場合は左部分木の中の最大値
    // currentの次に小さい値
    return TreeMaximum(current->left_);
  } else {
    if (current == current->parent_->left_) {
      // currentが親の左の子で, なおかつ左に子を持たない
      // currentが右の子になるまで親を遡る.
      // NIL_Nodeまで達したのならcurrentは最後のノード
      const RBTNode<Value> *next_node = current->parent_;
      while (!next_node->is_nil_node_ && current == next_node->left_) {
        current = next_node;
        next_node = next_node->parent_;
      }
      if (next_node->is_nil_node_) {
        // currentは最後のノードだった
        return NULL;
      }
      return next_node;
    } else {
      // currentが親の右のノードで, currentが左の子を持たない場合,
      // 次のノードはcurrentの親
      return current->parent_;
    }
  }
}

}  // namespace ft

#endif /* RED_BLACK_TREE_H_ */
