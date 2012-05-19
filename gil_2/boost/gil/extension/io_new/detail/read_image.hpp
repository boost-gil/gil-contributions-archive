/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_READ_IMAGE_HPP
#define BOOST_GIL_EXTENSION_IO_READ_IMAGE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/toolbox/dynamic_images.hpp>

#include "base.hpp"
#include "io_device.hpp"
#include "path_spec.hpp"
#include "conversion_policies.hpp"

namespace boost{ namespace gil {

/// \ingroup IO

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param reader    An image reader.
/// \param img       The image in which the data is read into. Must satisfy is_read_supported metafunction.
/// \throw std::ios_base::failure
template < typename Reader
         , typename Image
         >
inline
void read_image( Reader& reader
               , Image&  img
               )
{
    reader.init_image( img
                     , reader._settings
                     );

    reader.apply( view( img ));
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file      It's a device. Must satisfy is_input_device metafunction.
/// \param img       The image in which the data is read into. Must satisfy is_read_supported metafunction.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename Device
         , typename Image
         , typename FormatTag
         >
inline
void read_image( Device&                                 file
               , Image&                                  img
               , const image_read_settings< FormatTag >& settings
               )
{
    typedef get_reader< Device
                      , FormatTag
                      , detail::read_and_no_convert
                      >::type reader_t;

    reader_t reader = make_reader( file
                                 , settings
                                 , detail::read_and_no_convert()
                                 );

    read_image( reader
              , img
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file      It's a device. Must satisfy is_input_device metafunction.
/// \param img       The image in which the data is read into. Must satisfy is_read_supported metafunction.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename Device
         , typename Image
         , typename FormatTag
         >
inline
void read_image( Device&         file
               , Image&           img
               , const FormatTag& tag
               )
{
    typedef get_reader< Device
                      , FormatTag
                      , detail::read_and_no_convert
                      >::type reader_t;

    reader_t reader = make_reader( file
                                 , tag
                                 , detail::read_and_no_convert()
                                 );

    read_image( reader
              , img
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param img       The image in which the data is read into. Must satisfy is_read_supported metafunction.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename String
         , typename Image
         , typename FormatTag
         >
inline
void read_image( const String&                           file_name
               , Image&                                  img
               , const image_read_settings< FormatTag >& settings
               )
{
    typedef get_reader< String
                      , FormatTag
                      , detail::read_and_no_convert
                      >::type reader_t;

    reader_t reader = make_reader( file_name
                                 , settings
                                 , detail::read_and_no_convert()
                                 );

    read_image( reader
              , img
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param img       The image in which the data is read into. Must satisfy is_read_supported metafunction.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename String
         , typename Image
         , typename FormatTag
         >
inline
void read_image( const String&   file_name
               , Image&           img
               , const FormatTag& tag
               )
{
    typedef get_reader< String
                      , FormatTag
                      , detail::read_and_no_convert
                      >::type reader_t;

    reader_t reader = make_reader( file_name
                                 , tag
                                 , detail::read_and_no_convert()
                                 );

    read_image( reader
              , img
              );
}

///

template < typename Reader
         , typename Images
         >
inline
void read_image( Reader&              reader
               , any_image< Images >& images
               )
{
    reader.apply( images );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file      It's a device. Must satisfy is_adaptable_input_device metafunction.
/// \param images    Dynamic image ( mpl::vector ). See boost::gil::dynamic_image extension.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename Device
         , typename Images
         , typename FormatTag
         >
inline
void read_image( Device&                                 file
               , any_image< Images >&                    images
               , const image_read_settings< FormatTag >& settings
               )
{
    typedef get_dynamic_image_reader< Device
                                    , FormatTag
                                    >::type reader_t;

    reader_t reader = make_dynamic_image_reader( file
                                               , settings
                                               );

    read_image( reader
              , images
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file      It's a device. Must satisfy is_adaptable_input_device metafunction.
/// \param images    Dynamic image ( mpl::vector ). See boost::gil::dynamic_image extension.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename Device
         , typename Images
         , typename FormatTag
         >
inline
void read_image( Device&             file
               , any_image< Images >& images
               , const FormatTag&     tag
               )
{
    typedef get_dynamic_image_reader< Device
                                    , FormatTag
                                    >::type reader_t;

    reader_t reader = make_dynamic_image_reader( file
                                               , tag
                                               );

    read_image( reader
              , images
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param images    Dynamic image ( mpl::vector ). See boost::gil::dynamic_image extension.
/// \param settings  Specifies read settings depending on the image format.
/// \throw std::ios_base::failure
template < typename String
         , typename Images
         , typename FormatTag
         >
inline
void read_image( const String&                           file_name
               , any_image< Images >&                    images
               , const image_read_settings< FormatTag >& settings
               )
{
    typedef get_dynamic_image_reader< String
                                    , FormatTag
                                    >::type reader_t;

    reader_t reader = make_dynamic_image_reader( file_name
                                               , settings
                                               );

    read_image( reader
              , images
              );
}

/// \brief Reads an image without conversion. Image memory is allocated.
/// \param file_name File name. Must satisfy is_supported_path_spec metafunction.
/// \param images    Dynamic image ( mpl::vector ). See boost::gil::dynamic_image extension.
/// \param tag       Defines the image format. Must satisfy is_format_tag metafunction. 
/// \throw std::ios_base::failure
template < typename String
         , typename Images
         , typename FormatTag
         >
inline
void read_image( const String&       file_name
               , any_image< Images >& images
               , const FormatTag&     tag
               )
{
    typedef get_dynamic_image_reader< String
                                    , FormatTag
                                    >::type reader_t;

    reader_t reader = make_dynamic_image_reader( file_name
                                               , tag
                                               );

    read_image( reader
              , images
              );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_READ_IMAGE_HPP
