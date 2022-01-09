#include "red_black_tree.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace {
// テスト用の関数. 色の修正や木の回転などを行わない挿入.
template <class Key, class Value>
typename ft::RedBlackTree<Key, Value>::RBTNode *insertNodeWithoutFixup(
    typename ft::RedBlackTree<Key, Value>::RBTNode **root,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node,
    typename ft::RedBlackTree<Key, Value>::RBTNode *new_node) {
  typedef typename ft::RedBlackTree<Key, Value>::RBTNode RBTNode;

  if (*root == nil_node) {
    *root = new_node;
    new_node->parent_ = nil_node;
    new_node->left_ = nil_node;
    new_node->right_ = nil_node;
    return new_node;
  }

  RBTNode *parent = nil_node;
  RBTNode *node = *root;
  while (node != nil_node) {
    if (new_node->key_ == node->key_) {
      exit(1);
    } else if (new_node->key_ < node->key_) {
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
  if (new_node->key_ < parent->key_) {
    parent->left_ = new_node;
  } else {
    parent->right_ = new_node;
  }
  return new_node;
}

template <class Key, class Value>
typename ft::RedBlackTree<Key, Value>::RBTNode *insertNodeWithoutFixup(
    typename ft::RedBlackTree<Key, Value>::RBTNode **root,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node, Key key,
    Value value, typename ft::RedBlackTree<Key, Value>::RBTNode::Color color) {
  typedef typename ft::RedBlackTree<Key, Value>::RBTNode node_type;
  node_type *new_node = new node_type(key, value);
  new_node->color_ = color;
  insertNodeWithoutFixup<Key, Value>(root, nil_node, new_node);
  return new_node;
}

template <class Key, class Value>
void expectAllRedNodeHasTwoBlackChild(
    typename ft::RedBlackTree<Key, Value>::RBTNode *node,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node);

template <class Key, class Value>
int expectAllPathesAreSameBlackNodeCount(
    typename ft::RedBlackTree<Key, Value>::RBTNode *node,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node,
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
template <class Key, class Value>
void expectRedBlackTreeKeepRules(
    typename ft::RedBlackTree<Key, Value> &rb_tree) {
  typedef typename ft::RedBlackTree<Key, Value>::RBTNode node_type;

  // 2. 根は黒である. (たまにこの条件は省かれる)
  EXPECT_EQ(rb_tree.root_->color_, node_type::BLACK);

  // 3. 葉(NIL)は全て黒である. 葉は全て根と同じ色である.
  EXPECT_EQ(rb_tree.nil_node_->color_, node_type::BLACK);
  EXPECT_EQ(rb_tree.root_->color_, rb_tree.nil_node_->color_);

  // 4. 赤のノードは黒ノードを2つ子に持つ.
  expectAllRedNodeHasTwoBlackChild<Key, Value>(rb_tree.root_,
                                               rb_tree.nil_node_);

  // 5. 任意のノードについて,
  //    そのノードから子孫の葉までの道に含まれる黒ノードの数は,
  //    選んだ葉に依らず一定である.
  //    (この条件は,
  //    「根から葉までの道に含まれる黒いノードの数は、葉によらず一定である」
  //    と言い換えることができる)
  expectAllPathesAreSameBlackNodeCount<Key, Value>(rb_tree.root_,
                                                   rb_tree.nil_node_);
}

template <class Key, class Value>
void expectAllRedNodeHasTwoBlackChild(
    typename ft::RedBlackTree<Key, Value>::RBTNode *node,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node) {
  typedef typename ft::RedBlackTree<Key, Value>::RBTNode node_type;

  if (node == nil_node) {
    return;
  }
  if (node->color_ == node_type::RED) {
    EXPECT_EQ(node->left_->color_, node_type::BLACK);
    EXPECT_EQ(node->right_->color_, node_type::BLACK);
  }
  expectAllRedNodeHasTwoBlackChild<Key, Value>(node->left_, nil_node);
  expectAllRedNodeHasTwoBlackChild<Key, Value>(node->right_, nil_node);
}

template <class Key, class Value>
int expectAllPathesAreSameBlackNodeCount(
    typename ft::RedBlackTree<Key, Value>::RBTNode *node,
    typename ft::RedBlackTree<Key, Value>::RBTNode *nil_node, int black_count) {
  typedef typename ft::RedBlackTree<Key, Value>::RBTNode node_type;

  if (node == nil_node) {
    return black_count;
  }
  if (node->color_ == node_type::BLACK) {
    black_count++;
  }
  int left_count = expectAllPathesAreSameBlackNodeCount<Key, Value>(
      node->left_, nil_node, black_count);
  int right_count = expectAllPathesAreSameBlackNodeCount<Key, Value>(
      node->right_, nil_node, black_count);
  EXPECT_EQ(left_count, right_count);
  return left_count;
}

}  // namespace

TEST(RedBlackTree, BasicOperations) {
  ft::RedBlackTree<std::string, int> rb_tree;
  rb_tree.Insert("c", 1);
  rb_tree.Insert("b", 2);
  rb_tree.Insert("a", 3);

  std::cout << "Height: " << rb_tree.GetHeight() << std::endl;
  rb_tree.PrintTree2D();

  std::cout << "a: " << rb_tree["a"] << std::endl;
  std::cout << "b: " << rb_tree["b"] << std::endl;
  std::cout << "c: " << rb_tree["c"] << std::endl;

  EXPECT_EQ(rb_tree["c"], 1);
  EXPECT_EQ(rb_tree["b"], 2);
  EXPECT_EQ(rb_tree["a"], 3);

  rb_tree.Delete("a");
  rb_tree.Delete("b");
  std::cout << "after delete a and b" << std::endl;
  std::cout << "Height: " << rb_tree.GetHeight() << std::endl;
  rb_tree.PrintTree2D();
}

TEST(Modifier, ModifyExistNode) {
  ft::RedBlackTree<std::string, int> rb_tree;
  rb_tree.Insert("a", 1);
  EXPECT_EQ(rb_tree["a"], 1);
  rb_tree.Insert("a", 2);
  EXPECT_EQ(rb_tree["a"], 2);
  rb_tree.Delete("a");
}

TEST(Modifier, DeleteAndAccessItShouldThrowException) {
  ft::RedBlackTree<std::string, int> rb_tree;
  rb_tree.Insert("a", 1);
  EXPECT_EQ(rb_tree["a"], 1);
  rb_tree.Delete("a");
  EXPECT_THROW(rb_tree["a"], std::out_of_range);
}

TEST(TreeSuccessor, Random1000) {
  srand(time(NULL));
  typedef ft::RedBlackTree<int, int> tree_type;
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
    rb_tree.Insert(num, num);
  }

  const node_type *node = NULL;
  for (set_type::iterator it = s.begin(); it != s.end(); ++it) {
    node = rb_tree.TreeSuccessor(node);
    EXPECT_EQ(node->key_, *it);
    EXPECT_EQ(node->value_, *it);
  }
  node = rb_tree.TreeSuccessor(node);
  // 最後はNULLが返ってくる
  EXPECT_TRUE(node == NULL);
}

TEST(TreeSuccessor, DeleteNodeDuringIteration) {
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  for (int i = 0; i < 5; ++i) {
    rb_tree.Insert(i, i);
  }
  const node_type *node = rb_tree.TreeSuccessor(NULL);
  int i = 0;
  while (node) {
    EXPECT_EQ(node->value_, i);
    if (i == 1) {
      // 2をスキップする
      ++i;
      rb_tree.Delete(i);
    }
    node = rb_tree.TreeSuccessor(node);
    ++i;
  }
}

TEST(TreePredecessor, Random1000) {
  srand(time(NULL));
  typedef ft::RedBlackTree<int, int> tree_type;
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
    rb_tree.Insert(num, num);
  }

  const node_type *node = NULL;
  for (set_type::reverse_iterator rit = s.rbegin(); rit != s.rend(); ++rit) {
    node = rb_tree.TreePredecessor(node);
    EXPECT_EQ(node->key_, *rit);
    EXPECT_EQ(node->value_, *rit);
  }
  node = rb_tree.TreePredecessor(node);
  // 最後はNULLが返ってくる
  EXPECT_TRUE(node == NULL);
}

