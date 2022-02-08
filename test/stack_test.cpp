#include "stack.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>

#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif
#include "vector.hpp"

namespace {

TEST(Stack, InitializeIntStackAndBasicOperations) {
  ft::stack<int> int_stack;

  int_stack.push(1);
  int_stack.push(2);

  EXPECT_EQ(int_stack.size(), 2ul);
  EXPECT_EQ(int_stack.top(), 2);
  int_stack.pop();
  EXPECT_EQ(int_stack.size(), 1ul);
  EXPECT_EQ(int_stack.top(), 1);
  int_stack.pop();
  EXPECT_EQ(int_stack.size(), 0ul);
  EXPECT_TRUE(int_stack.empty());
}

TEST(Stack, InitializeWithContainer) {
  ft::vector<int> int_vec;
  for (int i = 0; i < 10; ++i) int_vec.push_back(i);

  ft::stack<int> int_stack(int_vec);
  for (int i = 9; i >= 0; --i) {
    EXPECT_EQ(int_stack.top(), int_vec[i]);
    int_stack.pop();
  }
}

TEST(Stack, CopyConstructor) {
  ft::stack<int> vec1;
  vec1.push(1);
  ft::stack<int> vec2(vec1);
  vec2.push(2);

  EXPECT_EQ(vec1.size(), 1ul);
  EXPECT_EQ(vec1.top(), 1);
  EXPECT_EQ(vec2.size(), 2ul);
  EXPECT_EQ(vec2.top(), 2);
}

TEST(Stack, AssignmentOperator) {
  ft::stack<int> vec1;
  vec1.push(1);
  ft::stack<int> vec2;
  vec2 = vec1;
  vec2.push(2);

  EXPECT_EQ(vec1.size(), 1ul);
  EXPECT_EQ(vec1.top(), 1);
  EXPECT_EQ(vec2.size(), 2ul);
  EXPECT_EQ(vec2.top(), 2);
}

TEST(Stack, Empty) {
  ft::stack<int> stack1;

  EXPECT_TRUE(stack1.empty());
  stack1.push(1);
  EXPECT_FALSE(stack1.empty());
}

TEST(Stack, Size) {
  ft::stack<int> stack1;

  for (ft::stack<int>::size_type i = 0; i < 10; ++i) {
    EXPECT_EQ(stack1.size(), i);
    stack1.push(i);
  }
}

TEST(Stack, Top) {
  ft::stack<int> stack1;

  for (int i = 0; i < 10; ++i) {
    stack1.push(i);
    EXPECT_EQ(stack1.top(), i);
  }
}

TEST(Stack, Push) {
  ft::stack<int> stack1;

  stack1.push(1);
  EXPECT_EQ(stack1.top(), 1);
  stack1.push(2);
  EXPECT_EQ(stack1.top(), 2);
}

TEST(Stack, Pop) {
  ft::stack<int> stack1;

  for (int i = 0; i < 10; ++i) {
    stack1.push(i);
  }

  for (int i = 9; i >= 0; --i) {
    EXPECT_EQ(stack1.top(), i);
    stack1.pop();
  }
  EXPECT_TRUE(stack1.empty());
}

TEST(Stack, OperatorEqual) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_TRUE(stack1 == stack2);
  stack1.push(1);
  EXPECT_FALSE(stack1 == stack2);
  stack2.push(1);
  EXPECT_TRUE(stack1 == stack2);
}

TEST(Stack, OperatorNotEqual) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_FALSE(stack1 != stack2);
  stack1.push(1);
  EXPECT_TRUE(stack1 != stack2);
  stack2.push(1);
  EXPECT_FALSE(stack1 != stack2);
}

TEST(Stack, OperatorLessThan) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_FALSE(stack1 < stack2);
  stack2.push(2);
  EXPECT_TRUE(stack1 < stack2);
  stack1.push(1);
  EXPECT_TRUE(stack1 < stack2);
}

TEST(Stack, OperatorLessThanOrEqual) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_TRUE(stack1 <= stack2);
  stack1.push(1);
  EXPECT_FALSE(stack1 <= stack2);
  stack2.push(2);
  EXPECT_TRUE(stack1 <= stack2);
}

TEST(Stack, OperatorGreaterThan) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_FALSE(stack1 > stack2);
  stack1.push(2);
  EXPECT_TRUE(stack1 > stack2);
  stack2.push(1);
  EXPECT_TRUE(stack1 > stack2);
}

TEST(Stack, OperatorGreaterThanOrEqual) {
  ft::stack<int> stack1;
  ft::stack<int> stack2;

  EXPECT_TRUE(stack1 >= stack2);
  stack2.push(1);
  EXPECT_FALSE(stack1 >= stack2);
  stack1.push(2);
  EXPECT_TRUE(stack1 >= stack2);
}

}  // namespace