#include "set.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

#include "pair.hpp"
#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif
#include "utils/Student.hpp"
#include "utils/comparison.hpp"
#include "utils/hash.hpp"
#include "utils/my_allocator.hpp"
#include "utils/string.hpp"

TEST(Set, DefaultConstructor) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s;

  EXPECT_EQ(s.empty(), true);
  EXPECT_EQ(s.size(), set_type::size_type(0));
}

TEST(Set, ConstructorWithCompareMethod) {
  typedef char key_type;
  typedef ft::test::less_or_greater<key_type> compare_type;
  typedef ft::set<key_type, compare_type> set_type;
  typedef set_type::iterator set_iterator;

  const compare_type lesscomp = compare_type(true);
  const compare_type greatercomp = compare_type(false);

  set_type less_set(lesscomp);
  set_type greater_set(greatercomp);

  for (char c = 'A'; c <= 'Z'; ++c) {
    std::string s(1, c);
    less_set.insert(c);
    greater_set.insert(c);
  }

  set_iterator it = less_set.begin();
  for (char c = 'A'; c <= 'Z' && it != less_set.end(); ++c, ++it) {
    EXPECT_EQ(*it, c);
  }
  EXPECT_EQ(it, less_set.end());

  it = greater_set.begin();
  for (char c = 'Z'; c >= 'A' && it != greater_set.end(); --c, ++it) {
    EXPECT_EQ(*it, c);
  }
  EXPECT_EQ(it, greater_set.end());
}

TEST(Set, ConstructorWithIterator) {
  typedef int key_type;
  typedef std::vector<key_type> vector_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  vector_type vec;
  for (int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }

  set_type s(vec.begin(), vec.end());

  set_iterator it = s.begin();
  for (int i = 0; i < 10; ++i, ++it) {
    EXPECT_EQ(*it, i);
  }
  EXPECT_EQ(it, s.end());
}

TEST(Set, CopyConstructorAndAssignation) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s1;

  s1.insert(0);
  s1.insert(1);
  s1.insert(2);

  set_type s2(s1);
  s2.insert(3);

  set_type s3;
  s3 = s2;
  s3.insert(4);

  EXPECT_EQ(s1.count(0), set_type::size_type(1));
  EXPECT_EQ(s1.count(1), set_type::size_type(1));
  EXPECT_EQ(s1.count(2), set_type::size_type(1));
  EXPECT_EQ(s1.count(3), set_type::size_type(0));
  EXPECT_EQ(s1.count(4), set_type::size_type(0));

  EXPECT_EQ(s2.count(0), set_type::size_type(1));
  EXPECT_EQ(s2.count(1), set_type::size_type(1));
  EXPECT_EQ(s2.count(2), set_type::size_type(1));
  EXPECT_EQ(s2.count(3), set_type::size_type(1));
  EXPECT_EQ(s2.count(4), set_type::size_type(0));

  EXPECT_EQ(s3.count(0), set_type::size_type(1));
  EXPECT_EQ(s3.count(1), set_type::size_type(1));
  EXPECT_EQ(s3.count(2), set_type::size_type(1));
  EXPECT_EQ(s3.count(3), set_type::size_type(1));
  EXPECT_EQ(s3.count(4), set_type::size_type(1));
}

TEST(Set, GetAllocator) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s;

  s.get_allocator();
}

TEST(Set, ForwardIterator) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef ft::set<key_type>::iterator set_iterator;
  typedef ft::set<key_type>::const_iterator const_set_iterator;

  set_type s1;
  EXPECT_EQ(s1.begin(), s1.end());

  s1.insert(0);
  s1.insert(1);
  s1.insert(2);
  s1.insert(3);

  set_iterator it = s1.begin();
  EXPECT_EQ(*it++, 0);
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it++, 3);
  EXPECT_EQ(it, s1.end());

  const set_type s2;
  EXPECT_EQ(s2.begin(), s2.end());

  const set_type s3(s1);
  const_set_iterator cit = s3.begin();
  EXPECT_EQ(*cit++, 0);
  EXPECT_EQ(*cit++, 1);
  EXPECT_EQ(*cit++, 2);
  EXPECT_EQ(*cit++, 3);
  EXPECT_EQ(cit, s3.end());
}

