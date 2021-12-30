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
  rb_tree.Insert("b", 1);
  rb_tree.Insert("a", 1);
  std::cout << "Height: " << rb_tree.GetHeight() << std::endl;
  rb_tree.PrintTree2D();
}