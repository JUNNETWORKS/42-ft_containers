#include "red_black_tree.hpp"

#include "pair.hpp"

// KeyOfValueを使うためにinclude
// #include "map.hpp"
// #include "set.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace ft {
template <typename Pair>
struct Select1st {
  /// @c argument_type is the type of the argument
  typedef Pair argument_type;

  /// @c result_type is the return type
  typedef typename Pair::first_type result_type;

  typename Pair::first_type &operator()(Pair &p) const {
    return p.first;
  }

  const typename Pair::first_type &operator()(const Pair &p) const {
    return p.first;
  }
};
}  // namespace ft

namespace {
template <class key_type, class Compare>
bool __are_keys_equal(const key_type &key1, const key_type &key2) {
  return !Compare()(key1, key2) && !Compare()(key2, key1);
}

// テスト用の関数. 色の修正や木の回転などを行わない挿入.
template <class Key, class Value, typename KeyOfValue,
          typename Compare = std::less<Key> >
typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *
insertNodeWithoutFixup(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *
        *root,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *new_node) {
  typedef typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
      node_type;

  if (*root == nil_node) {
    *root = new_node;
    new_node->parent_ = nil_node;
    new_node->left_ = nil_node;
    new_node->right_ = nil_node;
    return new_node;
  }

  node_type *parent = nil_node;
  node_type *node = *root;
  while (node != nil_node) {
    if (__are_keys_equal<Key, Compare>(KeyOfValue()(new_node->value_),
                                       KeyOfValue()(node->value_))) {
      exit(1);
    } else if (Compare()(KeyOfValue()(new_node->value_),
                         KeyOfValue()(node->value_))) {
      parent = node;
      node = node->left_;
    } else {
      parent = node;
      node = node->right_;
    }
  }
  new_node->parent_ = parent;
  new_node->left_ = nil_node;
  new_node->right_ = nil_node;
  if (Compare()(KeyOfValue()(new_node->value_), KeyOfValue()(parent->value_))) {
    parent->left_ = new_node;
  } else {
    parent->right_ = new_node;
  }
  return new_node;
}

template <class Key, class Value, typename KeyOfValue,
          typename Compare = std::less<Key> >
typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *
insertNodeWithoutFixup(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *
        *root,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node,
    Value value,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type::Color
        color) {
  typedef typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
      node_type;
  node_type *new_node = new node_type(value);
  new_node->color_ = color;
  insertNodeWithoutFixup<Key, Value, KeyOfValue, Compare>(root, nil_node,
                                                          new_node);
  return new_node;
}

template <class Key, class Value, typename KeyOfValue, typename Compare>
void expectAllRedNodeHasTwoBlackChild(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *node,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node);

template <class Key, class Value, typename KeyOfValue, typename Compare>
int expectAllPathesAreSameBlackNodeCount(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *node,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node,
    int black_count = 0);

// テスト用の関数. 赤黒木のルールを守っているか確かめる.
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
template <class Key, class Value, class KeyOfValue, class Compare>
void expectRedBlackTreeKeepRules(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare> &rb_tree) {
  typedef typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
      node_type;

  // 2. 根は黒である. (たまにこの条件は省かれる)
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);

  // 3. 葉(NIL)は全て黒である. 葉は全て根と同じ色である.
  EXPECT_EQ(rb_tree.nil_node_->color_, node_type::BLACK);
  EXPECT_EQ(rb_tree.root_->color_, rb_tree.nil_node_->color_);

  // 4. 赤のノードは黒ノードを2つ子に持つ.
  expectAllRedNodeHasTwoBlackChild<Key, Value, KeyOfValue, Compare>(
      rb_tree.root_, rb_tree.nil_node_);

  // 5. 任意のノードについて,
  //    そのノードから子孫の葉までの道に含まれる黒ノードの数は,
  //    選んだ葉に依らず一定である.
  //    (この条件は,
  //    「根から葉までの道に含まれる黒いノードの数は、葉によらず一定である」
  //    と言い換えることができる)
  expectAllPathesAreSameBlackNodeCount<Key, Value, KeyOfValue, Compare>(
      rb_tree.root_, rb_tree.nil_node_);
}

template <class Key, class Value, class KeyOfValue, typename Compare>
void expectAllRedNodeHasTwoBlackChild(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *node,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node) {
  typedef typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
      node_type;

  if (node == nil_node) {
    return;
  }
  if (node->color_ == node_type::RED) {
    EXPECT_EQ(node->left_->color_, node_type::BLACK);
    EXPECT_EQ(node->right_->color_, node_type::BLACK);
  }
  expectAllRedNodeHasTwoBlackChild<Key, Value, KeyOfValue, Compare>(node->left_,
                                                                    nil_node);
  expectAllRedNodeHasTwoBlackChild<Key, Value, KeyOfValue, Compare>(
      node->right_, nil_node);
}

template <class Key, class Value, typename KeyOfValue, typename Compare>
int expectAllPathesAreSameBlackNodeCount(
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type *node,
    typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
        *nil_node,
    int black_count) {
  typedef typename ft::RedBlackTree<Key, Value, KeyOfValue, Compare>::node_type
      node_type;

  if (node == nil_node) {
    return black_count;
  }
  if (node->color_ == node_type::BLACK) {
    black_count++;
  }
  int left_count =
      expectAllPathesAreSameBlackNodeCount<Key, Value, KeyOfValue, Compare>(
          node->left_, nil_node, black_count);
  int right_count =
      expectAllPathesAreSameBlackNodeCount<Key, Value, KeyOfValue, Compare>(
          node->right_, nil_node, black_count);
  EXPECT_EQ(left_count, right_count);
  return left_count;
}

}  // namespace