TEST(Set, ReverseIterator) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef ft::set<key_type>::reverse_iterator set_rev_iterator;
  typedef ft::set<key_type>::const_reverse_iterator const_set_rev_iterator;

  set_type s1;
  EXPECT_EQ(s1.rbegin(), s1.rend());

  s1.insert(0);
  s1.insert(1);
  s1.insert(2);
  s1.insert(3);

  set_rev_iterator it = s1.rbegin();
  EXPECT_EQ(*it++, 3);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 0);
  EXPECT_EQ(it, s1.rend());

  const set_type s2;
  EXPECT_EQ(s2.rbegin(), s2.rend());

  const set_type s3(s1);
  const_set_rev_iterator cit = s3.rbegin();
  EXPECT_EQ(*cit++, 3);
  EXPECT_EQ(*cit++, 2);
  EXPECT_EQ(*cit++, 1);
  EXPECT_EQ(*cit++, 0);
  EXPECT_EQ(cit, s3.rend());
}

TEST(Set, Capacity) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s;
  EXPECT_EQ(s.empty(), true);
  EXPECT_EQ(s.size(), set_type::size_type(0));
  EXPECT_TRUE(s.max_size() > 0);

  s.insert(0);
  EXPECT_EQ(s.empty(), false);
  EXPECT_EQ(s.size(), set_type::size_type(1));

  s.insert(1);
  EXPECT_EQ(s.empty(), false);
  EXPECT_EQ(s.size(), set_type::size_type(2));
}

TEST(Set, Clear) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s;

  EXPECT_EQ(s.empty(), true);
  EXPECT_EQ(s.size(), set_type::size_type(0));

  s.clear();
  EXPECT_EQ(s.empty(), true);
  EXPECT_EQ(s.size(), set_type::size_type(0));

  s.insert(0);
  EXPECT_EQ(s.empty(), false);
  EXPECT_EQ(s.size(), set_type::size_type(1));

  s.clear();
  EXPECT_EQ(s.empty(), true);
}

TEST(Set, Insert) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;
  typedef ft::pair<set_iterator, bool> result_type;

  set_type s;

  // 新しい要素を入れる
  EXPECT_EQ(s.count(0), set_type::size_type(0));
  result_type result = s.insert(0);
  EXPECT_EQ(*(result.first), 0);
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(s.count(0), set_type::size_type(1));

  // 既にある要素を入れても何もおきない
  result = s.insert(0);
  EXPECT_EQ(*(result.first), 0);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(s.count(0), set_type::size_type(1));
}

TEST(Set, InsertWithHint) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;

  s.insert(1);
  s.insert(3);
  s.insert(5);

  set_iterator hint_it = s.find(1);

  // --hint に入る
  set_iterator it = s.insert(hint_it, 0);
  EXPECT_EQ(*it, 0);
  // hint と同じ要素だ
  it = s.insert(hint_it, 1);
  EXPECT_EQ(*it, 1);
  // ++hint に入る
  it = s.insert(hint_it, 2);
  EXPECT_EQ(*it, 2);
  // hint の隣には入らない
  it = s.insert(hint_it, 10);
  EXPECT_EQ(*it, 10);

  EXPECT_EQ(s.size(), set_type::size_type(6));
}

TEST(Set, InsertWithIterator) {
  typedef int key_type;
  typedef std::vector<key_type> vector_type;
  typedef vector_type::iterator vector_iterator;
  typedef ft::set<key_type> set_type;

  vector_type vec;
  for (int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }

  set_type s;
  s.insert(vec.begin(), vec.end());
  for (vector_iterator it = vec.begin(); it != vec.end(); ++it) {
    EXPECT_EQ(s.count(*it), set_type::size_type(1));
  }
}

TEST(Set, EraseWithIteratorPosition) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;

  for (int i = 0; i < 10; ++i) {
    s.insert(i);
  }

  set_iterator it = s.find(1);
  s.erase(it);
  EXPECT_EQ(s.count(1), set_type::size_type(0));
}

