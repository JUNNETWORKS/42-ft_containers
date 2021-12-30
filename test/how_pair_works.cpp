#include <iostream>
#include <utility>

template <class T1, class T2>
void print(std::string rem, const std::pair<T1, T2>& pair) {
  std::cout << rem << "(" << pair.first << ", " << pair.second << ")\n";
};

int main() {
  std::pair<int, float> p1;
  print("(1) Value-initialized: ", p1);

  std::pair<int, double> p2(42, 3.1415);
  print("(2) Initialized with two values: ", p2);

  std::pair<char, int> p4(p2);
  print("(4) Implicitly converted: ", p4);

  std::pair<std::string, unsigned int> a("person1", 20);
  std::pair<std::string, unsigned int> b;
  b = a;
  print("a", a);
  print("b", b);
}
