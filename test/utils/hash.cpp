#include "hash.hpp"

#include <cstdint>
#include <string>

namespace {

const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ul;
const uint64_t FNV_PRIME = 1099511628211ul;

}  // namespace

namespace ft {
namespace test {

uint64_t generate_hash(const std::string &str) {
  uint64_t hash = FNV_OFFSET_BASIS;

  for (uint64_t i = 0; i < str.size(); ++i) {
    hash = hash ^ static_cast<uint64_t>(str[i]);
    hash = hash * FNV_PRIME;
  }
  return hash;
}

}  // namespace test
}  // namespace ft