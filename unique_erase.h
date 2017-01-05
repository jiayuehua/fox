// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

// Fox library
//
//  Copyright Jiayuehua 2017. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef BOOST_RANGE_ALGORITHM_EXT_UNIQUE_ERASE_HPP_INCLUDED
#define BOOST_RANGE_ALGORITHM_EXT_UNIQUE_ERASE_HPP_INCLUDED

#include <boost/range/config.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/difference_type.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/assert.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace boost
{
  namespace range
  {

    template< class Container>
    inline Container& unique_erase(Container& on)
    {
      BOOST_RANGE_CONCEPT_ASSERT((ForwardRangeConcept<Container>));
      on.erase(
        std::unique(boost::begin(on), boost::end(on)),
        boost::end(on));
      return on;
    }

    template< class Container, class Pred >
    inline Container& unique_erase_if(Container& on, Pred pred)
    {
      BOOST_RANGE_CONCEPT_ASSERT((ForwardRangeConcept<Container>));
      on.erase(
        std::unique(boost::begin(on), boost::end(on), pred),
        boost::end(on));
      return on;
    }

  } // namespace range
  using range::unique_erase;
  using range::unique_erase_if;
} // namespace boost

#endif // include guard
