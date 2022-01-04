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

TEST(TreeSuccessor, Random100) {
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

TEST(TreePredecessor, Random100) {
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
  std::cout << "Deep Copy Check" << std::endl;
  t2[0] = 2;
  t3[0] = 3;
  t4[0] = 4;
  EXPECT_EQ(t1[0], 1);
  EXPECT_EQ(t2[0], 2);
  EXPECT_EQ(t3[0], 3);
  EXPECT_EQ(t4[0], 4);

  std::cout << "Delete Node 0" << std::endl;
  t1.Delete(0);
  t2.Delete(0);
  t3.Delete(0);
  t4.Delete(0);

  std::cout << "Start TreeSuccessor" << std::endl;
  const tree_type::node_type *n1 = t1.TreeSuccessor();
  const tree_type::node_type *n2 = t2.TreeSuccessor();
  const tree_type::node_type *n3 = t3.TreeSuccessor();
  const tree_type::node_type *n4 = t4.TreeSuccessor();
  while (n1) {
    std::cout << n1->key_ << std::endl;
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