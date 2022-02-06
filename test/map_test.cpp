#include "map.hpp"

#include <gtest/gtest.h>

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
#include "test_utils.hpp"
#include "utils/comparison.hpp"
#include "utils/my_allocator.hpp"

namespace {
const std::uint64_t kLoopMax = 1000;
}

TEST(DefaultConstructor, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  EXPECT_EQ(m.size(), map_type::size_type(0));
  EXPECT_EQ(m.begin(), m.end());
}

TEST(ConstructorWithCompareMethod, CustomizableComparison) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::test::less_or_greater<key_type> compare_type;
  typedef ft::map<key_type, mapped_type, compare_type> map_type;
  typedef map_type::iterator map_iterator;

  const compare_type lesscomp = compare_type(true);
  const compare_type greatercomp = compare_type(false);

  map_type less_map(lesscomp);
  map_type greater_map(greatercomp);

  for (char c = 'A'; c <= 'Z'; ++c) {
    std::string s(1, c);
    less_map[s] = 0;
    greater_map[s] = 0;
  }

  map_iterator it = less_map.begin();
  for (char c = 'A'; c <= 'Z' && it != less_map.end(); ++c, ++it) {
    EXPECT_EQ((*it).first[0], c);
  }
  EXPECT_EQ(it, less_map.end());

  it = greater_map.begin();
  for (char c = 'Z'; c >= 'A' && it != greater_map.end(); --c, ++it) {
    EXPECT_EQ((*it).first[0], c);
  }
  EXPECT_EQ(it, greater_map.end());
}

TEST(ConstructorWithIterator, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::iterator map_iterator;
  typedef std::vector<map_type::value_type> vector_type;
  typedef vector_type::iterator vector_iterator;

  vector_type vec;

  for (int c = 'A'; c <= 'Z'; ++c) {
    std::string s(1, c);
    vec.push_back(map_type::value_type(s, 0));
  }

  map_type m(vec.begin(), vec.end());

  map_iterator map_it = m.begin();
  vector_iterator vec_it = vec.begin();
  for (; vec_it != vec.end(); ++vec_it, ++map_it) {
    EXPECT_EQ((*map_it).first, (*vec_it).first);
  }
  EXPECT_EQ(map_it, m.end());
}

TEST(CopyConstructorAndAssignation, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m1;
  m1["A"] = 0;
  m1["B"] = 1;

  map_type m2(m1);
  m2["A"] = 1;
  m2["B"] = 2;

  map_type m3;
  m3 = m2;
  m3["A"] = 2;
  m3["B"] = 3;

  EXPECT_EQ(m1["A"], 0);
  EXPECT_EQ(m1["B"], 1);

  EXPECT_EQ(m2["A"], 1);
  EXPECT_EQ(m2["B"], 2);

  EXPECT_EQ(m3["A"], 2);
  EXPECT_EQ(m3["B"], 3);
}

TEST(GetAllocator, DefaultAllocator) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef std::allocator<pair_type> allocator_type;
  typedef ft::map<key_type, mapped_type, std::less<key_type>, allocator_type>
      map_type;

  map_type m;
  map_type::allocator_type allocator = m.get_allocator();
  EXPECT_TRUE(dynamic_cast<allocator_type*>(&allocator) != nullptr);
}

TEST(GetAllocator, CustomAllocator) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef ft::test::MyAllocator<pair_type> allocator_type;
  typedef ft::map<key_type, mapped_type, std::less<key_type>, allocator_type>
      map_type;

  map_type m;
  map_type::allocator_type allocator = m.get_allocator();
  EXPECT_TRUE(dynamic_cast<allocator_type*>(&allocator) != nullptr);
}

TEST(ElementAccess, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  for (char c = 'A'; c <= 'Z'; ++c) {
    key_type s(1, c);
    m[s] = static_cast<mapped_type>(c);
  }

  // operator[]
  for (char c = 'A'; c <= 'Z'; ++c) {
    key_type s(1, c);
    EXPECT_EQ(m[s], static_cast<mapped_type>(c));
  }
  // operator[] はキーが存在しない時はノードを挿入してくれる
  key_type s = "operator[]";
  EXPECT_EQ(m[s], mapped_type());

  // at() はキーが存在しない時は out_of_range 例外を投げる
  s = "at()";
  EXPECT_THROW(m.at(s), std::out_of_range);
  m[s] = 100;
  EXPECT_EQ(m.at(s), 100);
}

