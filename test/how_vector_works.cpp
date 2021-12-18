#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
#include <vector>

#include "../srcs/type_traits.hpp"

class MyClass {
 public:
  MyClass() {
    std::cout << "MyClass default constructor is called!!" << std::endl;
  }

  ~MyClass() {
    std::cout << "MyClass destructor is called!!" << std::endl;
  }
};

template <typename T>
void showVectorInfo(std::vector<T>& vec) {
  std::cout << "Vector Information"
            << "\tsize: " << vec.size() << "\n"
            << "\tcap: " << vec.size() << std::endl;
}

// stdバージョン
template <typename T>
void thisIsStdInteger(
    T x, typename std::enable_if<std::is_integral<T>::value>::type* = 0) {
  std::cout << "This is integer. Std." << std::endl;
}

template <typename T>
void thisIsInteger(
    T x, typename ft::enable_if<ft::is_integral<T>::value>::type* = 0) {
  std::cout << "This is integer" << std::endl;
}

template <typename T>
void print_vector(std::vector<T>& vec) {
  int i = 0;
  for (typename std::vector<T>::const_iterator it = vec.begin();
       it != vec.end(); ++it) {
    std::cout << "vec[" << i << "]: " << vec[i] << std::endl;
    ++i;
  }
}

int main(int argc, char** argv) {
  /* enable_if, is_integral */
  {
    // these should be able to compile
    thisIsInteger<int>(10);
    thisIsInteger(10);
    thisIsInteger(10u);
    thisIsStdInteger(10u);
    thisIsInteger(10l);
    thisIsInteger(10ll);
    thisIsInteger('a');

    // these shouldn't be able to compile
    // thisIsInteger("abc");
    // thisIsInteger(std::string("abc"));
    // thisIsInteger(10.0f);
    // thisIsInteger(10.0);
  }

  /* vector */
  {
    std::vector<int> a(10);
    std::cout << "----- a -----" << std::endl;
    std::cout << "size: " << a.size() << std::endl;
    std::cout << "max_size: " << a.max_size() << std::endl;
    std::cout << "capacity: " << a.capacity() << std::endl;
    std::cout << "empty: " << a.empty() << std::endl;
    std::vector<int>::iterator it = a.begin();
  }

  {
    std::vector<int> a(10);
    std::vector<int> additional_data;
    for (int i = 0; i < 10; ++i) {
      additional_data.push_back(i);
    }
    showVectorInfo(a);
    showVectorInfo(additional_data);
    a.assign(additional_data.begin(), additional_data.end());
    showVectorInfo(a);
  }

  {
    std::vector<int> b;
    std::cout << "----- b -----" << std::endl;
    std::cout << "size: " << b.size() << std::endl;
    std::cout << "max_size: " << b.max_size() << std::endl;
    std::cout << "capacity: " << b.capacity() << std::endl;
    std::cout << "empty: " << b.empty() << std::endl;
  }

  {
    std::vector<int> c(0);
    std::cout << "----- c -----" << std::endl;
    std::cout << "size: " << c.size() << std::endl;
    std::cout << "max_size: " << c.max_size() << std::endl;
    std::cout << "capacity: " << c.capacity() << std::endl;
    std::cout << "empty: " << c.empty() << std::endl;
  }

  {
    std::vector<int> d(1);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    d.push_back(2);
    std::cout << "capacity: " << d.capacity() << std::endl;
    std::cout << "d[0]: " << d[0] << std::endl;
  }

  {
    std::cout << "===== class test =====" << std::endl;
    std::vector<MyClass> my_class_vector(1);
    std::cout << "=== pop_back() ===" << std::endl;
    my_class_vector.pop_back();
  }

  {
    std::cout << "==== assign test ====" << std::endl;
    std::vector<int> assign_test_vector(10);
    std::cout << "size: " << assign_test_vector.size() << std::endl;
    std::cout << "capacity: " << assign_test_vector.capacity() << std::endl;
    print_vector(assign_test_vector);
    std::cout << std::endl;

    assign_test_vector.assign(5, 1);
    std::cout << "size: " << assign_test_vector.size() << std::endl;
    std::cout << "capacity: " << assign_test_vector.capacity() << std::endl;
    print_vector(assign_test_vector);
    std::cout << std::endl;

    assign_test_vector.assign(10, 2);
    std::cout << "size: " << assign_test_vector.size() << std::endl;
    std::cout << "capacity: " << assign_test_vector.capacity() << std::endl;
    print_vector(assign_test_vector);
    std::cout << std::endl;

    assign_test_vector.assign(20, 3);
    std::cout << "size: " << assign_test_vector.size() << std::endl;
    std::cout << "capacity: " << assign_test_vector.capacity() << std::endl;
    print_vector(assign_test_vector);
  }

  {
    std::cout << "==== erase test ====" << std::endl;
    std::vector<std::string> str_vector;
    str_vector.push_back("hoge");
    str_vector.push_back("fuga");
    str_vector.push_back("hogefuga");
    print_vector(str_vector);
    std::cout << std::endl;
    str_vector.erase(str_vector.begin());
    print_vector(str_vector);
  }

  /* map */
  {}

  /* stack */
  {}

  /* set (bonus) */
  {}
}