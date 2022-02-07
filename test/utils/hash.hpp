#ifndef TEST_UTILS_HASH_H_
#define TEST_UTILS_HASH_H_

#include <cstdint>
#include <string>

namespace ft {
namespace test {

// get hash of std::string by FNV-1a hash
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
std::uint64_t generate_hash(const std::string &str);

}  // namespace test
}  // namespace ft

#endif