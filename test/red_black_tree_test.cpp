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