TEST(TreePredecessor, DeleteNodeDuringIteration) {
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef typename tree_type::node_type node_type;

  tree_type rb_tree;

  for (int i = 0; i < 5; ++i) {
    rb_tree.Insert(i, i);
  }
  const node_type *node = rb_tree.TreePredecessor(NULL);
  int i = 4;
  while (node) {
    EXPECT_EQ(node->value_, i);
    if (i == 2) {
      // 1をスキップする
      --i;
      rb_tree.Delete(i);
    }
    node = rb_tree.TreePredecessor(node);
    --i;
  }
}

TEST(RedBlackTree, CopyConstructor) {
  typedef ft::RedBlackTree<int, int> tree_type;

  srand(time(NULL));
  const int loop_num = 10;

  tree_type t1;

  for (int i = 0; i < loop_num; ++i) {
    t1.Insert(rand(), rand());
  }

  t1.Insert(0, 1);

  tree_type t2 = t1;
  tree_type t3(t1);
  tree_type t4;
  t4 = t1;

  // Deep Copy Check
  t2[0] = 2;
  t3[0] = 3;
  t4[0] = 4;
  EXPECT_EQ(t1[0], 1);
  EXPECT_EQ(t2[0], 2);
  EXPECT_EQ(t3[0], 3);
  EXPECT_EQ(t4[0], 4);

  t1.Delete(0);
  t2.Delete(0);
  t3.Delete(0);
  t4.Delete(0);

  const tree_type::node_type *n1 = t1.TreeSuccessor();
  const tree_type::node_type *n2 = t2.TreeSuccessor();
  const tree_type::node_type *n3 = t3.TreeSuccessor();
  const tree_type::node_type *n4 = t4.TreeSuccessor();
  while (n1) {
    EXPECT_EQ(n1->key_, n2->key_);
    EXPECT_EQ(n1->value_, n2->value_);
    EXPECT_NE(n1, n2);
    EXPECT_EQ(n1->key_, n3->key_);
    EXPECT_EQ(n1->value_, n3->value_);
    EXPECT_NE(n1, n3);
    EXPECT_EQ(n1->key_, n4->key_);
    EXPECT_EQ(n1->value_, n4->value_);
    EXPECT_NE(n1, n4);
    n1 = t1.TreeSuccessor(n1);
    n2 = t2.TreeSuccessor(n2);
    n3 = t3.TreeSuccessor(n3);
    n4 = t4.TreeSuccessor(n4);
  }
  // 最後はNULL
  EXPECT_EQ(n1, n2);
  EXPECT_EQ(n1, n3);
  EXPECT_EQ(n1, n4);
}

