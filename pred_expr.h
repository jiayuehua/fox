//
// Copyright (c) 2017 by Daniel Saks and Stephen C. Dewhurst.
//
//
// Copyright (c) 2017 by Jiayuehua
//
// Permission to use, copy, modify, distribute, and sell this
// software for any purpose is hereby granted without fee, provided
// that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice
// appear in supporting documentation.
// The authors make no representation about the suitability of this
// software for any purpose. It is provided "as is" without express
// or implied warranty.
//

#ifndef INCLUDED_PRED_EXPR
#define INCLUDED_PRED_EXPR

#include <type_traits>
#include <cstddef>

#include <type_traits>
#include <boost/hana/traits.hpp>
#include "detect.h"

// namespace hana = boost::hana;
//
// Facility to create a compile-time abstract syntax tree (AST) that
// represents a compound predicate composed of class template predicates.
// Each predicate takes a typename argument and produce a compile time
// boolean result when specialized.
//
// Can be used to make "poor man's concepts" less impoverished.
//
namespace boost
{
namespace hana
{
namespace traits
{
inline namespace ASTImplementation
{
//
// Convenience template for generating predicates.
//
template <bool c>
using truthiness = std::conditional_t<c, std::true_type, std::false_type>;

//
// Quick check to see if a series of zero or more bools all evaluate to true.
//
template <bool... bs>
struct AllTrue;

template <bool b, bool... bs>
struct AllTrue<b, bs...> {
  enum : bool { value = b && AllTrue<bs...>::value };
};

template <>
struct AllTrue<> {
  enum : bool { value = true };
};

//
// See if each Ts is derived from Base.
//
template <typename Base, typename... Ts>
struct all_derived_from;

template <typename Base, typename F, typename... Rs>
struct all_derived_from<Base, F, Rs...> {
  enum : bool { value = std::is_base_of<Base, F>::value && all_derived_from<Base, Rs...>::value };
};

template <typename Base>
struct all_derived_from<Base> {
  enum : bool { value = true };
};

//
// Every AST node is-a "Predicate AST"
//
struct PAST {
};

//
// Check that all are derived from PAST.
//
// template <typename... Ts>
// using AllPAst = all_derived_from<PAST, Ts...>;

template <class T>
struct IsTraits {
};
template <template <class... T> class TC>
struct IsTraits<detail::hana_trait<TC>> {
  typedef int type;
};
template <class T>
using TraitsType = typename IsTraits<std::remove_cv_t<T>>::type;

}  // namespace ASTImplementation

template <class T>
constexpr bool isTraits = is_detected<TraitsType, T>::value;
template <typename... T>
using AllPAst = AllTrue<(isTraits<T> || std::is_base_of<PAST, T>::value)...>;
//
// The language consists of type predicate leaves
// the binary operators &, |, and ^ (and, or, and exclusive or),
// and ! (negation).
//
// We use the compiler's parser to parse the expressions so (of course)
// the precedence and associativity match those of C++.
//
// We leverage the compiler's symbol table to build our compile-time
// ASTs and evaluate them with compile-time template arguments.
//
// This facility produces no runtime executable code.

//====================
// AST leaf nodes:
//     a) ids that wrap a predicate template that can be specilaized with one typename
//     b) binders that bind an argument of a binary predicate to produce an id

// An identifier type that wraps a predicate template.  The template
// takes a typename and passes judgement on it at compile time.
//
// Note use of variadic template template to allow predicates that take trailing defaults.
//
template <template <typename...> class Pred>
struct Id : PAST {
  template <typename T>
  static constexpr bool eval()
  {
    return Pred<T>::value;
  }
};
template <template <typename...> class Pred>
constexpr auto pred()
{
  return Id<Pred>();
}

// Binders that take a binary template and either a type
// or a std::size_t argument and produce something that can be
// used as an Id.
//
// Bind the first argument of a binary predicate that takes [typename, typename].
//
template <template <typename, typename> class BPred, typename First>
struct BindT1st : PAST {
  template <typename Second>
  static constexpr bool eval()
  {
    return BPred<First, Second>::value;
  }
};
template <template <typename, typename> class BPred, typename First>
constexpr auto bind1st()
{
  return BindT1st<BPred, First>();
}

// Bind the second argument of a of a binary predicate that takes [typename, typename].
//
template <template <typename, typename> class BPred, typename Second>
struct BindT2nd : PAST {
  template <typename First>
  static constexpr bool eval()
  {
    return BPred<First, Second>::value;
  }
};
template <template <typename, typename> class BPred, typename Second>
constexpr auto bind2nd()
{
  return BindT2nd<BPred, Second>();
}

// Bind the first argument of a binary predicate that takes [std::size_t, typename].
//
template <template <std::size_t, typename> class BPred, std::size_t first>
struct BindI1st : PAST {
  template <typename Second>
  static constexpr bool eval()
  {
    return BPred<first, Second>::value;
  }
};
template <template <std::size_t, typename> class BPred, std::size_t first>
constexpr auto bind1st()
{
  return BindI1st<BPred, first>();
}

// Bind the second argument of a of a binary predicate that takes [typename, std::size_t].
//
template <template <typename, std::size_t> class BPred, std::size_t second>
struct BindI2nd : PAST {
  template <typename First>
  static constexpr bool eval()
  {
    return BPred<First, second>::value;
  }
};
template <template <typename, std::size_t> class BPred, std::size_t second>
constexpr auto bind2nd()
{
  return BindI2nd<BPred, second>();
}

// Operators used to build the compile-time AST.
// We overload the bitwise operators &, |, and ^ rather than
// && and || so that we can have an ^ operation and also so as
// not to imply that the operators will short-circuit.
// We use ! for negation rather than ~, however.
//
namespace detail {
struct Base{};

template <typename P1, typename P2>
struct And : PAST {
  template<class T>
  constexpr bool operator()(T t)const
  {
    P1 p1 = {};
    P2 p2 = {};
    return p1(t)&p2(t);
  }
};
template <typename P1, typename P2, typename = AllPAst<P1, P2>>
constexpr auto operator&(P1, P2)
{
  And<P1, P2> a = {};
  return a;
}

template <typename P1, typename P2>
struct Or : PAST {
  template<class T>
  constexpr bool operator()(T t)const
  {
    P1 p1 = {};
    P2 p2 = {};
    return p1(t)|p2(t);
  }
};
template <typename P1, typename P2, typename = AllPAst<P1, P2>>
constexpr auto operator|(P1, P2)
{
  Or<P1, P2> a = {};
  return a;
}
template <typename P1, typename P2>
struct Xor : PAST {
  template<class T>
  constexpr bool operator()(T t)const
  {
    P1 p1 = {};
    P2 p2 = {};
    return p1(t)^p2(t);
  }
};
template <typename P1, typename P2, typename = AllPAst<P1, P2>>
constexpr auto operator^(P1, P2)
{
  Xor<P1, P2> a = {};
  return a;
}
template <typename P>
struct Not : PAST {
  template<class T, typename = AllPAst<P> >
  constexpr bool operator()(T t)const
  {
    P p = {};
    return p(t)?0:1;
  }
};
template <typename P, typename = AllPAst<P>>
constexpr auto operator!(P)
{
  return Not<P>();
}
}


// Another way to evaluate an AST object.  In addition to
//     ast.eval<T>()
// this provides the syntax
//     satisfies<T>(ast)
//
// Can also check multiple arguments against the constraint:
//    satisfies<T1, T2, T3>(ast)
//
template <typename... Ts, typename AST, typename = AllPAst<AST>>
constexpr bool satisfies(AST)
{
  return AllTrue<AST()(hana::type_c<Ts>)...>::value;
}

// For convenience with SFINAE applications.
// Uses an AST type:
//    Constraint<AST, T>
// checks whether T satisfies the constraints described by AST.
// The result is either void or represents no type.
//
// Can test multiple arguments against the constraint:
//    Constraint<AST, T1, T2, T3>
//
template <typename AST, typename... Ts>
using Constraint = std::enable_if_t<satisfies<Ts...>(AST()), void **>;

// A bool SFINAE constraint.
// Uses an AST object rather than a type.  Same syntax as "satisfies,"
// but result is either true or produces no result.
// Implementation of constraint inspired by example in
// http://en.cppreference.com/w/cpp/language/sfinae
//
template <typename... Ts, typename AST, bool test = AllTrue<AST()(hana::type_c<Ts>)...>::value>
constexpr bool constraint(AST, char (*)[test == true] = nullptr)
{
  return true;
}

//
// Handy but optional names to use as "trailing distinguishers" for
// overloaded member templates that are otherwise identical.
//
template <std::size_t>
struct Ignore {
};
constexpr bool ignore(std::size_t) { return true; }
}  // namespace traits
}  // namespace hana
}  // namespace boost
#endif