TEST(RedBlackTree, BasicOperations) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  tree_type rb_tree;
  rb_tree.insert_unique(pair_type("c", 1));
  rb_tree.insert_unique(pair_type("b", 2));
  rb_tree.insert_unique(pair_type("a", 3));

  std::cout << "Height: " << rb_tree.get_height() << std::endl;
  rb_tree.print_tree_2D();

  std::cout << "a: " << rb_tree["a"].second << std::endl;
  std::cout << "b: " << rb_tree["b"].second << std::endl;
  std::cout << "c: " << rb_tree["c"].second << std::endl;

  EXPECT_EQ(rb_tree["c"], pair_type("c", 1));
  EXPECT_EQ(rb_tree["b"], pair_type("b", 2));
  EXPECT_EQ(rb_tree["a"], pair_type("a", 3));

  rb_tree.delete_key_node("a");
  rb_tree.delete_key_node("b");
  std::cout << "after delete a and b" << std::endl;
  std::cout << "Height: " << rb_tree.get_height() << std::endl;
  rb_tree.print_tree_2D();
}

TEST(Modifier, ModifyExistNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("a", 1));
  EXPECT_EQ(rb_tree["a"], pair_type("a", 1));
  rb_tree["a"].second = 2;
  EXPECT_EQ(rb_tree["a"], pair_type("a", 2));
  rb_tree.delete_key_node("a");
}

TEST(Modifier, DeleteAndAccessItShouldThrowException) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("a", 1));
  EXPECT_EQ(rb_tree["a"], pair_type("a", 1));
  rb_tree.delete_key_node("a");
  EXPECT_THROW(rb_tree["a"], std::out_of_range);
}

TEST(get_next_node, Random1000) {
  srand(time(NULL));
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;
  typedef std::set<int> set_type;

  const int loop_num = 1000;

  set_type s;
  tree_type rb_tree;

  for (int i = 0; i < loop_num; i++) {
    int num = rand() % 10000;
    s.insert(num);
  }
  for (set_type::iterator it = s.begin(); it != s.end(); ++it) {
    int num = *it;
    rb_tree.insert_unique(pair_type(num, num));
  }

  const node_type *node = ft::find_minimum_node(rb_tree.root_);  // begin
  for (set_type::iterator it = s.begin(); it != s.end(); ++it) {
    EXPECT_EQ(node->value_.first, *it);
    EXPECT_EQ(node->value_.second, *it);
    node = ft::get_next_node(node);
  }
  // 最後は end_node_ が返ってくる
  EXPECT_TRUE(node == rb_tree.end_node_);
}

TEST(get_next_node, DeleteNodeDuringIteration) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  for (int i = 0; i < 5; ++i) {
    rb_tree.insert_unique(pair_type(i, i));
  }
  const node_type *node = ft::find_minimum_node(rb_tree.root_);  // begin
  int i = 0;
  while (!node->is_nil_node_) {
    EXPECT_EQ(node->value_.first, i);
    if (i == 1) {
      // 2をスキップする
      ++i;
      rb_tree.delete_key_node(i);
    }
    node = ft::get_next_node(node);
    ++i;
  }
}

TEST(get_prev_node, Random1000) {
  srand(time(NULL));
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;
  typedef std::set<int> set_type;
  const int loop_num = 1000;

  set_type s;
  tree_type rb_tree;

  for (int i = 0; i < loop_num; i++) {
    int num = rand() % 10000;
    s.insert(num);
  }
  for (set_type::reverse_iterator rit = s.rbegin(); rit != s.rend(); ++rit) {
    int num = *rit;
    rb_tree.insert_unique(pair_type(num, num));
  }

  const node_type *node = ft::find_maximum_node(rb_tree.root_);  // rbegin
  for (set_type::reverse_iterator rit = s.rbegin(); rit != s.rend(); ++rit) {
    EXPECT_EQ(node->value_.first, *rit);
    EXPECT_EQ(node->value_.second, *rit);
    node = ft::get_prev_node(node);
  }
  // 最後は end_node が返ってくる
  EXPECT_EQ(node, rb_tree.end_node_);
}

TEST(get_prev_node, DeleteNodeDuringIteration) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  for (int i = 0; i < 5; ++i) {
    rb_tree.insert_unique(pair_type(i, i));
  }
  const node_type *node = ft::find_maximum_node(rb_tree.root_);  // rbegin
  int i = 4;
  while (!node->is_nil_node_) {
    EXPECT_EQ(node->value_.first, i);
    if (i == 2) {
      // 1をスキップする
      --i;
      rb_tree.delete_key_node(i);
    }
    node = ft::get_prev_node(node);
    --i;
  }
  // 最後のノードは end_node である
  EXPECT_EQ(node, rb_tree.end_node_);
}

