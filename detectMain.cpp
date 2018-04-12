#include <iostream>
#include <type_traits>
#include <boost/hana/traits.hpp>
#include "pred_expr.h"
namespace hana = boost::hana;
namespace traits = hana::traits;
struct Pod{

};
  constexpr auto pc = hana::traits::is_pod & hana::traits::is_class;
  constexpr auto pf = hana::traits::is_signed & hana::traits::is_fundamental;
template<class T, traits::Constraint<decltype(pc), T> = nullptr>
void f(T)
{
  std::cout<<2<<std::endl;
}
template<class T, traits::Constraint<decltype(pf), T> = nullptr>
void f(T)
{
  std::cout<<1<<std::endl;
}
template<class T, bool = traits::constraint(pc)>
void f2(T)
{
  std::cout<<3<<std::endl;
}
struct CA{};
int main()
{
  Pod pod;
  f(pod);
  auto i = 100;
  f(i);
  f2(pod);
  return 0;
  //hana::traits::S s;
  //constexpr auto tb = s&s;
  //constexpr auto r = hana::traits::b & hana::traits::b;
  constexpr auto t = hana::traits::is_pod & hana::traits::is_class;
  constexpr auto o = hana::traits::is_pod | hana::traits::is_fundamental;
  constexpr auto xor_ = hana::traits::is_class ^ hana::traits::is_fundamental;
  constexpr auto notclass_ = !hana::traits::is_class ;
  static_assert(hana::traits::is_pod(hana::type_c<Pod>), "");
  static_assert(t(hana::type_c<Pod>), "");
  static_assert(o(hana::type_c<int>), "");
  static_assert(xor_(hana::type_c<int>), "");
  static_assert(xor_(hana::type_c<CA>), "");
  static_assert(notclass_(hana::type_c<int>), "");
  //constexpr auto tb = hana::traits::isvoid & hana::traits::isvoid;
  std::cout<<traits::AllPAst<decltype(hana::traits::is_pod),decltype(hana::traits::is_class) >::value<<std::endl;
  //std::cout<<traits::isTraits<decltype(hana::traits::is_pod)><<std::endl;
  std::cout<<traits::isTraits<const hana::traits::detail::hana_trait<std::is_pod>><<std::endl;
  //static_assert(traits::isTraits<decltype(hana::traits::is_pod)>,"");
}
