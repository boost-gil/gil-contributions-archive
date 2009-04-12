/*
    Copyright 2007-2008 Andreas Pokorny, Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Andreas Pokorny, Christian Henning \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

namespace boost { namespace gil { namespace detail {

class tiff_device_base
{
public:

    tiff_device_base()
    {}

    tiff_device_base( TIFF* tiff_file )
    : _tiff_file( tiff_file
                , TIFFClose )
    {}


    template <typename Property>
    bool get_property( typename Property::type& value  )
    {
        if( TIFFGetFieldDefaulted( _tiff_file.get()
                                 , Property::tag
                                 , &value ) == 1 )
        {
            return true;
        }

        return false;
    }
   

    template <typename Property>
    inline
    bool set_property( const typename Property::type& value )
    {
       if( TIFFSetField( _tiff_file.get()
                       , Property::tag
                       , value  ) == 1 )
       {
          return true;
       }

       return false;
    }

    unsigned int get_default_strip_size()
    {
        return TIFFDefaultStripSize( _tiff_file.get()
                                   , 0 );
    }

    template< typename Buffer >
    inline 
    void write_scaline( Buffer&     buffer
                      , uint32      row
                      , tsample_t   plane
                      )
    {
       io_error_if( TIFFWriteScanline( _tiff_file.get()
                                     , &buffer.front()
                                     , row
                                     , plane 
                                     ) == -1
                   , "Write error"
                   );
    }

protected:

   typedef boost::shared_ptr<TIFF> tiff_file_t;
   tiff_file_t _tiff_file;

};

/*!
 *
 * file_stream_device specialization for tiff images, which are based on TIFF*.
 */
template<>
class file_stream_device< tiff_tag > : public tiff_device_base
{
public:

    struct read_tag {};
    struct write_tag {};

    file_stream_device( std::string const& file_name, read_tag )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "r" )) == NULL
                   , "file_stream_device: failed to open file" );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

    file_stream_device( std::string const& file_name, write_tag )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
                   , "file_stream_device: failed to open file" );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

    file_stream_device( TIFF* tiff_file )
    : tiff_device_base( tiff_file )
    {}

    template< typename Buffer >
    void read_scaline( Buffer&        buffer
                     , std::ptrdiff_t row
                     , tsample_t      plane   )
    {
        io_error_if( TIFFReadScanline( _tiff_file.get()
                                     , reinterpret_cast< tdata_t >( &buffer.front() )
                                     , (uint32) row
                                     , plane           ) == -1
                   , "Read error."
                   );
    }

   bool are_bytes_swapped()
   {
      return ( TIFFIsByteSwapped( _tiff_file.get() )) ? true : false;
   }

   std::size_t get_scanline_size()
   {
      return TIFFScanlineSize( _tiff_file.get() );
   }


   int get_field_defaulted( boost::uint16_t*& red
                          , boost::uint16_t*& green
                          , boost::uint16_t*& blue  )
   {
      return TIFFGetFieldDefaulted( _tiff_file.get()
                                  , TIFFTAG_COLORMAP
                                  , &red
                                  , &green
                                  , &blue              );
   }

};

/*!
 *
 * ostream_device specialization for tiff images.
 */
template<>
class ostream_device< tiff_tag > : public tiff_device_base
{
public:
    ostream_device( std::ostream & out )
    : _out( out )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFClientOpen( "dummy"
                                            , "w"
                                            , &_out
                                            , ostream_device< tiff_tag >::read_proc
                                            , ostream_device< tiff_tag >::write_proc
                                            , ostream_device< tiff_tag >::seek_proc
                                            , ostream_device< tiff_tag >::close_proc
                                            , ostream_device< tiff_tag >::size_proc
                                            , NULL
                                            , NULL
                                            )
                     ) == NULL
                   , "ostream_device: failed to open file"
                   );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

private:

    static std::ostream* get_ostream( thandle_t handle )
    {
        return reinterpret_cast< std::ostream* >( handle );
    }

    static tsize_t read_proc( thandle_t fd
                            , tdata_t   buf
                            , tsize_t   size
                            )
    {
        return 0;
    }

    static tsize_t write_proc( thandle_t handle
                             , tdata_t   buf
                             , tsize_t   size
                             )
    {
        std::cout << "write: " << size << std::endl;

        std::ostream* os = get_ostream( handle );

        io_error_if( os->write( reinterpret_cast<char const*>( buf )
                              , static_cast<std::streamsize>( size )
                              ).bad() == true
                   , "ostream_device: Cannot write."
                   );

        assert( os->good() );

        return size;
    }

    static toff_t seek_proc( thandle_t handle
                           , toff_t    off
                           , int       way
                           )
    {
/*
        os->seekp( off
                 , i == SEEK_SET
                 ? std::ios::beg
                 : ( i == SEEK_CUR
                     ? std::ios::cur
                     : std::ios::end )
                  );
*/
        return off;
    }

    static int close_proc( thandle_t handle )
    {
        std::ostream* os = get_ostream( handle );

        os->operator<< ( std::flush );

        return 0;
    }

    static toff_t size_proc( thandle_t handle )
    {
        return 0;
    }

private:

    std::ostream& _out;
};



/*
template< typename T, typename D >
struct is_adaptable_input_device< tiff_tag, T, D > : mpl::false_{};
*/

template< typename FormatTag >
struct is_adaptable_input_device< FormatTag
                                , TIFF*
                                , void
                                >
    : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};

template< typename FormatTag >
struct is_adaptable_output_device< FormatTag
                                 , TIFF*
                                 , void
                                 >
    : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED
