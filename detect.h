#pragma once
#include <type_traits>

struct nonesuch {
  nonesuch()                 = delete;
  ~nonesuch()                = delete;
  nonesuch(nonesuch const &) = delete;
  void operator=(nonesuch const &) = delete;
};

template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
struct DETECTOR {
  using value_t = std::false_type;
  using type    = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct DETECTOR<Default, std::void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
  using type    = Op<Args...>;
};

template <template <class...> class Op, class... Args>
using is_detected = typename DETECTOR<nonesuch, void, Op, Args...>::value_t;

template <template <class...> class Op, class... Args>
using detected_t = typename DETECTOR<nonesuch, void, Op, Args...>::type;

template <class Default, template <class...> class Op, class... Args>
using detected_or = DETECTOR<Default, void, Op, Args...>;

template <class Expected, template <class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

template <class To, template <class...> class Op, class... Args>
using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;