TEST(ForwardIterator, TestAll) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;
  typedef map_type::const_iterator const_map_iterator;

  // non-const forward iterator test
  map_type m1;

  EXPECT_EQ(m1.begin(), m1.end());

  m1["A"] = 1;
  m1["B"] = 2;
  m1["C"] = 3;
  m1["D"] = 4;
  m1["E"] = 5;

  map_iterator it = m1.begin();
  EXPECT_EQ(*it++, pair_type("A", 1));
  EXPECT_EQ(*it++, pair_type("B", 2));
  EXPECT_EQ(*it++, pair_type("C", 3));
  EXPECT_EQ(*it++, pair_type("D", 4));
  EXPECT_EQ(*it++, pair_type("E", 5));
  EXPECT_EQ(it, m1.end());

  // const forward iterator test
  const map_type m2(m1);
  const_map_iterator cit = m2.begin();
  EXPECT_EQ(*cit++, pair_type("A", 1));
  EXPECT_EQ(*cit++, pair_type("B", 2));
  EXPECT_EQ(*cit++, pair_type("C", 3));
  EXPECT_EQ(*cit++, pair_type("D", 4));
  EXPECT_EQ(*cit++, pair_type("E", 5));
  EXPECT_EQ(cit, m2.end());
}

TEST(ReverseIterator, TestALl) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::reverse_iterator map_reverse_iterator;
  typedef map_type::const_reverse_iterator const_map_reverse_iterator;

  // non-const forward iterator test
  map_type m1;

  EXPECT_EQ(m1.rbegin(), m1.rend());

  m1["A"] = 1;
  m1["B"] = 2;
  m1["C"] = 3;
  m1["D"] = 4;
  m1["E"] = 5;

  map_reverse_iterator it = m1.rbegin();
  EXPECT_EQ(*it++, pair_type("E", 5));
  EXPECT_EQ(*it++, pair_type("D", 4));
  EXPECT_EQ(*it++, pair_type("C", 3));
  EXPECT_EQ(*it++, pair_type("B", 2));
  EXPECT_EQ(*it++, pair_type("A", 1));
  EXPECT_EQ(it, m1.rend());

  // const forward iterator test
  const map_type m2(m1);
  const_map_reverse_iterator cit = m2.rbegin();
  EXPECT_EQ(*cit++, pair_type("E", 5));
  EXPECT_EQ(*cit++, pair_type("D", 4));
  EXPECT_EQ(*cit++, pair_type("C", 3));
  EXPECT_EQ(*cit++, pair_type("B", 2));
  EXPECT_EQ(*cit++, pair_type("A", 1));
  EXPECT_EQ(cit, m2.rend());
}

TEST(Capacity, SimpleTest) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  EXPECT_EQ(m.size(), map_type::size_type(0));
  EXPECT_EQ(m.empty(), true);

  m["A"] = 0;
  m["B"] = 0;

  EXPECT_EQ(m.size(), map_type::size_type(2));
  EXPECT_EQ(m.empty(), false);
}

TEST(Clear, RemoveAllElements) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  m["A"] = 0;
  m["B"] = 1;

  m.clear();

  EXPECT_THROW(m.at("A"), std::out_of_range);
  EXPECT_EQ(m.size(), map_type::size_type(0));
  EXPECT_EQ(m.empty(), true);
}

TEST(Insert, InsertElement) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  // 要素が無い場合は挿入される
  m.insert(pair_type("A", 1));
  EXPECT_EQ(m["A"], 1);

  // 既に要素がある場合は挿入は無視される
  m.insert(pair_type("A", 2));
  EXPECT_EQ(m["A"], 1);

  // 要素を消してもう一回入れ直す
  m.erase("A");
  m.insert(pair_type("A", 3));
  EXPECT_EQ(m["A"], 3);
}

TEST(InsertWithHint, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;

  map_type m;

  m["C"] = 3;
  m["D"] = 4;
  m["E"] = 5;

  map_iterator it = m.find("C");

  // hintの隣に配置されるkey
  m.insert(it, pair_type("B", 2));

  // hintの隣に配置されないkey
  m.insert(it, pair_type("A", 1));

  // 問題なく全て入っているか
  it = m.begin();
  EXPECT_EQ(*it++, pair_type("A", 1));
  EXPECT_EQ(*it++, pair_type("B", 2));
  EXPECT_EQ(*it++, pair_type("C", 3));
  EXPECT_EQ(*it++, pair_type("D", 4));
  EXPECT_EQ(*it++, pair_type("E", 5));
  EXPECT_EQ(it, m.end());
}

