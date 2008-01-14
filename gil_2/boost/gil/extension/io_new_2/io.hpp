/*
  Copyright 2007-2008 Christian Henning, Andreas Pokorny
  Distributed under the MIT License (see accompanying file_name LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_IO_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_IO_HPP_INCLUDED 

/*! 
 * \page iobackend Adding a new io backend
 * \section Overview of backend requirements
 * To add support for a new IO backend the following is required:
 *  - a format tag, to identify the image format, derived from boost::gil::format_tag
 *  - boolean meta function is_supported<PixelType,FormatTag> must be implemented for 
 *    the new format tag
 *  - explicit specialisation of image_read_info<FormatTag> must be provided, containing
 *    runtime information available before/at reading the image
 *  - explicit specialisation of image_write_info<FormatTag> must be provided, containing 
 *    runtime encoding parameters for writing an image 
 *  - An image reader must be specialized:
 *    \code
 *      template<typename IODevice, typename ConversionPolicy>
 *      struct boost::gil::detail::reader<IODevice,FormatTag,ConversionPolicy>
 *      {
 *        reader( IODevice & device )
 *        reader( IODevice & device, typename ConversionPolicy::color_converter_type const& cc )
 *        image_read_info<FormatTag> get_info(); 
 *        template<typename Image>
 *        void read_image( Image &, point_t const& top_left );
 *        template<typename View>
 *        void read_view( View &, point_t const& top_left );
 *      };
 *    \endcode
 *  - An image writer must be specialized:
 *    \code
 *      \template <typename IODevice>
 *      struct boost::gil::detail::writer<IODevice,FormatTag>
 *      {
 *        writer( IODevice & device )
 *        template<typename View>
 *        void apply( View const&, point_t const& top_left );
 *        template<typename View>
 *        void apply( View const&, point_t const& top_left, image_write_info<FormatTag> const& );
 *      };
 *    \endcode
 *
 * Or instead of the items above implement overloads of read_view, read_and_convert_view, read_image, 
 * read_and_convert_image, write_view and read_image_info.
 *
 * \section ConversionPolicy Interface of the ConversionPolicy 
 * There are two different conversion policies in use, when reading images:
 * read_and_convert<ColorConverter> and read_and_no_convert. ColorConverter 
 * can be a user defined color converter. 
 *  
 * \code 
 * struct ConversionPolicy
 * {
 *    template<typename InputIterator,typename OutputIterator>
 *    void read( InputIterator in_begin, InputIterator in_end, 
 *          OutputIterator out_end );
 * };
 * \endcode
 *
 * Methods like read_view and read_image are supposed to bail out with an 
 * exception instead of converting the image 
 *
 * \section IODevice Concept of IO Device
 * A Device is simply an object used to read and write data to and from a stream. 
 * The IODevice was added as a template paramter to be able to replace the file_name
 * access functionality. This is only an interim solution, as soon as boost provides
 * a good IO library, interfaces/constraints provided by that library could be used.
 *
 * \code
 *  concept IODevice
 *  {
 *      void IODevice::read( unsigned char* data, int count );
 *      void IODevice::write( unsigned char* data, int count );
 *      void IODevice::seek(long count, int whence);
 *      void IODevice::flush();
 *  };
 * \endcode
 *
 * For the time being a boolean meta function must be specialized:
 * \code 
 * namespace boost{namespace gil{namespace detail{
 *  template<typename Device> 
 *  struct detail::is_input_device; 
 * }}}
 * \endcode
 * 
 */

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/gil/extension/io_new_2/detail/io_device.hpp>
#include <boost/gil/extension/io_new_2/detail/base.hpp>
#include <boost/gil/extension/io_new_2/detail/path_spec.hpp>
#include <boost/gil/extension/io_new_2/detail/conversion_policies.hpp>