TEST(Set, EraseWithIteratorRange) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;

  for (int i = 0; i < 10; ++i) {
    s.insert(i);
  }

  // 1-8 まで消す
  set_iterator first = s.find(1);
  set_iterator last = s.find(9);
  s.erase(first, last);

  EXPECT_EQ(s.size(), set_type::size_type(2));
  EXPECT_EQ(s.count(0), set_type::size_type(1));
  for (int i = 1; i < 9; ++i) {
    EXPECT_EQ(s.count(i), set_type::size_type(0));
  }
  EXPECT_EQ(s.count(9), set_type::size_type(1));
}

TEST(Set, EraseWithKey) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s;

  for (int i = 0; i < 10; ++i) {
    s.insert(i);
  }

  EXPECT_EQ(s.erase(0), set_type::size_type(1));
  EXPECT_EQ(s.erase(10), set_type::size_type(0));
  EXPECT_EQ(s.count(0), set_type::size_type(0));
}

TEST(Set, Swap) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s1;
  s1.insert(0);
  s1.insert(1);
  s1.insert(2);

  set_type s2;
  s2.insert(3);
  s2.insert(4);
  s2.insert(5);

  s1.swap(s2);

  EXPECT_EQ(s1.count(0), set_type::size_type(0));
  EXPECT_EQ(s1.count(1), set_type::size_type(0));
  EXPECT_EQ(s1.count(2), set_type::size_type(0));
  EXPECT_EQ(s1.count(3), set_type::size_type(1));
  EXPECT_EQ(s1.count(4), set_type::size_type(1));
  EXPECT_EQ(s1.count(5), set_type::size_type(1));

  EXPECT_EQ(s2.count(0), set_type::size_type(1));
  EXPECT_EQ(s2.count(1), set_type::size_type(1));
  EXPECT_EQ(s2.count(2), set_type::size_type(1));
  EXPECT_EQ(s2.count(3), set_type::size_type(0));
  EXPECT_EQ(s2.count(4), set_type::size_type(0));
  EXPECT_EQ(s2.count(5), set_type::size_type(0));
}

TEST(Set, StdSwap) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;

  set_type s1;
  s1.insert(0);
  s1.insert(1);
  s1.insert(2);

  set_type s2;
  s2.insert(3);
  s2.insert(4);
  s2.insert(5);

  std::swap(s1, s2);

  EXPECT_EQ(s1.count(0), set_type::size_type(0));
  EXPECT_EQ(s1.count(1), set_type::size_type(0));
  EXPECT_EQ(s1.count(2), set_type::size_type(0));
  EXPECT_EQ(s1.count(3), set_type::size_type(1));
  EXPECT_EQ(s1.count(4), set_type::size_type(1));
  EXPECT_EQ(s1.count(5), set_type::size_type(1));

  EXPECT_EQ(s2.count(0), set_type::size_type(1));
  EXPECT_EQ(s2.count(1), set_type::size_type(1));
  EXPECT_EQ(s2.count(2), set_type::size_type(1));
  EXPECT_EQ(s2.count(3), set_type::size_type(0));
  EXPECT_EQ(s2.count(4), set_type::size_type(0));
  EXPECT_EQ(s2.count(5), set_type::size_type(0));
}

TEST(Set, CountAndFind) {
  typedef int key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;
  s.insert(0);

  // 要素が存在している
  EXPECT_EQ(s.count(0), set_type::size_type(1));
  set_iterator it = s.find(0);
  EXPECT_TRUE(it != s.end());
  EXPECT_EQ(*it, 0);

  // 要素が存在していない
  EXPECT_EQ(s.count(1), set_type::size_type(0));
  it = s.find(1);
  EXPECT_TRUE(it == s.end());
}

TEST(Set, EqualRange) {
  typedef std::string key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;
  typedef ft::pair<set_iterator, set_iterator> result_type;

  set_type s;
  s.insert("A");
  s.insert("C");
  s.insert("E");

  result_type result = s.equal_range("A");
  for (set_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ(*it, "A");
  }
  result = s.equal_range("C");
  for (set_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ(*it, "C");
  }
  result = s.equal_range("E");
  for (set_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ(*it, "E");
  }
}