TEST(InsertRange, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;
  typedef std::vector<pair_type> pair_vector;

  pair_vector vec;

  for (char c = 'A'; c <= 'Z'; ++c) {
    key_type s(1, c);
    vec.push_back(pair_type(s, static_cast<mapped_type>(c)));
  }

  map_type m;

  m.insert(vec.begin(), vec.end());

  map_iterator it = m.begin();
  for (char c = 'A'; c <= 'Z'; ++c) {
    key_type s(1, c);
    EXPECT_EQ(*it, pair_type(s, static_cast<mapped_type>(c)));
    ++it;
  }
  EXPECT_EQ(it, m.end());
}

TEST(Erase, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;

  map_type m;
  m["A"] = 1;
  m["B"] = 2;
  m["C"] = 3;
  m["D"] = 4;
  m["E"] = 5;

  // erase with key
  m.erase("C");
  EXPECT_EQ(m.size(), map_type::size_type(4));
  EXPECT_THROW(m.at("C"), std::out_of_range);

  // erase with iterator
  // remove B,D
  map_iterator first = m.lower_bound("B");
  map_iterator last = m.upper_bound("D");
  m.erase(first, last);
  EXPECT_EQ(m.size(), map_type::size_type(2));
  EXPECT_THROW(m.at("B"), std::out_of_range);
  EXPECT_THROW(m.at("D"), std::out_of_range);

  map_iterator it = m.begin();
  EXPECT_EQ(*it++, pair_type("A", 1));
  EXPECT_EQ(*it++, pair_type("E", 5));
}

TEST(Swap, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;

  map_type m1;
  m1["A"] = 1;
  m1["B"] = 2;
  m1["C"] = 3;

  map_type m2;
  m2["D"] = 4;
  m2["E"] = 5;
  m2["F"] = 6;

  m1.swap(m2);

  map_iterator it = m1.begin();
  EXPECT_EQ(*it++, pair_type("D", 4));
  EXPECT_EQ(*it++, pair_type("E", 5));
  EXPECT_EQ(*it++, pair_type("F", 6));
  EXPECT_EQ(it, m1.end());

  it = m2.begin();
  EXPECT_EQ(*it++, pair_type("A", 1));
  EXPECT_EQ(*it++, pair_type("B", 2));
  EXPECT_EQ(*it++, pair_type("C", 3));
  EXPECT_EQ(it, m2.end());
}

TEST(FindAndCount, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::value_type pair_type;
  typedef map_type::iterator map_iterator;

  map_type m;
  m["A"] = 1;
  m["C"] = 3;
  m["E"] = 5;

  // find()
  map_iterator it = m.find("A");
  EXPECT_EQ(*it, pair_type("A", 1));
  it = m.find("B");
  EXPECT_EQ(it, m.end());
  it = m.find("C");
  EXPECT_EQ(*it, pair_type("C", 3));
  it = m.find("D");
  EXPECT_EQ(it, m.end());
  it = m.find("E");
  EXPECT_EQ(*it, pair_type("E", 5));

  // count()
  EXPECT_EQ(m.count("A"), map_type::size_type(1));
  EXPECT_EQ(m.count("B"), map_type::size_type(0));
  EXPECT_EQ(m.count("C"), map_type::size_type(1));
  EXPECT_EQ(m.count("D"), map_type::size_type(0));
  EXPECT_EQ(m.count("E"), map_type::size_type(1));
}

TEST(EqualRange, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::iterator map_iterator;
  typedef ft::pair<map_iterator, map_iterator> result_type;

  map_type m;
  m["A"] = 1;
  m["C"] = 3;
  m["E"] = 5;

  result_type result = m.equal_range("A");
  for (map_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ((*it).first, "A");
  }
  result = m.equal_range("C");
  for (map_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ((*it).first, "C");
  }
  result = m.equal_range("E");
  for (map_iterator it = result.first; it != result.second; ++it) {
    EXPECT_EQ((*it).first, "E");
  }
}

TEST(LowerBound, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::iterator map_iterator;

  map_type m;
  m["A"] = 1;
  m["C"] = 3;
  m["E"] = 5;

  // lower_bound() Returns an iterator pointing to the first element
  // that is not less than (i.e. greater or equal to) key.
  map_iterator it = m.lower_bound("A");
  EXPECT_EQ((*it).first, "A");
  it = m.lower_bound("B");
  EXPECT_EQ((*it).first, "C");
  it = m.lower_bound("C");
  EXPECT_EQ((*it).first, "C");
  it = m.lower_bound("D");
  EXPECT_EQ((*it).first, "E");
  it = m.lower_bound("E");
  EXPECT_EQ((*it).first, "E");
  it = m.lower_bound("F");
  EXPECT_EQ(it, m.end());
}