namespace boost{ namespace gil {

namespace detail {
    template<typename Device, typename FormatTag, typename ConversionPolicy>
    struct reader;
    template<typename Device, typename FormatTag>
    struct writer;
}

template<typename FormatTag>
struct is_format_tag : is_base_and_derived<format_tag,FormatTag> {};

template<typename FormatTag>
struct image_read_info;

template<typename FormatTag>
struct image_write_info;

/// \ingroup IO
/// \brief Returns the image info for generating a gil image type.
template<typename Device, typename FormatTag>
inline 
image_read_info<FormatTag>
read_image_info( Device & file, FormatTag const& tag, 
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_input_device<Device>::type
        >::type>::type * ptr = 0
        )
{
    return detail::reader<Device,FormatTag,detail::read_and_no_convert>( file ).get_info();
}

template<typename Device, typename FormatTag>
inline 
image_read_info<FormatTag>
read_image_info( Device & file, FormatTag const& tag, 
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_adaptable_input_device<Device>::type
        >::type>::type * ptr = 0
        )
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(file);
    return detail::reader<device_type,FormatTag,detail::read_and_no_convert>( file ).get_info();
}

template<typename String,typename FormatTag>
inline 
image_read_info<FormatTag>  
read_image_info( String const& file_name, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_supported_path_spec<String>::type
        >::type>::type * ptr = 0
        )
{
    detail::file_stream_device reader( 
            detail::convert_to_string(file_name), 
            detail::file_stream_device::read_tag()
            );
    return read_image_info( reader, tag);
}

// ------ Image --- Reader -------- no conversion -----------

/// \ingroup IO
template <typename Device, typename Image, typename FormatTag> 
inline void read_image( 
        Device & file, Image& img, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type 
        >::type>::type * ptr = 0
        )
{
    detail::reader<Device,FormatTag,detail::read_and_no_convert> reader(file);
    reader.read_image( img, top_left );
}

template <typename Device, typename Image, typename FormatTag> 
inline void read_image( 
        Device & file, Image& img, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type 
        >::type>::type * ptr = 0
        )
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(file);
    detail::reader<device_type,FormatTag,detail::read_and_no_convert> reader(dev);
    reader.read_image( img, top_left );
}



template <typename String,typename Image, typename FormatTag> 
inline void read_image( 
        String const& file_name, Image& img, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    detail::file_stream_device device( 
            detail::convert_to_string(file_name), 
            detail::file_stream_device::read_tag()
            );
    read_image( device, img, top_left, tag );
}

/// \ingroup IO
template <typename Device, typename Image, typename FormatTag> 
inline
void read_image( 
        Device& file, Image& img, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_input_device<Device>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 
        )
{
    read_image( file, img, point_t( 0, 0 ), tag );
}

/// \ingroup IO
template <typename Device, typename Image, typename FormatTag> 
inline
void read_image( 
        Device& file, Image& img, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_input_device<Device>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 
        )
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(file);
    read_image( dev, img, point_t( 0, 0 ), tag );
}

/// \ingroup IO
template <typename String, typename Image,typename FormatTag> 
inline
void read_image( 
        String const& file_name,
        Image& img, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename Image::value_type, FormatTag>::type 
        >::type>::type * ptr = 0 
        )
{
    detail::file_stream_device device( 
            detail::convert_to_string(file_name), 
            detail::file_stream_device::read_tag()
            );
    read_image( device, img, point_t( 0, 0 ), tag );
}

// ------ View --- Reader -------- no conversion -----------

/// \ingroup PNG_I
template<typename Device, typename View, typename FormatTag > 
inline void read_view( 
        Device & file, const View& view, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_input_device<Device>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0 
        )
{
    detail::reader<Device,FormatTag,detail::read_and_no_convert> reader(file);
    reader.read_view( view, top_left );
}

template<typename Device, typename View, typename FormatTag > 
inline void read_view( 
        Device & file, const View& view, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_adaptable_input_device<Device>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0 
        )
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(file);
    detail::reader<device_type,FormatTag,detail::read_and_no_convert> reader(dev);
    reader.read_view( view, top_left );
}

template<typename String,typename View, typename FormatTag> 
inline void read_view( 
        String const& file_name, const View& view, point_t top_left, 
        FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_supported_path_spec<String>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    detail::file_stream_device device(
            detail::convert_to_string(file_name), 
            detail::file_stream_device::read_tag()
            );
    read_view( device, view, top_left, tag);
}

/// \ingroup IO
/// \brief Loads the image specified by the \a file_name into the given \a view.
template<typename String,typename View, typename FormatTag> 
inline void read_view( 
        String const& file_name, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_supported_path_spec<String>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    read_view( file_name, view, point_t(0,0), tag);
}

template<typename Device,typename View, typename FormatTag> 
inline void read_view( 
        Device & device, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
            >::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    read_view( device, view, point_t(0,0), tag);
}

