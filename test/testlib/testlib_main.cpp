#include <iostream>
#include <vector>

#include "testlib/testlib.hpp"

test_func_vec_type test_functions;
testf_func_vec_type testf_functions;

#include "lexicographical_compare_test.cpp"
#include "map_test.cpp"
#include "pair_test.cpp"
#include "red_black_tree_test.cpp"
#include "set_test.cpp"
#include "stack_test.cpp"
#include "type_traits_test.cpp"
#include "vector_test.cpp"

namespace ft {
namespace test {

bool MakeAndRegisterTestInfo(std::string test_name, test_func_type func) {
  test_functions.push_back(test_func_pair_type(test_name, func));
  return true;
}

bool MakeAndRegisterTestInfo(std::string test_name, testing::Test *func) {
  testf_functions.push_back(testf_func_pair_type(test_name, func));
  return true;
}

}  // namespace test
}  // namespace ft

int main() {
  typedef test_func_vec_type::iterator test_func_vec_iterator;
  typedef testf_func_vec_type::iterator testf_func_vec_iterator;

  std::cout << "Test Start!!" << std::endl;
  std::cout << "Test count: " << test_functions.size() << std::endl;

  for (test_func_vec_iterator it = test_functions.begin();
       it != test_functions.end(); ++it) {
    std::string test_func_name = (*it).first;
    test_func_type test_func = (*it).second;

    std::cout << "RUN:     " << test_func_name << std::endl;
    test_func();
    std::cout << "SUCCESS: " << test_func_name << "\n" << std::endl;
  }

  std::cout << "Test Finished!!" << std::endl;

  std::cout << "RUN TEST_F()" << std::endl;
  for (testf_func_vec_iterator it = testf_functions.begin();
       it != testf_functions.end(); ++it) {
    std::string test_func_name = (*it).first;
    testing::Test *test_obj = (*it).second;

    std::cout << "RUN:     " << test_func_name << std::endl;
    test_obj->Run();
    std::cout << "SUCCESS: " << test_func_name << "\n" << std::endl;
  }
}