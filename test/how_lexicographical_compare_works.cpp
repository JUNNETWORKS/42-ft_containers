#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
#include <vector>

int main(int argc, char** argv) {
  {
    // lexicographical_compare
    typedef std::vector<int> container;
    typedef typename container::iterator iterator;

    container vec1;
    container vec2;
    for (int i = 0; i < 10; ++i) {
      vec1.push_back(i);
      vec2.push_back(i * 2);
    }

    std::cout << std::boolalpha
              << std::lexicographical_compare(vec1.begin(), vec1.end(),
                                              vec2.begin(), vec2.end())
              << std::endl;

    std::cout << std::boolalpha
              << std::lexicographical_compare(vec2.begin(), vec2.end(),
                                              vec1.begin(), vec1.end())
              << std::endl;
  }
}