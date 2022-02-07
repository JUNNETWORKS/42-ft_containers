#include "hash.hpp"

#include <cstdint>
#include <string>

namespace {

const std::uint64_t FNV_OFFSET_BASIS = 14695981039346656037ul;
const std::uint64_t FNV_PRIME = 1099511628211ul;

}  // namespace

namespace ft {
namespace test {

std::uint64_t generate_hash(const std::string &str) {
  std::uint64_t hash = FNV_OFFSET_BASIS;

  for (std::uint64_t i = 0; i < str.size(); ++i) {
    hash = hash ^ static_cast<std::uint64_t>(str[i]);
    hash = hash * FNV_PRIME;
  }
  return hash;
}

}  // namespace test
}  // namespace ft