// ------ Image --- Reader ------- With ColorConverter -----------------

/// \ingroup IO
template< typename Device, typename Image, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_image( Device & file, Image& img, const point_t& top_left, ColorConverter const& cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 
        )
{
    typedef detail::read_and_convert< ColorConverter > reader_color_convert;

    detail::reader<Device,FormatTag,reader_color_convert> reader(file, cc);

    reader.read_image( img, top_left );
}

template< typename Device, typename Image, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_image( Device & file, Image& img, const point_t& top_left, ColorConverter const& cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type
            >::type>::type * ptr1 = 0 
        )
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(file);
    read_and_convert_image( dev, img, top_left, cc, tag );
}

template<typename String,typename Image, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_image( String const& file_name, Image& img, const point_t& top_left, ColorConverter const& cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 )
{
    detail::file_stream_device device(
            detail::convert_to_string(file_name),
            detail::file_stream_device::read_tag()
            );
    read_and_convert_image( device, img, top_left, cc, tag );
}

/// \ingroup IO
template<typename String,typename Image, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_image(
        String const& file_name, Image& img, 
        ColorConverter const& cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
            >::type>::type * ptr1 = 0 
        )
{
    read_and_convert_image( file_name, img, point_t( 0, 0 ), cc, tag );
}

template<typename Device,typename Image, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_image(
        Device & device, Image& img, 
        ColorConverter const& cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
            >::type>::type * ptr1 = 0 
        )
{
    read_and_convert_image( device, img, point_t( 0, 0 ), cc, tag );
}

/// \ingroup IO
template <typename String, typename Image, typename FormatTag>
inline
void read_and_convert_image( String const& file_name, Image& img, const point_t& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 )
{
   read_and_convert_image( file_name, img, top_left, default_color_converter(), tag );
}

template <typename Device, typename Image, typename FormatTag>
inline
void read_and_convert_image( Device& device, Image& img, const point_t& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 )
{
   read_and_convert_image( device, img, top_left, default_color_converter(), tag );
}

/// \ingroup IO
template <typename String, typename Image, typename FormatTag>
inline
void read_and_convert_image( String const& file_name, Image& img, FormatTag const & tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
            >::type>::type * ptr1 = 0 )
{
   read_and_convert_image( file_name, img, point_t( 0, 0 ), default_color_converter(), tag );
}

template <typename Device, typename Image, typename FormatTag>
inline
void read_and_convert_image( Device & device, Image& img, FormatTag const & tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
            >::type>::type * ptr1 = 0 )
{
   read_and_convert_image( device, img, point_t( 0, 0 ), default_color_converter(), tag );
}

// ----- View --- Reader --------- With ColorConverter -----------------

/// \ingroup IO
template<typename Device, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( Device& device, const View& view, const point_t& top_left, ColorConverter cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    typedef detail::read_and_convert< ColorConverter > reader_color_convert;

    detail::reader<Device,FormatTag,reader_color_convert> reader(device,cc);

    reader.read_view( view, top_left );
}

template<typename Device, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( Device& device, const View& view, const point_t& top_left, ColorConverter cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_input_device<Device>::type,
            typename is_format_tag<FormatTag>::type
         >::type>::type * ptr1 = 0)
{
    typedef typename detail::is_adaptable_input_device<Device>::device_type device_type;
    device_type dev(device);
    read_and_convert_view( dev, view, top_left, cc, tag );
}

template<typename String, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( String const& file_name, const View& view, const point_t& top_left, ColorConverter cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    detail::file_stream_device device( 
            detail::convert_to_string(file_name), 
            detail::file_stream_device::read_tag() 
            );
    read_and_convert_view( device, view, top_left, cc, tag );
}

template<typename Device, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( Device& device, const View& view, ColorConverter cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    read_and_convert_view( device, view, point_t(0,0), cc, tag );
}

template<typename String, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( String const& file_name, const View& view, ColorConverter cc, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    read_and_convert_view( file_name, view, point_t(0,0), cc, tag );
}

/// \ingroup IO
template<typename Device, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( Device& device, const View& view, const point_t& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    read_and_convert_view( device, view, top_left, default_color_converter(), tag );
}

template< typename String, typename View, typename FormatTag>
inline
void read_and_convert_view( String const& file_name, const View& view, const point_t& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0 )
{
   read_and_convert_view( file_name, view, top_left, default_color_converter(), tag );
}