TEST(Set, LowerBound) {
  typedef std::string key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;
  s.insert("A");
  s.insert("C");
  s.insert("E");

  // lower_bound() Returns an iterator pointing to the first element
  // that is not less than (i.e. greater or equal to) key.
  set_iterator it = s.lower_bound("A");
  EXPECT_EQ(*it, "A");
  it = s.lower_bound("B");
  EXPECT_EQ(*it, "C");
  it = s.lower_bound("C");
  EXPECT_EQ(*it, "C");
  it = s.lower_bound("D");
  EXPECT_EQ(*it, "E");
  it = s.lower_bound("E");
  EXPECT_EQ(*it, "E");
  it = s.lower_bound("F");
  EXPECT_EQ(it, s.end());
}

TEST(Set, UpperBound) {
  typedef std::string key_type;
  typedef ft::set<key_type> set_type;
  typedef set_type::iterator set_iterator;

  set_type s;
  s.insert("A");
  s.insert("C");
  s.insert("E");

  // upper_bound() Returns an iterator pointing to the first element
  // that is greater than key.
  set_iterator it = s.upper_bound("A");
  EXPECT_EQ(*it, "C");
  it = s.upper_bound("B");
  EXPECT_EQ(*it, "C");
  it = s.upper_bound("C");
  EXPECT_EQ(*it, "E");
  it = s.upper_bound("D");
  EXPECT_EQ(*it, "E");
  it = s.upper_bound("E");
  EXPECT_EQ(it, s.end());
  it = s.upper_bound("F");
  EXPECT_EQ(it, s.end());
}

TEST(Set, Observerse) {
  typedef std::string key_type;
  typedef ft::set<key_type> set_type;

  set_type s;

  set_type::key_compare key_comp_func = s.key_comp();
  set_type::value_compare value_comp_func = s.value_comp();

  EXPECT_TRUE(key_comp_func("A", "B"));
  EXPECT_FALSE(key_comp_func("A", "A"));

  EXPECT_TRUE(value_comp_func("A", "B"));
  EXPECT_FALSE(value_comp_func("A", "A"));
}

TEST(Set, Operators) {
  typedef std::string key_type;
  typedef ft::set<key_type> set_type;

  set_type s1;
  set_type s2;

  // s1 == s2
  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 != s2);
  EXPECT_FALSE(s1 < s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s1 >= s2);

  // s1 > s2
  s1.insert("A");
  EXPECT_FALSE(s1 == s2);
  EXPECT_TRUE(s1 != s2);
  EXPECT_FALSE(s1 < s2);
  EXPECT_FALSE(s1 <= s2);
  EXPECT_TRUE(s1 > s2);
  EXPECT_TRUE(s1 >= s2);

  // s1 == s2
  s2.insert("A");
  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 != s2);
  EXPECT_FALSE(s1 < s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s1 >= s2);

  // s1 < s2
  s2.insert("B");
  EXPECT_FALSE(s1 == s2);
  EXPECT_TRUE(s1 != s2);
  EXPECT_TRUE(s1 < s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s1 > s2);
  EXPECT_FALSE(s1 >= s2);
}

TEST(Set, MyOriginalClass) {
  typedef ft::test::Student Student;
  typedef ft::set<Student, Student::Compare> set_type;
  typedef set_type::iterator set_iterator;

  set_type students;
  Student jun("JUN", 20);
  Student bob("Bob", 30);
  Student chris("Chris", 10);
  Student pika("Pika", 10);
  students.insert(jun);
  students.insert(bob);
  students.insert(chris);
  students.insert(pika);

  EXPECT_EQ(students.size(), set_type::size_type(4));
  for (set_iterator it = students.begin(); it != students.end(); ++it) {
    // Student.id_ は名前のハッシュ値なのだ。
    EXPECT_EQ((*it).id_, ft::test::generate_hash((*it).name_));
  }

  // remove Bob and Chris
  students.erase(bob);
  students.erase(chris);

  EXPECT_EQ(students.size(), set_type::size_type(2));
  EXPECT_EQ(students.count(jun), set_type::size_type(1));
  EXPECT_EQ(students.count(bob), set_type::size_type(0));
  EXPECT_EQ(students.count(chris), set_type::size_type(0));
  EXPECT_EQ(students.count(pika), set_type::size_type(1));
  EXPECT_TRUE(students.find(jun) != students.end());
  EXPECT_TRUE(students.find(bob) == students.end());
  EXPECT_TRUE(students.find(chris) == students.end());
  EXPECT_TRUE(students.find(pika) != students.end());
}