#include <iostream>
#include <map>

template <typename K, typename V>
void print(std::map<K, V> &m) {
  typename std::map<K, V>::const_iterator it = m.begin();
  for (int i = 0; it != m.end(); ++it, ++i) {
    std::cout << "map[" << it->first << "]: " << it->second << std::endl;
  }
}

int main() {
  {
    std::map<int, int> m;
    for (int i = 0; i < 10; ++i) {
      m.insert(std::make_pair(i, i * i));
    }
    print(m);
    std::cout << m[20] << std::endl;
  }
}