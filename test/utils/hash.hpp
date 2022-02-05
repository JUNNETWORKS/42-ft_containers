#ifndef TEST_UTILS_HASH_H_
#define TEST_UTILS_HASH_H_

namespace {

const std::uint64_t FNV_OFFSET_BASIS = 14695981039346656037ul;
const std::uint64_t FNV_PRIME = 1099511628211ul;

}  // namespace

namespace ft {
namespace test {

// get hash of std::string by FNV-1a hash
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
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

#endif