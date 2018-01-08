#include <boost/assert.hpp>
#include <cstdio>
namespace boost
{
void assertion_failed(char const* expr, char const* func, char const* file, long line)
{
  fprintf(stderr, "Assertion failed: %s, file %s, function %s, line %ld\n", expr, file, func, line);
}
}  // namespace boost
