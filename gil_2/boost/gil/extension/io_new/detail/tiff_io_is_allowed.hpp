/*
    Copyright 2008 Christian Henning, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_IO_IS_ALLOWED_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_TIFF_IO_IS_ALLOWED_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

namespace boost { namespace gil { namespace detail {

template< typename Channel >
int format_value( boost::mpl::true_ ) // is_bit_aligned
{
    return SAMPLEFORMAT_UINT;
}

template< typename Channel >
int format_value( boost::mpl::false_ ) // is_bit_aligned
{
    if( is_unsigned< Channel >::value )
    {
        return SAMPLEFORMAT_UINT;
    }
    if( is_signed< Channel >::value )
    {
        return SAMPLEFORMAT_INT;
    }
    else if( is_floating_point< Channel >::value )
    {
        return SAMPLEFORMAT_IEEEFP;
    }

    throw std::runtime_error( "Unkown channel format." );

    return 0;
}

// The following two functions look the same but are different since one is using
// a pixel_t as template parameter whereas the other is using reference_t.
template< typename View >
bool compare_channel_sizes( const std::vector< unsigned int >& channel_sizes // in bits
                          , mpl::false_                                      // is_bit_aligned
                          , mpl::true_                                       // is_homogeneous
                          ) 
{
    typedef typename View::value_type pixel_t;
    typedef typename channel_traits< element_type< pixel_t >::type >::value_type channel_t;

    unsigned int s = boost::gil::detail::unsigned_integral_num_bits< channel_t >::value;

    return ( s == channel_sizes[0] );
}


template< typename View >
bool compare_channel_sizes( const std::vector< unsigned int >& channel_sizes // in bits
                          , mpl::true_                                       // is_bit_aligned
                          , mpl::true_                                       // is_homogeneous
                          ) 
{
    typedef typename View::reference ref_t;

    typedef channel_traits< element_type< ref_t >::type >::value_type channel_t;
    channel_t c;

    unsigned int s = boost::gil::detail::unsigned_integral_num_bits< channel_t >::value;

    return ( s == channel_sizes[0] );
}

struct compare_channel_sizes_fn
{
    compare_channel_sizes_fn( unsigned int* a )
    : _a( a )
    , _b( true )
    {}

    template< typename ChannelSize >
    void operator()( ChannelSize x)
    {
        if( x != *_a++ )
        {
            _b = false;
        }
    }

    unsigned int* _a;
    bool _b;
};

template< typename View >
bool compare_channel_sizes( std::vector< unsigned int >& channel_sizes // in bits
                          , mpl::true_                                 // is_bit_aligned
                          , mpl::false_                                // is_homogeneous
                          )
{
    // loop through all channels and compare

    typedef typename View::reference ref_t;
    typedef channel_sizes_type< ref_t >::type cs_t;

    compare_channel_sizes_fn fn( &channel_sizes.front() );
    mpl::for_each< cs_t >( fn );

    return fn._b;
}

template< typename View >
bool is_allowed( unsigned int                       src_n // num channels
               , const std::vector< unsigned int >& src_s // array of channel sizes
               , unsigned int                       src_f // channel format
               , boost::mpl::true_                        // is read_and_no_convert
               )
{
    typedef typename View::value_type pixel_t;
    typedef typename View::reference  ref_t;
    typedef typename channel_traits< element_type< pixel_t >::type >::value_type channel_t;

    int dst_n = num_channels< pixel_t >::value;
    int dst_f = format_value< channel_t >( typename is_bit_aligned< pixel_t >::type() );

    bool s = compare_channel_sizes< View >( src_s
                                          , typename is_bit_aligned< ref_t >::type()
                                          , typename is_homogeneous< ref_t >::type() );

    return (  dst_n == src_n
           && s
           && dst_f == src_f
           );
}

template< typename View >
bool is_allowed( unsigned int                       src_n // num channels
               , const std::vector< unsigned int >& src_s // array of channel sizes
               , unsigned int                       src_f // channel format
               , boost::mpl::false_                       // is read_and_convert
               )
{
    return true;
}

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_IO_IS_ALLOWED_HPP_INCLUDED