TEST(RedBlackTree, CopyConstructor) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  srand(time(NULL));
  const int loop_num = 10;

  tree_type t1;

  for (int i = 1; i < loop_num; ++i) {
    t1.insert_unique(pair_type(i, rand()));
  }

  t1.insert_unique(pair_type(0, 1));

  tree_type t2 = t1;
  tree_type t3(t1);
  tree_type t4;
  // t4の木は代入演算子内で削除されるはずである。
  t4.insert_unique(pair_type(-1, -1));
  t4.insert_unique(pair_type(-2, -2));
  t4 = t1;

  // Deep Copy Check
  t2[0].second = 2;
  t3[0].second = 3;
  t4[0].second = 4;
  EXPECT_EQ(t1[0].second, 1);
  EXPECT_EQ(t2[0].second, 2);
  EXPECT_EQ(t3[0].second, 3);
  EXPECT_EQ(t4[0].second, 4);

  t1.delete_key_node(0);
  t2.delete_key_node(0);
  t3.delete_key_node(0);
  t4.delete_key_node(0);

  t1.print_tree_2D();

  const tree_type::node_type *n1 = ft::find_minimum_node(t1.root_);
  const tree_type::node_type *n2 = ft::find_minimum_node(t2.root_);
  const tree_type::node_type *n3 = ft::find_minimum_node(t3.root_);
  const tree_type::node_type *n4 = ft::find_minimum_node(t4.root_);
  while (!n1->is_nil_node_) {
    EXPECT_EQ(n1->value_.first, n2->value_.first);
    EXPECT_EQ(n1->value_.second, n2->value_.second);
    EXPECT_NE(n1, n2);
    EXPECT_EQ(n1->value_.first, n3->value_.first);
    EXPECT_EQ(n1->value_.second, n3->value_.second);
    EXPECT_NE(n1, n3);
    EXPECT_EQ(n1->value_.first, n4->value_.first);
    EXPECT_EQ(n1->value_.second, n4->value_.second);
    EXPECT_NE(n1, n4);

    printf("n1: %d, n2, %d, n3: %d, n4: %d\n", n1->value_.first,
           n2->value_.first, n3->value_.first, n4->value_.first);

    n1 = ft::get_next_node(n1);
    n2 = ft::get_next_node(n2);
    n3 = ft::get_next_node(n3);
    n4 = ft::get_next_node(n4);
  }
  // 最後は end_node_
  EXPECT_EQ(n1, t1.end_node_);
  EXPECT_EQ(n2, t2.end_node_);
  EXPECT_EQ(n3, t3.end_node_);
  EXPECT_EQ(n4, t4.end_node_);
}

// insert_unique

