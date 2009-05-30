/*
    Copyright 2009 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_IS_ALLOWED_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNM_IO_IS_ALLOWED_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

namespace boost { namespace gil { namespace detail {

template< typename View >
bool is_allowed( const image_read_info< pnm_tag >& info
               , mpl::true_   // is read_and_no_convert
               )
{
    /// @todo
    /*
    typedef typename channel_traits< typename View::value_type >::value_type channel_t;
    return detail::unsigned_integral_num_bits< channel_t >::value == info._bits_per_pixel;
    */

    return true;
}

template< typename View >
bool is_allowed( const image_read_info< pnm_tag >& info
               , mpl::false_  // is read_and_convert
               )
{
    return true;
}

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_IS_ALLOWED_HPP_INCLUDED
