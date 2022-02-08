#ifndef TEST_UTILS_STUDENT_H_
#define TEST_UTILS_STUDENT_H_

#include <stdint.h>

#include <string>

#include "hash.hpp"
#include "string.hpp"

namespace ft {
namespace test {

struct Student {
  typedef std::string name_type;
  typedef uint8_t age_type;
  typedef uint64_t id_type;

  std::string name_;
  uint8_t age_;
  uint64_t id_;

  class Compare {
   public:
    bool operator()(const Student& lhs, const Student& rhs) const {
      return lhs.id_ < rhs.id_;
    }
  };

  Student()
      : name_(generate_random_string(12)), age_(0), id_(generate_hash(name_)) {}

  Student(const std::string& name, const uint8_t age)
      : name_(name), age_(age), id_(generate_hash(name)) {}

  Student(const Student& other)
      : name_(other.name_), age_(other.age_), id_(other.id_) {}

  void operator=(const Student& other) {
    if (this != &other) {
      name_ = other.name_;
      age_ = other.age_;
      id_ = other.id_;
    }
  }

  ~Student() {}

 private:
};

}  // namespace test
}  // namespace ft

#endif