TEST(insert_unique, Random1000) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef std::set<int> set_type;

  tree_type rb_tree;
  set_type s;

  srand(time(NULL));

  for (int i = 0; i < 1000; ++i) {
    s.insert(rand());
  }
  for (set_type::iterator it = s.begin(), it_end = s.end(); it != it_end;
       ++it) {
    rb_tree.insert_unique(pair_type(*it, 0));
  }
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(insert_unique, Sorted1000) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  for (int i = 0; i < 1000; ++i) {
    rb_tree.insert_unique(pair_type(i, 0));
  }
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(insert_unique, UncleIsRedLeft) {
  /* 修正パターン1: 叔父ノードが赤色.
   *              g_B                                          g_R
   *             /   \                                        /   \
   *           p_R    u_R     -- Change color g,p,u -->     p_B   u_B
   *           /                                            /
   *         n_R                                          n_R
   *
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の左部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(4, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 10);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->value_.first, 5);
  ASSERT_EQ(left_subtree->color_, node_type::BLACK);
  ASSERT_EQ(left_subtree->right_->value_.first, 7);
  ASSERT_EQ(left_subtree->right_->color_, node_type::RED);
  ASSERT_EQ(left_subtree->left_->value_.first, 4);
  ASSERT_EQ(left_subtree->left_->color_, node_type::RED);

  rb_tree.insert_unique(pair_type(3, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 10);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->value_.first, 5);
  EXPECT_EQ(left_subtree->color_, node_type::RED);
  EXPECT_EQ(left_subtree->right_->value_.first, 7);
  EXPECT_EQ(left_subtree->right_->color_, node_type::BLACK);
  EXPECT_EQ(left_subtree->left_->value_.first, 4);
  EXPECT_EQ(left_subtree->left_->color_, node_type::BLACK);
  EXPECT_EQ(left_subtree->left_->left_->value_.first, 3);
  EXPECT_EQ(left_subtree->left_->left_->color_, node_type::RED);
}

TEST(insert_unique, UncleIsRedRight) {
  /* 修正パターン1: 叔父ノードが赤色.
   *              g_B                                        g_R
   *             /   \                                      /   \
   *           p_R    u_R     -- Change color g,p,u -->   p_B    u_B
   *                   \                                          \
   *                   n_R                                        n_R
   *
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の右部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(12, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 5);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->value_.first, 10);
  ASSERT_EQ(right_subtree->color_, node_type::BLACK);
  ASSERT_EQ(right_subtree->left_->value_.first, 7);
  ASSERT_EQ(right_subtree->left_->color_, node_type::RED);
  ASSERT_EQ(right_subtree->right_->value_.first, 12);
  ASSERT_EQ(right_subtree->right_->color_, node_type::RED);

  rb_tree.insert_unique(pair_type(15, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 5);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->value_.first, 10);
  EXPECT_EQ(right_subtree->color_, node_type::RED);
  EXPECT_EQ(right_subtree->left_->value_.first, 7);
  EXPECT_EQ(right_subtree->left_->color_, node_type::BLACK);
  EXPECT_EQ(right_subtree->right_->value_.first, 12);
  EXPECT_EQ(right_subtree->right_->color_, node_type::BLACK);
  EXPECT_EQ(right_subtree->right_->right_->value_.first, 15);
  EXPECT_EQ(right_subtree->right_->right_->color_, node_type::RED);
}

TEST(insert_unique, UncleIsBlackAndNewNodeIsLeftLeft) {
  /* 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
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
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の左部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(3, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 10);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->value_.first, 5);
  ASSERT_EQ(left_subtree->color_, node_type::BLACK);
  ASSERT_EQ(left_subtree->left_->value_.first, 3);
  ASSERT_EQ(left_subtree->left_->color_, node_type::RED);
  ASSERT_EQ(left_subtree->right_->value_.first, 7);
  ASSERT_EQ(left_subtree->right_->color_, node_type::BLACK);

  rb_tree.insert_unique(pair_type(1, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 10);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->value_.first, 3);
  EXPECT_EQ(left_subtree->color_, node_type::BLACK);
  EXPECT_EQ(left_subtree->left_->value_.first, 1);
  EXPECT_EQ(left_subtree->left_->color_, node_type::RED);
  EXPECT_EQ(left_subtree->right_->value_.first, 5);
  EXPECT_EQ(left_subtree->right_->color_, node_type::RED);
  EXPECT_EQ(left_subtree->right_->right_->value_.first, 7);
  EXPECT_EQ(left_subtree->right_->right_->color_, node_type::BLACK);
}

TEST(insert_unique, UncleIsBlackAndNewNodeIsRightRight) {
  /* 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の右の子
   *              g_B                                     p_R
   *             /   \                                   /   \
   *           u_B    p_R     -- Rotate left g -->     g_B    n_R
   *                   \                               /
   *                   n_R                            u_B
   *
   *                                              p_B
   *                                             /   \
   *         -- Change color p,g -->           g_R    n_R
   *                                           /
   *                                          u_B
   *
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の右部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(1, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(3, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 1);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->value_.first, 5);
  ASSERT_EQ(right_subtree->color_, node_type::BLACK);
  ASSERT_EQ(right_subtree->left_->value_.first, 3);
  ASSERT_EQ(right_subtree->left_->color_, node_type::BLACK);
  ASSERT_EQ(right_subtree->right_->value_.first, 7);
  ASSERT_EQ(right_subtree->right_->color_, node_type::RED);

  rb_tree.insert_unique(pair_type(10, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 1);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->value_.first, 7);
  EXPECT_EQ(right_subtree->color_, node_type::BLACK);
  EXPECT_EQ(right_subtree->right_->value_.first, 10);
  EXPECT_EQ(right_subtree->right_->color_, node_type::RED);
  EXPECT_EQ(right_subtree->left_->value_.first, 5);
  EXPECT_EQ(right_subtree->left_->color_, node_type::RED);
  EXPECT_EQ(right_subtree->left_->left_->value_.first, 3);
  EXPECT_EQ(right_subtree->left_->left_->color_, node_type::BLACK);
}

TEST(insert_unique, UncleIsBlackAndNewNodeIsLeftRight) {
  /* 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の右の子
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
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の左部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(3, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 10);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->value_.first, 5);
  ASSERT_EQ(left_subtree->color_, node_type::BLACK);
  ASSERT_EQ(left_subtree->left_->value_.first, 3);
  ASSERT_EQ(left_subtree->left_->color_, node_type::RED);
  ASSERT_EQ(left_subtree->right_->value_.first, 7);
  ASSERT_EQ(left_subtree->right_->color_, node_type::BLACK);

  rb_tree.insert_unique(pair_type(4, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 10);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->value_.first, 4);
  EXPECT_EQ(left_subtree->color_, node_type::BLACK);
  EXPECT_EQ(left_subtree->left_->value_.first, 3);
  EXPECT_EQ(left_subtree->left_->color_, node_type::RED);
  EXPECT_EQ(left_subtree->right_->value_.first, 5);
  EXPECT_EQ(left_subtree->right_->color_, node_type::RED);
  EXPECT_EQ(left_subtree->right_->right_->value_.first, 7);
  EXPECT_EQ(left_subtree->right_->right_->color_, node_type::BLACK);
}

TEST(insert_unique, UncleIsBlackAndNewNodeIsRightleft) {
  /* 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の左の子
   *              g_B                                     g_B
   *             /   \                                   /   \
   *           u_B    p_R     -- Rotate Right p -->    u_B    n_R
   *                  /                                        \
   *                n_R                                        p_R
   *
   *                                         n_B
   *                                        /   \
   *         -- Apply pattern2 -->        p_R    p_R
   *                                      /
   *                                    u_B
   * これをこのまま構築すると, 根は黒というルールによって根が黒になるので,
   * 根の右部分木としてこの木を構築する.
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(1, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(3, 0), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 0), node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->value_.first, 1);
  ASSERT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->value_.first, 5);
  ASSERT_EQ(right_subtree->color_, node_type::BLACK);
  ASSERT_EQ(right_subtree->left_->value_.first, 3);
  ASSERT_EQ(right_subtree->left_->color_, node_type::BLACK);
  ASSERT_EQ(right_subtree->right_->value_.first, 7);
  ASSERT_EQ(right_subtree->right_->color_, node_type::RED);

  rb_tree.insert_unique(pair_type(6, 0));

  EXPECT_EQ(rb_tree.root_->value_.first, 1);
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->value_.first, 6);
  EXPECT_EQ(right_subtree->color_, node_type::BLACK);
  EXPECT_EQ(right_subtree->right_->value_.first, 7);
  EXPECT_EQ(right_subtree->right_->color_, node_type::RED);
  EXPECT_EQ(right_subtree->left_->value_.first, 5);
  EXPECT_EQ(right_subtree->left_->color_, node_type::RED);
  EXPECT_EQ(right_subtree->left_->left_->value_.first, 3);
  EXPECT_EQ(right_subtree->left_->left_->color_, node_type::BLACK);
}

TEST(search_key_node, BasicOperations) {
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  for (int i = 0; i < 1000; ++i) {
    rb_tree.insert_unique(pair_type(i, i));
  }
  for (int i = 0; i < 1000; ++i) {
    EXPECT_EQ(rb_tree[i].second, i);
  }
}

TEST(delete_key_node, TargetNodeHasOnlyRightChild) {
  /* パターン1: 削除ノードが左の子を持たない場合
   *      q                     q
   *      |                     |
   *      z          -->        r
   *       \
   *        r
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type(5, 5));
  rb_tree.insert_unique(pair_type(7, 7));
  rb_tree.insert_unique(pair_type(9, 9));
  rb_tree.insert_unique(pair_type(11, 11));

  rb_tree.delete_key_node(9);

  EXPECT_EQ(rb_tree[5].second, 5);
  EXPECT_EQ(rb_tree[7].second, 7);
  EXPECT_EQ(rb_tree[11].second, 11);
  // TODO: ノードが削除されたかテストする
}

TEST(delete_key_node, TargetNodeHasOnlyLeftChild) {
  /* パターン2: 削除ノードが右の子を持たない場合
   *      q                     q
   *      |                     |
   *      z          -->        l
   *     /
   *    l
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type(5, 5));
  rb_tree.insert_unique(pair_type(4, 4));
  rb_tree.insert_unique(pair_type(3, 3));
  rb_tree.insert_unique(pair_type(2, 2));

  rb_tree.delete_key_node(3);

  EXPECT_EQ(rb_tree[5].second, 5);
  EXPECT_EQ(rb_tree[4].second, 4);
  EXPECT_EQ(rb_tree[2].second, 2);
}

TEST(delete_key_node, TargetNodeHasTwoChildAndRightChildIsNextNode) {
  /* パターン3: 削除ノードが2つの子を持ち,
   *            削除ノードの右の子が次節点の場合
   *      q                       q
   *      |                       |
   *      z          -->          r
   *     / \                     / \
   *    l   r                   l   1
   *         \
   *          1
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;
  rb_tree.insert_unique(pair_type(2, 2));
  rb_tree.insert_unique(pair_type(1, 1));
  rb_tree.insert_unique(pair_type(3, 3));
  rb_tree.insert_unique(pair_type(4, 4));

  rb_tree.delete_key_node(2);

  EXPECT_EQ(rb_tree[1].second, 1);
  EXPECT_EQ(rb_tree[3].second, 3);
  EXPECT_EQ(rb_tree[4].second, 4);
}

TEST(delete_key_node, TargetNodeHasTwoChildAndNextNodeIsInRightSubtree) {
  /* パターン4: 削除ノードが2つの子を持ち,
   *            次節点が削除ノードの右の部分木内にある場合
   *            (ただし削除ノードの右の子は次節点ではない)
   *      q                                                 q
   *      |                                                 |
   *      z          -- 2の場所を3で置き換える -->          z
   *     / \           (次節点は左の子を持たない)          / \
   *    l   r                                             l   r     2
   *       /                                                 /
   *      2                                                 3
   *       \
   *        3
   *
   *                                      q
   *                                      |
   *  -- zの部分を2に置き換える -->       2
   *                                     / \
   *                                    l   r
   *                                       /
   *                                      3
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;
  rb_tree.insert_unique(pair_type(1, 1));
  rb_tree.insert_unique(pair_type(5, 5));
  rb_tree.insert_unique(pair_type(0, 0));
  rb_tree.insert_unique(pair_type(3, 3));
  rb_tree.insert_unique(pair_type(10, 10));
  rb_tree.insert_unique(pair_type(6, 6));
  rb_tree.insert_unique(pair_type(7, 7));

  rb_tree.delete_key_node(1);

  EXPECT_EQ(rb_tree[5].second, 5);
  EXPECT_EQ(rb_tree[0].second, 0);
  EXPECT_EQ(rb_tree[3].second, 3);
  EXPECT_EQ(rb_tree[10].second, 10);
  EXPECT_EQ(rb_tree[6].second, 6);
  EXPECT_EQ(rb_tree[7].second, 7);
}

TEST(DeleteLeft, TargetNodesBrotherIsRed) {
  /* 修正パターン1: 兄弟が赤
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
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*               10B
   *          /             \
   *         5B             20R
   *           \         /        \
   *           7B      15B        25B
   *                   /  \      /   \
   *                 12B  17B  22B   30B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 10), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 5), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 7), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(15, 15), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(12, 12), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(17, 17), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(22, 22), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::BLACK);

  rb_tree.delete_key_node(5);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteLeft, TargetNodeAndBrotherAreBlack) {
  /* 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
   *       p_B                                   p_B
   *      /   \                                 /   \
   *    x_B   b_B      -- 兄弟を赤にする -->  x_B   b_R
   *         /   \                                 /   \
   *       l_B   r_B                             l_B   r_B
   *
   *    -- xを親に設定し, ループを継続 -->
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*               5B
   *          /             \
   *         3B             7B
   *           \         /        \
   *           4B      6B        8B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 5), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(3, 3), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(4, 4), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 7), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(6, 6), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(8, 8), node_type::BLACK);

  rb_tree.delete_key_node(3);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteLeft,
     TargetNodesdBrotherIsBlackAndBrothersLeftChildIsRedAndRightOneIsBlack) {
  /* 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
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
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*               10B
   *          /             \
   *         5B             20B
   *           \         /        \
   *           7B      15R        25B
   *                 /      \
   *               12B      18B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 10), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 5), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 7), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(15, 15), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(12, 12), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(18, 18), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::BLACK);

  rb_tree.delete_key_node(5);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteLeft, TargetNodesdBrotherIsBlackAndBrothersRightChildIsRed) {
  /* 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤 (左の子は赤でも黒でもいい)
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
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*
   *         10B
   *      /           \
   *     5B           15B
   *       \       /         \
   *       7B    13B          30R
   *      /  \         /              \
   *     6R   8R      20B             40B
   *                /    \           /    \
   *              18R    25R       35R    45R
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 10), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(5, 5), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 7), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(6, 6), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(8, 8), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(15, 15), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(13, 13), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(18, 18), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(40, 40), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(35, 35), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(45, 45), node_type::RED);

  rb_tree.delete_key_node(5);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteRight, TargetNodesBrotherIsRed) {
  /* 修正パターン1: 兄弟が赤
   *       p_B                                                p_R
   *      /   \                                              /   \
   *    b_R   x_B        -- 親と兄弟の色を変える -->       b_B   x_B
   *   /   \                                              /   \
   * l_B   r_B                                          l_B   r_B
   *
   *                              b_B
   *                             /   \
   *    -- 親を右回転 -->      l_B   p_R
   *                                /   \
   *                              r_B   x_B
   *
   *    -- 修正パターン2,3,4で修正する -->
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*
   *                50B
   *           /           \
   *          25R          75B
   *       /       \         \
   *      20B      30B       80B
   *     /   \    /   \
   *   15B   22B 27B  35B
   *
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(50, 50), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(75, 75), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(80, 80), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(15, 15), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(22, 22), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(27, 27), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(35, 35), node_type::BLACK);

  rb_tree.delete_key_node(75);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteRight, TargetNodeAndBrotherAreBlack) {
  /* 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
   *       p_B                                   p_B
   *      /   \                                 /   \
   *    b_B   x_B   -- 兄弟を赤にする -->     b_R   x_B
   *   /   \                                 /   \
   * l_B   r_B                             l_B   r_B
   *
   *    -- xを親に設定し, ループを継続 -->
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*
   *               50B
   *            /        \
   *           25B       75B
   *          /   \      /
   *        10B   30B  80B
   *
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(50, 50), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(75, 75), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(80, 80), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 10), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::BLACK);

  rb_tree.delete_key_node(75);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteRight,
     TargetNodesdBrotherIsBlackAndBrothersLeftChildIsRedAndRightOneIsBlack) {
  /* 修正パターン3: 兄弟が黒 + 兄弟の右の子が赤, 左の子が黒
   *       p_B                                                           p_B
   *      /   \                                                         /   \
   *    b_B   x_B      -- 兄弟と兄弟の右の子で色を交換する -->        b_R   x_B
   *   /   \                                                         /   \
   * l_B   r_R                                                     l_B   r_B
   *
   *          p_B                              p_B
   *         /   \                            /   \
   *       b_R   x_B   -- 兄弟を左回転 -->  r_B   x_B
   *      /   \                             /
   *    l_B   r_B                         b_R
   *                                      /
   *                                    l_B
   *
   *    -- 修正パターン4で修正する -->
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*
   *               50B
   *           /           \
   *          25B          60B
   *        /     \        /
   *      20B     30R    55B
   *              /  \
   *            27B  40B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(50, 50), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(60, 60), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(55, 55), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(27, 27), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(40, 40), node_type::BLACK);

  rb_tree.delete_key_node(60);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(DeleteRight, TargetNodesdBrotherIsBlackAndBrothersRightChildIsRed) {
  /* 修正パターン4: 兄弟が黒 + 兄弟の左の子が赤 (右の子は赤でも黒でもいい)
   *       p_R                                                       p_B
   *      /   \                                                     /   \
   *     b_B  x_B  -- 親と兄弟の色を交換 & 兄弟の左を黒に -->     b_R   x_B
   *    /   \                                                    /   \
   *  l_R   r_B                                                l_B   r_B
   *
   *                             b_R
   *                            /   \
   *    -- 親を右回転 -->     l_B   p_B
   *                               /   \
   *                             r_B   x_B
   */
  typedef int key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::node_type node_type;

  /*
   *                        50B
   *                   /            \
   *                 25B             60B
   *             /        \        /
   *           20R        30B     55B
   *        /       \             /   \
   *      10B       22B         52R   57R
   *     /   \     /   \
   *    7R   12R 21R   23R
   *
   */
  tree_type rb_tree;
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(50, 50), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(60, 60), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(55, 55), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(52, 52), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(57, 57), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(25, 25), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(30, 30), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(20, 20), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(10, 10), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(22, 22), node_type::BLACK);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(7, 7), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(12, 12), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(21, 21), node_type::RED);
  insertNodeWithoutFixup<key_type, pair_type, ft::Select1st<pair_type> >(
      &rb_tree.root_, rb_tree.nil_node_, pair_type(23, 23), node_type::RED);

  rb_tree.delete_key_node(60);
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(SizeAndCapacity, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));
  EXPECT_EQ(rb_tree.empty(), true);
  EXPECT_EQ(rb_tree.max_size(), tree_type::node_allocator().max_size());

  rb_tree.insert_unique(pair_type("A", 1));
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(1));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.insert_unique(pair_type("AB", 2));
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(2));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.insert_unique(pair_type("AA", 3));
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(3));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.insert_unique(pair_type("CC", 4));
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(4));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.insert_unique(pair_type("DD", 5));
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(5));
  EXPECT_EQ(rb_tree.empty(), false);

  rb_tree.delete_key_node("A");
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(4));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.delete_key_node("AB");
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(3));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.delete_key_node("AA");
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(2));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.delete_key_node("CC");
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(1));
  EXPECT_EQ(rb_tree.empty(), false);
  rb_tree.delete_key_node("DD");
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));
  EXPECT_EQ(rb_tree.empty(), true);
}

