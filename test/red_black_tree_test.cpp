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

TEST(InsertFixup, UncleIsRedLeft) {
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
  node_type *new_node = insertNodeWithoutFixup(
      &rb_tree.root_, rb_tree.nil_node_, 3, 0, node_type::RED);

  rb_tree.InsertFixup(new_node);

  EXPECT_EQ(rb_tree.root_->key_, 10);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *left_subtree = rb_tree.root_->left_;
  EXPECT_EQ(left_subtree->key_, 5);
  EXPECT_EQ(left_subtree->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(left_subtree->right_->key_, 7);
  EXPECT_EQ(left_subtree->right_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->key_, 4);
  EXPECT_EQ(left_subtree->left_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(left_subtree->left_->left_->key_, 3);
  EXPECT_EQ(left_subtree->left_->left_->color_, tree_type::RBTNode::RED);
}

TEST(InsertFixup, UncleIsRedRight) {
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
  node_type *new_node = insertNodeWithoutFixup(
      &rb_tree.root_, rb_tree.nil_node_, 15, 0, node_type::RED);

  rb_tree.InsertFixup(new_node);

  EXPECT_EQ(rb_tree.root_->key_, 5);
  EXPECT_EQ(rb_tree.root_->color_, tree_type::RBTNode::BLACK);  // 根は黒
  node_type *right_subtree = rb_tree.root_->right_;
  EXPECT_EQ(right_subtree->key_, 10);
  EXPECT_EQ(right_subtree->color_, tree_type::RBTNode::RED);
  EXPECT_EQ(right_subtree->left_->key_, 7);
  EXPECT_EQ(right_subtree->left_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->key_, 12);
  EXPECT_EQ(right_subtree->right_->color_, tree_type::RBTNode::BLACK);
  EXPECT_EQ(right_subtree->right_->right_->key_, 15);
  EXPECT_EQ(right_subtree->right_->right_->color_, tree_type::RBTNode::RED);
}