// Insert

TEST(Insert, Random1000) {
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;

  srand(time(NULL));

  for (int i = 0; i < 1000; ++i) {
    rb_tree.Insert(rand(), 0);
  }
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(Insert, Sorted1000) {
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;

  for (int i = 0; i < 1000; ++i) {
    rb_tree.Insert(i, 0);
  }
  expectRedBlackTreeKeepRules(rb_tree);
}

TEST(Insert, UncleIsRedLeft) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 4, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 10);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->key_, 5);
  ASSERT_EQ(left_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(left_subtree->right_->key_, 7);
  ASSERT_EQ(left_subtree->right_->color_, tree_type::RBTNode::RED);
  ASSERT_EQ(left_subtree->left_->key_, 4);
  ASSERT_EQ(left_subtree->left_->color_, tree_type::RBTNode::RED);

  rb_tree.Insert(3, 0);

  EXPECT_EQ(rb_tree.root_->key_, 10);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->key_, 5);
  EXPECT_EQ(left_subtree->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->key_, 7);
  EXPECT_EQ(left_subtree->right_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->key_, 4);
  EXPECT_EQ(left_subtree->left_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->left_->key_, 3);
  EXPECT_EQ(left_subtree->left_->left_->color_, tree_type::RBTNode::RED);
}

TEST(Insert, UncleIsRedRight) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 12, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 5);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->key_, 10);
  ASSERT_EQ(right_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(right_subtree->left_->key_, 7);
  ASSERT_EQ(right_subtree->left_->color_, tree_type::RBTNode::RED);
  ASSERT_EQ(right_subtree->right_->key_, 12);
  ASSERT_EQ(right_subtree->right_->color_, tree_type::RBTNode::RED);

  rb_tree.Insert(15, 0);

  EXPECT_EQ(rb_tree.root_->key_, 5);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->key_, 10);
  EXPECT_EQ(right_subtree->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->key_, 7);
  EXPECT_EQ(right_subtree->left_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->key_, 12);
  EXPECT_EQ(right_subtree->right_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->right_->key_, 15);
  EXPECT_EQ(right_subtree->right_->right_->color_, tree_type::RBTNode::RED);
}

