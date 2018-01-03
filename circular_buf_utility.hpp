#include <boost/circular_buffer.hpp>
namespace boost
{
  template<class T, class A>
  bool notFull(const boost::circular_buffer<T, A> & c)
  {
    return !c.full();
  }
  template<class T, class A>
  bool notEmpty(const boost::circular_buffer<T, A> & c)
  {
    return !c.empty();
  }
}
