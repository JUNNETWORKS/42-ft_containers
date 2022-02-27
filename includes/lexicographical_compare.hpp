#ifndef LEXICOGRAPHICAL_COMPARE_H_
#define LEXICOGRAPHICAL_COMPARE_H_

namespace ft {
/*
Return true if the first range is lexicographically less than the second.


Lexicographical comparison is a operation with the following properties:

Two ranges are compared element by element.
The first mismatching element defines which range is lexicographically less or
greater than the other. If one range is a prefix of another, the shorter range
is lexicographically less than the other. If two ranges have equivalent elements
and are of the same length, then the ranges are lexicographically equal. An
empty range is lexicographically less than any non-empty range. Two empty ranges
are lexicographically equal.
*/
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  // どちらかのシーケンスの横断が先に終わる場合(つまり、範囲の長さが合わない場合)、先に終わった方が小さいと判断される。
  return (first1 == last1) && (first2 != last2);
}

template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return (first1 == last1) && (first2 != last2);
}
}  // namespace ft

#endif