TEST(Insert, UncleIsBlackAndNewNodeIsLeftLeft) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 3, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 10);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->key_, 5);
  ASSERT_EQ(left_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(left_subtree->left_->key_, 3);
  ASSERT_EQ(left_subtree->left_->color_, tree_type::RBTNode::RED);
  ASSERT_EQ(left_subtree->right_->key_, 7);
  ASSERT_EQ(left_subtree->right_->color_, tree_type::RBTNode::BLACK);

  rb_tree.Insert(1, 0);

  EXPECT_EQ(rb_tree.root_->key_, 10);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->key_, 3);
  EXPECT_EQ(left_subtree->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->key_, 1);
  EXPECT_EQ(left_subtree->left_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->key_, 5);
  EXPECT_EQ(left_subtree->right_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->right_->key_, 7);
  EXPECT_EQ(left_subtree->right_->right_->color_, tree_type::RBTNode::BLACK);
}

TEST(Insert, UncleIsBlackAndNewNodeIsRightRight) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 1, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 3, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 1);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->key_, 5);
  ASSERT_EQ(right_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(right_subtree->left_->key_, 3);
  ASSERT_EQ(right_subtree->left_->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(right_subtree->right_->key_, 7);
  ASSERT_EQ(right_subtree->right_->color_, tree_type::RBTNode::RED);

  rb_tree.Insert(10, 0);

  EXPECT_EQ(rb_tree.root_->key_, 1);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->key_, 7);
  EXPECT_EQ(right_subtree->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->key_, 10);
  EXPECT_EQ(right_subtree->right_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->key_, 5);
  EXPECT_EQ(right_subtree->left_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->left_->key_, 3);
  EXPECT_EQ(right_subtree->left_->left_->color_, tree_type::RBTNode::BLACK);
}

TEST(Insert, UncleIsBlackAndNewNodeIsLeftRight) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 3, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 10);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  ASSERT_EQ(left_subtree->key_, 5);
  ASSERT_EQ(left_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(left_subtree->left_->key_, 3);
  ASSERT_EQ(left_subtree->left_->color_, tree_type::RBTNode::RED);
  ASSERT_EQ(left_subtree->right_->key_, 7);
  ASSERT_EQ(left_subtree->right_->color_, tree_type::RBTNode::BLACK);

  rb_tree.Insert(4, 0);

  EXPECT_EQ(rb_tree.root_->key_, 10);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->key_, 4);
  EXPECT_EQ(left_subtree->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->key_, 3);
  EXPECT_EQ(left_subtree->left_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->key_, 5);
  EXPECT_EQ(left_subtree->right_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->right_->key_, 7);
  EXPECT_EQ(left_subtree->right_->right_->color_, tree_type::RBTNode::BLACK);
}

TEST(Insert, UncleIsBlackAndNewNodeIsRightleft) {
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef ft::RedBlackTree<int, int>::RBTNode node_type;

  tree_type rb_tree;

  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 1, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 3, 0,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 0,
                         node_type::RED);

  // これがおかしいということはテストの入力がおかしいので, プログラムを終了
  ASSERT_EQ(rb_tree.root_->key_, 1);
  ASSERT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  ASSERT_EQ(right_subtree->key_, 5);
  ASSERT_EQ(right_subtree->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(right_subtree->left_->key_, 3);
  ASSERT_EQ(right_subtree->left_->color_, tree_type::RBTNode::BLACK);
  ASSERT_EQ(right_subtree->right_->key_, 7);
  ASSERT_EQ(right_subtree->right_->color_, tree_type::RBTNode::RED);

  rb_tree.Insert(6, 0);

  EXPECT_EQ(rb_tree.root_->key_, 1);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->key_, 6);
  EXPECT_EQ(right_subtree->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->key_, 7);
  EXPECT_EQ(right_subtree->right_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->key_, 5);
  EXPECT_EQ(right_subtree->left_->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->left_->key_, 3);
  EXPECT_EQ(right_subtree->left_->left_->color_, tree_type::RBTNode::BLACK);
}

TEST(Search, BasicOperations) {
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;

  for (int i = 0; i < 1000; ++i) {
    rb_tree.Insert(i, i);
  }
  for (int i = 0; i < 1000; ++i) {
    EXPECT_EQ(rb_tree[i], i);
  }
}

TEST(Delete, TargetNodeHasOnlyRightChild) {
  /* パターン1: 削除ノードが左の子を持たない場合
   *      q                     q
   *      |                     |
   *      z          -->        r
   *       \
   *        r
   */
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;

  rb_tree.Insert(5, 5);
  rb_tree.Insert(7, 7);
  rb_tree.Insert(9, 9);
  rb_tree.Insert(11, 11);

  rb_tree.Delete(9);

  EXPECT_EQ(rb_tree[5], 5);
  EXPECT_EQ(rb_tree[7], 7);
  EXPECT_EQ(rb_tree[11], 11);
}

