#include "red_black_tree.hpp"

#include <gtest/gtest.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
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