/// \ingroup IO
template<typename Device, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( Device& device, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_input_device<Device>::type,
                typename detail::is_adaptable_input_device<Device>::type
                >::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    read_and_convert_view( device, view, point_t(0,0), default_color_converter(), tag );
}

template<typename String, typename View, typename ColorConverter, typename FormatTag>
inline
void read_and_convert_view( String const& file_name, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type
        >::type>::type * ptr1 = 0)
{
    read_and_convert_view( file_name, view, point_t(0,0), default_color_converter(), tag );
}


//---- Writer -------------------------------

/// \ingroup IO
template<typename Device, typename View, typename FormatTag>
inline
void write_view( Device & device, View const& view, point_t const& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_output_device<Device>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
   typedef detail::is_bit_aligned< typename View::value_type >::type tt;

    detail::writer<Device,FormatTag> writer(device);
    writer.apply( view, top_left, tt() );
}

template<typename Device, typename View, typename FormatTag>
inline
void write_view( Device & device, View const& view, point_t const& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_output_device<Device>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    typename detail::is_adaptable_output_device<Device>::device_type dev(device);
    write_view( dev, view, top_left );
}

template<typename String, typename View, typename FormatTag>
inline
void write_view( String const& file_name, View const& view, point_t const& top_left, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
    detail::file_stream_device device( 
            detail::convert_to_string(file_name), 
            detail::file_stream_device::write_tag()
            );
    write_view( device, view, top_left, tag );
}

/// \ingroup IO
template< typename Device, typename View, typename FormatTag > 
inline
void write_view( Device & device, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename mpl::or_<
                typename detail::is_output_device<Device>::type,
                typename detail::is_adaptable_output_device<Device>::type
            >::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
   write_view( device, view, point_t( 0, 0 ), tag );
}

template<typename String,typename View, typename FormatTag > 
inline
void write_view( String const& file_name, const View& view, FormatTag const& tag,
        typename enable_if<typename mpl::and_<
            typename detail::is_supported_path_spec<String>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr = 0
        )
{
   write_view( file_name, view, point_t( 0, 0 ), tag );
}

/// \ingroup IO
template< typename Device,typename View, typename FormatTag> 
inline
void write_view( Device & device, const View& view, const point_t& top_left, 
        const image_write_info<FormatTag>& info, 
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_output_device<Device>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr2 = 0 
        )
{
    detail::writer<Device,FormatTag> writer(device);
    writer.apply( view, top_left, info );
}

template< typename Device,typename View, typename FormatTag> 
inline
void write_view( Device & device, const View& view, const point_t& top_left, 
        const image_write_info<FormatTag>& info, 
        typename enable_if<typename mpl::and_<
            typename detail::is_adaptable_output_device<Device>::type,
            typename is_format_tag<FormatTag>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr2 = 0 
        )
{
    typename detail::is_adaptable_output_device<Device>::device_type dev(device);
    write_view(dev, view, top_left, info);
}

template<typename String,typename View, typename FormatTag> 
inline
void write_view( String const& file_name, const View& view, const point_t& top_left, 
        const image_write_info<FormatTag>& info,
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_supported_path_spec<String>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr2 = 0 
        )
{
    detail::file_stream_device device(
            detail::convert_to_string(file_name),
            detail::file_stream_device::write_tag()
            );
    write_view( device, view, top_left, info );
}

/// \ingroup IO
template< typename String, typename View, typename FormatTag > 
inline
void write_view( String const& file_name, const View& view, const image_write_info<FormatTag>& info, 
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename detail::is_supported_path_spec<String>::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr2 = 0 
        )
{
   write_view( file_name, view, point_t( 0, 0 ), info );
}

template<typename Device, typename View, typename FormatTag > 
inline
void write_view( Device & device, const View& view, const image_write_info<FormatTag>& info, 
        typename enable_if<typename mpl::and_<
            typename is_format_tag<FormatTag>::type,
            typename mpl::or_<
                typename detail::is_output_device<Device>::type,
                typename detail::is_adaptable_output_device<Device>::type
            >::type,
            typename is_supported<typename View::value_type, FormatTag>::type
        >::type>::type * ptr2 = 0 
        )
{
   write_view( device, view, point_t( 0, 0 ), info );
}

}}

#endif

