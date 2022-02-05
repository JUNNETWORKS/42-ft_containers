#include <cassert>
#include <iostream>
#include <map>

#include "map.hpp"
#include "pair.hpp"

inline void PrintHeader(std::string s) {
  std::cout << "---------- " << s << " ----------" << std::endl;
}

template <typename Pair>
struct Select1st {
  /// @c argument_type is the type of the argument
  typedef Pair argument_type;

  /// @c result_type is the return type
  typedef typename Pair::first_type result_type;

  typename Pair::first_type& operator()(Pair& p) const {
    return p.first;
  }

  const typename Pair::first_type& operator()(const Pair& p) const {
    return p.first;
  }
};

class Student {
 public:
  Student(std::string& name) : name_(name) {}

 private:
  std::string name_;
};

template <typename K, typename V>
void print(std::map<K, V>& m) {
  typename std::map<K, V>::const_iterator it = m.begin();
  for (int i = 0; it != m.end(); ++it, ++i) {
    std::cout << "map[" << it->first << "]: " << it->second << std::endl;
  }
}

int main() {
  PrintHeader("lower_bound()");
  {
    typedef std::map<std::string, int> map_type;
    map_type m;

    m["a"] = 0;
    m["c"] = 0;

    map_type::iterator it = m.lower_bound("b");
    std::cout << it->first << std::endl;

    m["b"] = 0;
    it = m.lower_bound("b");
    std::cout << it->first << std::endl;

    print(m);
  }

  PrintHeader("Select1st");
  {
    typedef ft::pair<std::string, int> pair_type;

    pair_type p = pair_type("JUN", 20);
    pair_type p2 = pair_type("HOGE", 21);
    std::cout << Select1st<pair_type>()(p) << std::endl;
    std::cout << std::less<std::string>()(Select1st<pair_type>()(p),
                                          Select1st<pair_type>()(p2))
              << std::endl;
  }

  PrintHeader("iterator");
  {
    typedef std::map<std::string, int> map_type;

    map_type m;
    m["a"] = 0;
    map_type::iterator it = m.begin();
    std::cout << (*it).first << std::endl;
    --it;
    std::cout << (*it).first << std::endl;
    --it;
    std::cout << (*it).first << std::endl;
  }

  PrintHeader("clear()");
  {
    typedef std::map<std::string, int> map_type;
    typedef map_type::iterator map_iterator;
    typedef map_type::reverse_iterator map_reverse_iterator;

    map_type m;

    m["a"] = 0;

    // Any past-the-end iterator remains valid.
    map_iterator it = m.end();
    map_reverse_iterator rit = m.rend();
    m.clear();
    assert(m.end() == it);
    // assert(m.rend() == rit);
  }

  PrintHeader("insert() with one value");
  {
    typedef std::map<std::string, int> map_type;
    typedef map_type::value_type value_type;

    map_type m;
    m.insert(value_type("a", 0));
    std::cout << "a: " << m["a"] << std::endl;
    m.insert(value_type("a", 1));
    std::cout << "a: " << m["a"] << std::endl;
  }

  PrintHeader("lower_bound");
  {
    typedef std::map<std::string, int> map_type;
    typedef map_type::iterator iterator;
    typedef map_type::value_type value_type;

    map_type m;
    m.insert(value_type("A", 0));
    m.insert(value_type("C", 0));
    m.insert(value_type("E", 0));
    m.insert(value_type("G", 0));
    m.insert(value_type("I", 0));

    iterator it;
    it = m.lower_bound("0");
    std::cout << (*it).first << std::endl;
    it = m.lower_bound("A");
    std::cout << (*it).first << std::endl;
    it = m.lower_bound("B");
    std::cout << (*it).first << std::endl;
    it = m.lower_bound("C");
    std::cout << (*it).first << std::endl;
    it = m.lower_bound("I");
    std::cout << (it == m.end()) << std::endl;
    it = m.lower_bound("J");
    std::cout << (it == m.end()) << std::endl;
  }

  PrintHeader("Test strange default constructor?");
  {
    typedef int key_type;
    typedef int mapped_type;
    typedef std::less<key_type> compare_type;
    typedef ft::map<key_type, mapped_type, compare_type> map_type;
    typedef map_type::iterator iterator;
    typedef map_type::value_type value_type;

    const compare_type comp = compare_type();
    map_type m(comp);
  }

  PrintHeader("My original map");
  {
    typedef std::string key_type;
    typedef int mapped_type;
    typedef std::less<key_type> compare_type;
    typedef ft::map<key_type, mapped_type, compare_type> map_type;
    typedef map_type::iterator iterator;
    typedef map_type::value_type value_type;

    map_type m;
    // m.insert(value_type("A", 0));
    // m.insert(value_type("C", 0));
    // m.insert(value_type("E", 0));
    // m.insert(value_type("G", 0));
    // m.insert(value_type("I", 0));

    while (std::cin && !std::cin.eof()) {
      std::string command;
      std::cout << "\nInput command from the list [ADD, SHOW, SEARCH]"
                << std::endl;
      std::cout << "Command: " << std::flush;
      std::cin >> command;
      if (command != "ADD" && command != "SHOW" && command != "SEARCH") {
        std::cout << "Command is invalid!" << std::endl;
        continue;
      }
      if (!std::cin || std::cin.eof()) {
        break;
      }
      if (command == "ADD") {
        std::string key;
        std::cout << "Key: " << std::flush;
        std::cin >> key;
        if (!std::cin || std::cin.eof()) {
          break;
        }
        int value;
        std::cout << "Value(int): " << std::flush;
        std::cin >> value;
        while (!std::cin && !std::cin.eof()) {
          std::cout << "Input one more time" << std::endl;
          std::cin.clear();
          std::cout << "Value(int): " << std::flush;
          std::cin >> key;
        }
        if (std::cin.eof()) {
          break;
        }
        m[key] = value;
        std::cout << "new value is added!" << std::endl;
        std::cout << "map[" << key << "] = " << m[key] << std::endl;
      } else if (command == "SEARCH") {
        std::string key;
        std::cout << "Key: " << std::flush;
        std::cin >> key;
        if (!std::cin || std::cin.eof()) {
          std::cout << "map[" << key << "] = " << m[key] << std::endl;
        }
        try {
        } catch (std::out_of_range& e) {
        }
      } else if (command == "SHOW") {
        for (iterator it = m.begin(); it != m.end(); ++it) {
          std::cout << "map[" << (*it).first << "] = " << (*it).second
                    << std::endl;
        }
      }
    }
  }
}