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

/* is_integral */
template <typename>
struct is_integral_base {};

/* enable_if */
/*
整数のみしか受け付けないようなベクターは以下のように定義できる
template <class T,
  typename std::enable_if<std::is_integral<T>::value,T>::type* = nullptr>
class IntOnlyVector<T> {};

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