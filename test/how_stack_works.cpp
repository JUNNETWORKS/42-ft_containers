#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>

int main() {
  {
    std::stack<int> hoge;
    hoge.push(0);
    std::stack<int> fuga;
    fuga = hoge;
    fuga.push(1);
    std::cout << "hoge: " << hoge.size() << std::endl;
    std::cout << "fuga: " << fuga.size() << std::endl;
  }

  return 0;
}