TEST(UpperBound, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;
  typedef map_type::iterator map_iterator;

  map_type m;
  m["A"] = 1;
  m["C"] = 3;
  m["E"] = 5;

  // upper_bound() Returns an iterator pointing to the first element
  // that is greater than key.
  map_iterator it = m.upper_bound("A");
  EXPECT_EQ((*it).first, "C");
  it = m.upper_bound("B");
  EXPECT_EQ((*it).first, "C");
  it = m.upper_bound("C");
  EXPECT_EQ((*it).first, "E");
  it = m.upper_bound("D");
  EXPECT_EQ((*it).first, "E");
  it = m.upper_bound("E");
  EXPECT_EQ(it, m.end());
  it = m.upper_bound("F");
  EXPECT_EQ(it, m.end());
}

TEST(Observers, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::pair<key_type, mapped_type> pair_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m;

  map_type::key_compare key_comp_func = m.key_comp();
  map_type::value_compare value_comp_func = m.value_comp();

  EXPECT_TRUE(key_comp_func("A", "B"));
  EXPECT_FALSE(key_comp_func("A", "A"));

  EXPECT_TRUE(value_comp_func(pair_type("A", 0), pair_type("B", 0)));
  EXPECT_FALSE(value_comp_func(pair_type("A", 0), pair_type("A", 0)));
}

TEST(Operators, Normal) {
  typedef std::string key_type;
  typedef int mapped_type;
  typedef ft::map<key_type, mapped_type> map_type;

  map_type m1;
  map_type m2;

  // m1 == m2
  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);
  EXPECT_FALSE(m1 < m2);
  EXPECT_TRUE(m1 <= m2);
  EXPECT_FALSE(m1 > m2);
  EXPECT_TRUE(m1 >= m2);

  // m1 > m2
  m1["A"] = 0;
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m1 != m2);
  EXPECT_FALSE(m1 < m2);
  EXPECT_FALSE(m1 <= m2);
  EXPECT_TRUE(m1 > m2);
  EXPECT_TRUE(m1 >= m2);

  // m1 == m2
  m2["A"] = 0;
  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);
  EXPECT_FALSE(m1 < m2);
  EXPECT_TRUE(m1 <= m2);
  EXPECT_FALSE(m1 > m2);
  EXPECT_TRUE(m1 >= m2);

  // m1 < m2
  m1["B"] = 0;
  m2["B"] = 1;
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m1 != m2);
  EXPECT_TRUE(m1 < m2);
  EXPECT_TRUE(m1 <= m2);
  EXPECT_FALSE(m1 > m2);
  EXPECT_FALSE(m1 >= m2);

  // m1 < m2
  m2["C"] = 2;
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m1 != m2);
  EXPECT_TRUE(m1 < m2);
  EXPECT_TRUE(m1 <= m2);
  EXPECT_FALSE(m1 > m2);
  EXPECT_FALSE(m1 >= m2);
}

TEST(MapFeatures, MyOriginalClassStudent) {
  typedef ft::test::Student Student;
  typedef ft::map<Student::id_type, Student> students_map_type;
  typedef students_map_type::iterator students_map_iterator;
  typedef ft::map<std::string, Student::id_type> student_id_table_type;

  students_map_type students;
  student_id_table_type student_id_table;

  Student jun("JUN", 20);
  student_id_table[jun.name_] = jun.id_;
  students[jun.id_] = jun;

  Student bob("Bob", 30);
  student_id_table[bob.name_] = bob.id_;
  students[bob.id_] = bob;

  Student chris("Chris", 10);
  student_id_table[chris.name_] = chris.id_;
  students[chris.id_] = chris;

  Student pika("Pika", 10);
  student_id_table[pika.name_] = pika.id_;
  students[pika.id_] = pika;

  EXPECT_EQ(students.size(), students_map_type::size_type(4));
  EXPECT_EQ(student_id_table.size(), student_id_table_type::size_type(4));

  for (students_map_iterator it = students.begin(); it != students.end();
       ++it) {
    // Student.id_ は名前のハッシュ値なのだ。
    EXPECT_EQ((*it).first, ft::test::generate_hash((*it).second.name_));
    EXPECT_EQ((*it).first, student_id_table[(*it).second.name_]);
  }

  // remove Bob and Chris
  students.erase(student_id_table["Bob"]);
  student_id_table.erase("Bob");
  students.erase(student_id_table["Chris"]);
  student_id_table.erase("Chris");

  EXPECT_EQ(students.size(), students_map_type::size_type(2));
  EXPECT_EQ(student_id_table.size(), student_id_table_type::size_type(2));
}