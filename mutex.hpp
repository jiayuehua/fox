#pragma once
#include <mutex>
//#include <shared_mutex>
#include <boost/parameter.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/binding.hpp>
namespace fox {
  using boost::mpl::_;
  BOOST_PARAMETER_TEMPLATE_KEYWORD(RecursiveType)
  BOOST_PARAMETER_TEMPLATE_KEYWORD(TimedType)
  BOOST_PARAMETER_TEMPLATE_KEYWORD(SharedType)

  class Shared
  {
  };
  class NotShared:public Shared {

  };
  class CanShared:public Shared {

  };
  class Timed
  {
  };
  class NotTimed:public Timed {

  };
  class CanTimed:public Timed {

  };
  class Rec
  {
  };
  class NotRec:public Rec {

  };
  class CanRec :public Rec {

  };
  template<class T, class U, class V>
  class Mutex;
  template<>
  class Mutex<NotRec, NotTimed, NotShared> : public std::mutex {
  };

  template<>
  class Mutex<CanRec, NotTimed, NotShared> : public std::recursive_mutex {
  };

  template<>
  class Mutex<NotRec, Timed, NotShared> : public std::timed_mutex {
  };
  template<>
  class Mutex<CanRec, Timed, NotShared> : public std::recursive_timed_mutex {
  };
  //template<>
  //class Mutex<NotRec, NotTimed, Shared> : public std::shared_mutex {
  //};
  //template<>
  //class Mutex<NotRec, Timed, Shared> : public std::shared_timed_mutex {
  //};
  namespace parameter = boost::parameter;
  typedef parameter::parameters<
    parameter::optional<parameter::deduced<tag::RecursiveType>, boost::is_base_and_derived<Rec, _>>
    , parameter::optional<parameter::deduced<tag::TimedType>, boost::is_base_and_derived<Timed, _>>
    , parameter::optional<parameter::deduced<tag::SharedType>, boost::is_base_and_derived<Shared, _>>
  > class_signature;
  template <
    class A0 = parameter::void_
    , class A1 = parameter::void_
    , class A2 = parameter::void_
  >
    struct MyMutexImpl
  {
    typedef typename
      class_signature::bind<A0, A1, A2>::type
      args;

    // Extract first logical parameter.
    typedef typename parameter::value_type<
      args, tag::RecursiveType, NotRec >::type RecursiveType;

    typedef typename parameter::value_type<
      args, tag::TimedType, NotTimed >::type TimedType;

    typedef typename parameter::value_type<
      args, tag::SharedType, NotShared >::type SharedType;
    typedef Mutex<RecursiveType, TimedType, SharedType> MutexType;
  };

  template <
    class A0 = parameter::void_
    , class A1 = parameter::void_
    , class A2 = parameter::void_
  >
    using GereralMutex = typename MyMutexImpl<A0, A1, A2>::MutexType;
}