TEST(ForwardIterator, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::iterator tree_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_iterator it = rb_tree.begin();

  // check value that is pointed by iterator
  EXPECT_EQ(*it, pair_type("A", 1));
  ++it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  it++;
  EXPECT_EQ(*it++, pair_type("AB", 2));
  EXPECT_EQ(*it, pair_type("CC", 4));
  EXPECT_EQ(*(++it), pair_type("DD", 5));
  it++;
  EXPECT_EQ(it, rb_tree.end());
}

TEST(ForwardIterator, IteratorCanIncrementAndDecrement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::iterator tree_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_iterator it = rb_tree.begin();

  // check value that is pointed by iterator
  EXPECT_EQ(*it, pair_type("A", 1));
  ++it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  ++it;
  EXPECT_EQ(*it, pair_type("AB", 2));
  ++it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  ++it;
  EXPECT_EQ(*it, pair_type("DD", 5));
  ++it;
  EXPECT_EQ(it, rb_tree.end());
  --it;
  EXPECT_EQ(*it, pair_type("DD", 5));
  --it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  --it;
  EXPECT_EQ(*it, pair_type("AB", 2));
  --it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  --it;
  EXPECT_EQ(*it, pair_type("A", 1));
}

TEST(ForwardIterator, DeleteNodeDuringIteration) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::iterator tree_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_iterator it = rb_tree.begin();

  // check value that is pointed by iterator
  EXPECT_EQ(*it, pair_type("A", 1));
  ++it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  rb_tree.delete_key_node("AB");
  it++;
  EXPECT_EQ(*it, pair_type("CC", 4));
  EXPECT_EQ(*(++it), pair_type("DD", 5));
  it++;
  EXPECT_EQ(it, rb_tree.end());
}

