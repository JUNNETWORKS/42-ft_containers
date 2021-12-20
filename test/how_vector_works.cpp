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
void printVectorInfo(std::vector<T>& vec) {
  std::cout << "Vector Information"
            << "\tsize: " << vec.size() << "\n"
            << "\tcap: " << vec.capacity() << std::endl;
}

template <typename T>
void printVector(std::vector<T>& vec) {
  std::cout << "Vector Data" << std::endl;
  for (typename std::vector<T>::const_iterator it = vec.begin();
       it != vec.end(); ++it) {
    std::cout << "\t" << *it << std::endl;
  }
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
    printVectorInfo(a);
    printVectorInfo(additional_data);
    a.assign(additional_data.begin(), additional_data.end());
    printVectorInfo(a);
  }

  {
    std::cout
        << "--------------------------- insert test ---------------------------"
        << std::endl;
    std::vector<std::string> a(4, "__init__");

    printVectorInfo(a);
    printVector(a);
    std::cout << "------------------------" << std::endl;
    a.insert(a.begin(), "this is string at begin().");
    printVectorInfo(a);
    printVector(a);
    std::cout << "------------------------" << std::endl;
    a.insert(a.begin() + 1, "this is string at begin() + 1.");
    printVectorInfo(a);
    printVector(a);
    std::cout << "------------------------" << std::endl;
    a.insert(a.begin() + 1, "this is string at begin() + 1.");
    printVectorInfo(a);
    printVector(a);
    std::cout << "------------------------" << std::endl;
  }

  {
    std::cout << "--------------------------- insert(it, it) test "
                 "---------------------------"
              << std::endl;
    std::vector<std::string> a(4, "__init__");

    std::vector<std::string> additional_data(2, "additional");

    printVectorInfo(a);
    printVector(a);
    std::cout << "------------------------" << std::endl;
    a.insert(a.begin(), additional_data.begin(), additional_data.end());
    printVectorInfo(a);
    printVector(a);
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
    typedef std::vector<int> container;
    typedef typename container::iterator iterator;
    std::cout << "==== erase test ====" << std::endl;
    std::vector<int> vec;
    for (int i = 0; i < 10; ++i) {
      vec.push_back(i);
    }
    print_vector(vec);

    iterator it = vec.erase(vec.begin() + 1, vec.end() - 2);
    std::cout << "return *it: " << *it << std::endl;
    print_vector(vec);
  }
}