
#ifndef BOOST_GIL_EXTENSION_IO_TIFF_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_TIFF_HPP_INCLUDED 

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/detail/tiff_io.hpp>
#include <boost/gil/extension/io_new_2/detail/tiff_supported_types.hpp>

namespace boost{ namespace gil {

template<typename PixelType>
struct is_supported<PixelType,tiff_tag>
    : mpl::bool_<detail::tiff_rw_support<
                    typename channel_type<PixelType>::type,
                    typename color_space_type<PixelType>::type
                    >::is_supported>
{};

}}

#endif // BOOST_GIL_EXTENSION_IO_TIFF_HPP_INCLUDED