TEST(ForwardIterator, TreeHasNoElement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  EXPECT_EQ(rb_tree.begin(), rb_tree.end());
}

TEST(ReverseIterator, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::reverse_iterator tree_reverse_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_reverse_iterator it = rb_tree.rbegin();

  // check value that is pointed by reverse_iterator
  EXPECT_EQ(*it, pair_type("DD", 5));
  ++it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  it++;
  EXPECT_EQ(*it++, pair_type("AB", 2));
  EXPECT_EQ(*it, pair_type("AA", 3));
  EXPECT_EQ(*(++it), pair_type("A", 1));
  it++;
  EXPECT_EQ(it, rb_tree.rend());
}

TEST(ReverseIterator, IteratorCanIncrementAndDecrement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::reverse_iterator tree_reverse_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_reverse_iterator it = rb_tree.rbegin();

  // check value that is pointed by reverse_iterator
  EXPECT_EQ(*it, pair_type("DD", 5));
  ++it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  ++it;
  EXPECT_EQ(*it, pair_type("AB", 2));
  ++it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  ++it;
  EXPECT_EQ(*it, pair_type("A", 1));
  ++it;
  EXPECT_EQ(it, rb_tree.rend());
  --it;
  EXPECT_EQ(*it, pair_type("A", 1));
  --it;
  EXPECT_EQ(*it, pair_type("AA", 3));
  --it;
  EXPECT_EQ(*it, pair_type("AB", 2));
  --it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  --it;
  EXPECT_EQ(*it, pair_type("DD", 5));
}

