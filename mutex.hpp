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

    class SharedWay
  {
  };
  class NotShared :public SharedWay {

  };
  class Shared :public SharedWay {

  };
  class TimedWay
  {
  };
  class NotTimed :public TimedWay {

  };
  class Timed :public TimedWay {

  };
  class RecursiveWay
  {
  };
  class NotRecursive :public RecursiveWay {

  };
  class Recursive :public RecursiveWay {

  };
  template<class T, class U, class V>
  class Mutex;
  template<>
  class Mutex<NotRecursive, NotTimed, NotShared> : public std::mutex {
  };

  template<>
  class Mutex<Recursive, NotTimed, NotShared> : public std::recursive_mutex {
  };

  template<>
  class Mutex<NotRecursive, Timed, NotShared> : public std::timed_mutex {
  };
  template<>
  class Mutex<Recursive, Timed, NotShared> : public std::recursive_timed_mutex {
  };
  //template<>
  //class Mutex<NotRecursive, NotTimed, Shared> : public std::shared_mutex {
  //};
  //template<>
  //class Mutex<NotRecursive, Timed, Shared> : public std::shared_timed_mutex {
  //};
  namespace parameter = boost::parameter;
  typedef parameter::parameters<
    parameter::optional<parameter::deduced<tag::RecursiveType>, boost::is_base_and_derived<RecursiveWay, _>>
    , parameter::optional<parameter::deduced<tag::TimedType>, boost::is_base_and_derived<TimedWay, _>>
    , parameter::optional<parameter::deduced<tag::SharedType>, boost::is_base_and_derived<SharedWay, _>>
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
      args, tag::RecursiveType, NotRecursive >::type RecursiveType;

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
    using GeneralMutex = typename MyMutexImpl<A0, A1, A2>::MutexType;
}
