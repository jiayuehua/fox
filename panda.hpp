#include <forward_list>
template <class Container, class Iter>
Iter eraseViaSwap(Container& c, Iter i) noexcept
{
  if (i== c.end())
  {
    return i;
  }
  auto e = c.end();
  --e;
  std::iter_swap(i, e);
  c.pop_back();
  return i;
}

template <class Container, class Iter>
Iter erase(Container& c, Iter i)
{
  if (i== c.end())
  {
    return i;
  }
  *i = c.back();
  c.pop_back();
  return i;
}

template <class T>
std::size_t size(const T & t)noexcept
{
  return t.size();
}

template <class U>
std::size_t size(const std::forward_list<U> & fla)noexcept
{
  return std::distance(fla.begin(), fla.end());
}