TEST(ReverseIterator, DeleteNodeDuringIteration) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef typename tree_type::reverse_iterator tree_reverse_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  tree_reverse_iterator it = rb_tree.rbegin();

  // check value that is pointed by reverse_iterator
  EXPECT_EQ(*it, pair_type("DD", 5));
  ++it;
  EXPECT_EQ(*it, pair_type("CC", 4));
  rb_tree.delete_key_node("AB");
  it++;
  EXPECT_EQ(*it, pair_type("AA", 3));
  EXPECT_EQ(*(++it), pair_type("A", 1));
  it++;
  EXPECT_EQ(it, rb_tree.rend());
}

TEST(ReverseIterator, TreeHasNoElement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  EXPECT_EQ(rb_tree.rbegin(), rb_tree.rend());
}

TEST(Clear, TreeHasNoNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));

  rb_tree.clear();

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));
}

TEST(Clear, TreeHasOneNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(1));

  rb_tree.clear();

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));
}

TEST(Clear, TreeHasManyNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));
  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  EXPECT_EQ(rb_tree.size(), tree_type::size_type(5));

  tree_iterator it = rb_tree.end();

  rb_tree.clear();

  // After call clear(), size() returns zero.
  EXPECT_EQ(rb_tree.size(), tree_type::size_type(0));
  // Any past-the-end iterator remains valid.
  EXPECT_EQ(it, rb_tree.end());
}

TEST(Clear, InsertNewValueAfterClearCall) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;

  tree_type rb_tree;

  rb_tree.insert_unique(pair_type("A", 1));
  rb_tree.insert_unique(pair_type("AB", 2));
  rb_tree.insert_unique(pair_type("AA", 3));

  rb_tree.clear();

  rb_tree.insert_unique(pair_type("CC", 4));
  rb_tree.insert_unique(pair_type("DD", 5));

  // begin_node_ は新しい要素を指しているはずである。
  EXPECT_EQ(*(rb_tree.begin()), pair_type("CC", 4));
}

TEST(insert_unique, ReturnIteratorAndWhetherInsertIsSuccess) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;

  ft::pair<tree_iterator, bool> result;
  // 挿入対象のキーがまだ存在しない場合はその値がそのまま挿入される
  result = rb_tree.insert_unique(pair_type("A", 1));
  EXPECT_EQ(*(result.first), pair_type("A", 1));
  EXPECT_EQ(result.second, true);
  result = rb_tree.insert_unique(pair_type("AB", 2));
  EXPECT_EQ(*(result.first), pair_type("AB", 2));
  EXPECT_EQ(result.second, true);
  result = rb_tree.insert_unique(pair_type("AA", 3));
  EXPECT_EQ(*(result.first), pair_type("AA", 3));
  EXPECT_EQ(result.second, true);
  result = rb_tree.insert_unique(pair_type("CC", 4));
  EXPECT_EQ(*(result.first), pair_type("CC", 4));
  EXPECT_EQ(result.second, true);
  // 挿入対象のキーが既にある場合は何も行わない(値は更新されない)
  result = rb_tree.insert_unique(pair_type("CC", 5));
  EXPECT_EQ(*(result.first), pair_type("CC", 4));
  EXPECT_EQ(result.second, false);
}

TEST(InsertWithHint, NewValueNextToTheHint) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値はヒントの次に入る。
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("F", 0));
  EXPECT_EQ(*it, pair_type("F", 0));

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(InsertWithHint, NewValuePreviousToTheHint) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値はヒントの前に入る。
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("D", 0));
  EXPECT_EQ(*it, pair_type("D", 0));

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(InsertWithHint, NewValueNextNextToTheHint) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値はヒントの次の次に入る。
  // この場合は普通にルートから辿る
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("H", 0));
  EXPECT_EQ(*it, pair_type("H", 0));

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(InsertWithHint, NewValuePreviousPreviousToTheHint) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値はヒントの前の前に入る。
  // この場合は普通にルートから辿る
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("B", 0));
  EXPECT_EQ(*it, pair_type("B", 0));

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(InsertWithHint, NewValueIsTheSameAsTheHint) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値がヒントと同じキー
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("E", 2));
  EXPECT_EQ(*it, pair_type("E", 0));

  expectRedBlackTreeKeepRules(rb_tree);

  // 値は更新されない
  EXPECT_EQ(rb_tree.search_key_node("E")->value_.second, 0);
}

