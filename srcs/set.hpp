#ifndef MAP_H_
#define MAP_H_

template <typename _Tp>
struct _Identity : public unary_function<_Tp, _Tp> {
  /// @c argument_type is the type of the argument
  typedef _Tp argument_type;

  /// @c result_type is the return type
  typedef _Tp result_type;

  _Tp& operator()(_Tp& __x) const {
    return __x;
  }

  const _Tp& operator()(const _Tp& __x) const {
    return __x;
  }
};

#endif