TEST(Delete, TargetNodeHasOnlyLeftChild) {
  /* パターン2: 削除ノードが右の子を持たない場合
   *      q                     q
   *      |                     |
   *      z          -->        l
   *     /
   *    l
   */
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;

  rb_tree.Insert(5, 5);
  rb_tree.Insert(4, 4);
  rb_tree.Insert(3, 3);
  rb_tree.Insert(2, 2);

  rb_tree.Delete(3);

  EXPECT_EQ(rb_tree[5], 5);
  EXPECT_EQ(rb_tree[4], 4);
  EXPECT_EQ(rb_tree[2], 2);
}

TEST(Delete, TargetNodeHasTwoChildAndRightChildIsNextNode) {
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
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;
  rb_tree.Insert(2, 2);
  rb_tree.Insert(1, 1);
  rb_tree.Insert(3, 3);
  rb_tree.Insert(4, 4);

  rb_tree.Delete(2);

  EXPECT_EQ(rb_tree[1], 1);
  EXPECT_EQ(rb_tree[3], 3);
  EXPECT_EQ(rb_tree[4], 4);
}

TEST(Delete, TargetNodeHasTwoChildAndNextNodeIsInRightSubtree) {
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
  typedef ft::RedBlackTree<int, int> tree_type;

  tree_type rb_tree;
  rb_tree.Insert(1, 1);
  rb_tree.Insert(5, 5);
  rb_tree.Insert(0, 0);
  rb_tree.Insert(3, 3);
  rb_tree.Insert(10, 10);
  rb_tree.Insert(6, 6);
  rb_tree.Insert(7, 7);

  rb_tree.Delete(1);

  EXPECT_EQ(rb_tree[5], 5);
  EXPECT_EQ(rb_tree[0], 0);
  EXPECT_EQ(rb_tree[3], 3);
  EXPECT_EQ(rb_tree[10], 10);
  EXPECT_EQ(rb_tree[6], 6);
  EXPECT_EQ(rb_tree[7], 7);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

  /*               10B
   *          /             \
   *         5B             20R
   *           \         /        \
   *           7B      15B        25B
   *                   /  \      /   \
   *                 12B  17B  22B   30B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 10,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 5,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 7,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 15, 15,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 12, 12,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 17, 17,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 22, 22,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::BLACK);

  rb_tree.Delete(5);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

  /*               5B
   *          /             \
   *         3B             7B
   *           \         /        \
   *           4B      6B        8B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 5,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 3, 3,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 4, 4,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 7,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 6, 6,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 8, 8,
                         node_type::BLACK);

  rb_tree.Delete(3);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

  /*               10B
   *          /             \
   *         5B             20B
   *           \         /        \
   *           7B      15R        25B
   *                 /      \
   *               12B      18B
   */
  tree_type rb_tree;
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 10,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 5,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 7,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 15, 15,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 12, 12,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 18, 18,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::BLACK);

  rb_tree.Delete(5);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

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
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 10,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 5, 5,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 7,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 6, 6,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 8, 8,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 15, 15,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 13, 13,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 18, 18,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 40, 40,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 35, 35,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 45, 45,
                         node_type::RED);

  rb_tree.Delete(5);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

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
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 50, 50,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 75, 75,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 80, 80,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 15, 15,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 22, 22,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 27, 27,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 35, 35,
                         node_type::BLACK);

  rb_tree.Delete(75);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

  /*
   *               50B
   *            /        \
   *           25B       75B
   *          /   \      /
   *        10B   30B  80B
   *
   */
  tree_type rb_tree;
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 50, 50,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 75, 75,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 80, 80,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 10,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::BLACK);

  rb_tree.Delete(75);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

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
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 50, 50,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 60, 60,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 55, 55,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 27, 27,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 40, 40,
                         node_type::BLACK);

  rb_tree.Delete(60);
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
  typedef ft::RedBlackTree<int, int> tree_type;
  typedef tree_type::RBTNode node_type;

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
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 50, 50,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 60, 60,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 55, 55,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 52, 52,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 57, 57,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 25, 25,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 30, 30,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 20, 20,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 10, 10,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 22, 22,
                         node_type::BLACK);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 7, 7,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 12, 12,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 21, 21,
                         node_type::RED);
  insertNodeWithoutFixup(&rb_tree.root_, rb_tree.nil_node_, 23, 23,
                         node_type::RED);

  rb_tree.Delete(60);
  expectRedBlackTreeKeepRules(rb_tree);
}