TEST(InsertWithHint, NewValueIsTheSameAsTheNext) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値がヒントの次と同じキー
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("G", 2));
  EXPECT_EQ(*it, pair_type("G", 0));

  expectRedBlackTreeKeepRules(rb_tree);

  // 値は更新されない
  EXPECT_EQ(rb_tree.search_key_node("G")->value_.second, 0);
}

TEST(InsertWithHint, NewValueIsTheSameAsThePrevious) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  result = rb_tree.insert_unique(pair_type("E", 0));
  hint_it = result.first;
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // 新しい値がヒントの前と同じキー
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("C", 2));
  EXPECT_EQ(*it, pair_type("C", 0));

  expectRedBlackTreeKeepRules(rb_tree);

  // 値は更新されない
  EXPECT_EQ(rb_tree.search_key_node("C")->value_.second, 0);
}

TEST(InsertWithHint, TheHintIsTheBeginNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  rb_tree.insert_unique(pair_type("E", 0));
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // ヒントがbegin()だった
  hint_it = rb_tree.begin();
  // "0" < "A"
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("0", 0));
  EXPECT_EQ(*it, pair_type("0", 0));

  expectRedBlackTreeKeepRules(rb_tree);

  EXPECT_EQ((*(rb_tree.begin())).first, "0");
}

TEST(InsertWithHint, TheHintIsPreviousToTheEndNode) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  ft::pair<tree_iterator, bool> result;
  tree_iterator hint_it;

  /*     C
   *   /   \
   *  A     G
   *      /    \
   *     E      I
   */
  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("C", 0));
  rb_tree.insert_unique(pair_type("E", 0));
  rb_tree.insert_unique(pair_type("G", 0));
  rb_tree.insert_unique(pair_type("I", 0));

  // ヒントがend() - 1だった
  hint_it = rb_tree.end();
  --hint_it;
  tree_iterator it = rb_tree.insert_unique(hint_it, pair_type("J", 0));
  EXPECT_EQ(*it, pair_type("J", 0));

  expectRedBlackTreeKeepRules(rb_tree);

  // end() - 1 は新しく挿入したノードになっている
  it = rb_tree.end();
  --it;
  EXPECT_EQ((*it).first, "J");
}

TEST(InsertRange, TreeHasNoElement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef std::vector<pair_type> pair_vector;
  typedef pair_vector::iterator vector_iterator;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  pair_vector vec;

  vec.push_back(pair_type("A", 0));
  vec.push_back(pair_type("C", 0));
  vec.push_back(pair_type("E", 0));
  vec.push_back(pair_type("G", 0));
  vec.push_back(pair_type("I", 0));

  rb_tree.insert_range(vec.begin(), vec.end());

  tree_iterator tree_it = rb_tree.begin();
  vector_iterator vec_it = vec.begin();
  for (; tree_it != rb_tree.end() && vec_it != vec.end(); ++tree_it, ++vec_it) {
    EXPECT_EQ(*tree_it, *vec_it);
  }

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(InsertRange, TreeHasElements) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<const key_type, mapped_type> pair_type;
  typedef ft::RedBlackTree<key_type, pair_type, ft::Select1st<pair_type> >
      tree_type;
  typedef std::vector<pair_type> pair_vector;
  typedef pair_vector::iterator vector_iterator;
  typedef tree_type::iterator tree_iterator;

  tree_type rb_tree;
  pair_vector vec;

  rb_tree.insert_unique(pair_type("A", 0));
  rb_tree.insert_unique(pair_type("B", 0));
  rb_tree.insert_unique(pair_type("D", 0));
  rb_tree.insert_unique(pair_type("F", 0));
  rb_tree.insert_unique(pair_type("H", 0));
  rb_tree.insert_unique(pair_type("J", 0));

  // Aは2回insertされる。rb_treeに既にAがあるので pair("A", 2) は反映されない
  vec.push_back(pair_type("A", 2));
  vec.push_back(pair_type("C", 0));
  vec.push_back(pair_type("E", 0));
  vec.push_back(pair_type("G", 0));
  vec.push_back(pair_type("I", 0));

  rb_tree.insert_range(vec.begin(), vec.end());

  // rb_tree には A~J までが入っているはずである
  pair_vector expected_vec;
  for (char c = 'A'; c <= 'J'; ++c) {
    expected_vec.push_back(pair_type(std::string(1, c), 0));
  }

  tree_iterator tree_it = rb_tree.begin();
  vector_iterator vec_it = expected_vec.begin();
  for (; tree_it != rb_tree.end() && vec_it != expected_vec.end();
       ++tree_it, ++vec_it) {
    EXPECT_EQ((*tree_it).first, (*vec_it).first);
    EXPECT_EQ((*tree_it).second, 0);
  }

  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(Erase, NeedToThinkAboutWhatTestDo) {}

TEST(Swap, NeedToThinkAboutWhatTestDo) {}

TEST(Count, NeedToThinkAboutWhatTestDo) {}

TEST(Find, NeedToThinkAboutWhatTestDo) {}

TEST(LowerBound, NeedToThinkAboutWhatTestDo) {}

TEST(UpperBound, NeedToThinkAboutWhatTestDo) {}

TEST(EqualRange, NeedToThinkAboutWhatTestDo) {}