#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

namespace ft {
/* remove_cv_t */
template <class T>
struct remove_cv {
  typedef T type;
};
template <class T>
struct remove_cv<const T> {
  typedef T type;
};
template <class T>
struct remove_cv<volatile T> {
  typedef T type;
};
template <class T>
struct remove_cv<const volatile T> {
  typedef T type;
};
template <class T>
struct remove_const {
  typedef T type;
};
template <class T>
struct remove_const<const T> {
  typedef T type;
};
template <class T>
struct remove_volatile {
  typedef T type;
};
template <class T>
struct remove_volatile<volatile T> {
  typedef T type;
};

template <typename T, T Val>
struct integral_constant {
  typedef integral_constant type;
  typedef T value_type;
  enum { value = Val };
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

/* is_integral
テンプレートの完全特殊化と SFINAE を用いて実現する
*/
template <typename T>
struct is_integral_base : public false_type {};

template <>
struct is_integral_base<bool> : public true_type {
  typedef bool value;
};
template <>
struct is_integral_base<char> : public true_type {
  typedef char value;
};
template <>
struct is_integral_base<char16_t> : public true_type {
  typedef char16_t value;
};
template <>
struct is_integral_base<char32_t> : public true_type {
  typedef char32_t value;
};
template <>
struct is_integral_base<wchar_t> : public true_type {
  typedef wchar_t value;
};
template <>
struct is_integral_base<short> : public true_type {
  typedef short value;
};
template <>
struct is_integral_base<unsigned short> : public true_type {
  typedef short value;
};
template <>
struct is_integral_base<int> : public true_type {
  typedef int value;
};
template <>
struct is_integral_base<unsigned int> : public true_type {
  typedef int value;
};
template <>
struct is_integral_base<long> : public true_type {
  typedef long value;
};
template <>
struct is_integral_base<unsigned long> : public true_type {
  typedef long value;
};
template <>
struct is_integral_base<long long> : public true_type {
  typedef long long value;
};
template <>
struct is_integral_base<unsigned long long> : public true_type {
  typedef long long value;
};

template <typename T>
struct is_integral
    : public is_integral_base<typename remove_cv<T>::type>::type {};

/* enable_if */
/*
整数のみしか受け付けないようなベクターは以下のように定義できる
template <class T,
  typename std::enable_if<std::is_integral<T>::value,T>::type* = nullptr>
class IntOnlyVector {};

enable_ifのテンプレート第一引数がtrueになれば
  テンプレートから実体が生成されるのでオーバーロードの候補となる.
逆にテンプレート第一引数がfalseの場合はテンプレートの実体化に失敗し,
  オーバーロードの候補とはならない.
*/
template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

}  // namespace ft

#endif