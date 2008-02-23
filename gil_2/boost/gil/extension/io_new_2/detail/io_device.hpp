/*
  Copyright 2007-2008 Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_IO_DEVICE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_IO_DEVICE_HPP_INCLUDED

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

#include <boost/shared_ptr.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/gil/extension/io_new_2/detail/base.hpp>

namespace boost { namespace gil { namespace detail {
/*!
 * Implements the IODevice concept c.f. to \ref IODevice required by Image libraries like
 * libjpeg and libpng. 
 *
 * \todo switch to a sane interface as soon as there is 
 * something good in boost. I.E. the IOChains library
 * would fit very well here. 
 *
 * This implementation is based on FILE*.
 */
template< typename FormatTag >
class file_stream_device
{
public:

   typedef FormatTag _tag_t;

public:
    struct read_tag {};
    struct write_tag {};
    file_stream_device( std::string const& file_name, read_tag )
        : file(0),_close(true)
    {
        io_error_if( ( file = fopen( file_name.c_str(), "rb" )) == NULL
                , "file_stream_device: failed to open file" );
    }

    file_stream_device( std::string const& file_name, write_tag )
        : file(0),_close(true)
    {
        io_error_if( ( file = fopen( file_name.c_str(), "wb" )) == NULL
                , "file_stream_device: failed to open file" );
    }

    file_stream_device( FILE * filep)
        : file(filep),_close(false)
    {
    }

    ~file_stream_device()
    {
        if(_close)
            fclose(file);
    }

    size_t read( unsigned char * data, int count )
    {
        return fread(data, 1, count, file );
    }

    void write( unsigned char const * data, int count )
    {
        fwrite(data, 1, count, file);
    }

    //!\todo replace with std::ios::seekdir?
    void seek( long count, int whence )
    {
        fseek(file, count, whence );
    }


    void flush()
    {
        fflush(file);
    }
private:
    file_stream_device( file_stream_device const& );
    file_stream_device& operator=( file_stream_device const& );
    FILE* file;
    bool _close;
};

/*!
 *
 * This implementation is based on TIFF*.
 */
template<>
class file_stream_device< tiff_tag >
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

    unsigned int get_default_strip_size()
    {
        return TIFFDefaultStripSize( _tiff_file.get()
                                   , 0 );
    }

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

    template< typename Buffer >
    void read_scaline( Buffer&        buffer
                     , std::ptrdiff_t row
                     , tsample_t      plane   )
    {
      io_error_if( TIFFReadScanline( _tiff_file.get()
                                   , &buffer.front()
                                   , (uint32) row
                                   , plane           ) == -1
                  , "Read error."                             );
   }

   bool are_bytes_swapped()
   {
      TIFFIsByteSwapped( _tiff_file.get() );
   }

   std::size_t get_scanline_size()
   {
      return TIFFScanlineSize( _tiff_file.get() );
   }


   int get_field_defaulted( boost::uint16_t* red
                          , boost::uint16_t* green
                          , boost::uint16_t* blue  )
   {
      return TIFFGetFieldDefaulted( _tiff_file.get()
                                  , TIFFTAG_COLORMAP
                                  , &red
                                  , &green
                                  , &blue              );
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


private:

   typedef boost::shared_ptr<TIFF> tiff_file_t;
   tiff_file_t _tiff_file;
};

/**
 * Input stream device
 */
class istream_device
{
public:
    istream_device( std::istream & in )
        : in(in)
    {
    }

    size_t read( unsigned char * data, size_t count )
    {
        size_t cr = 0;
        do{
            size_t c = in.readsome( reinterpret_cast<char*>( data )
                                  , static_cast<std::streamsize>( count ));

            count -= c;
            data += c;
            cr += c;
        } while(count && in );
        return cr;
    }

    void seek( long count, int whence )
    {
        in.seekg(
                count,
                whence == SEEK_SET
                ?std::ios::beg
                :(whence == SEEK_CUR
                    ?std::ios::cur
                    :std::ios::end)
                );
    }

    void write( unsigned char const* data, int count )
    {
        //throw?
    }

    void flush() {}

private:
    std::istream& in;
};

/**
 * Output stream device
 */
class ostream_device
{
public:
    ostream_device( std::ostream & out )
        : out(out)
    {
    }

    size_t read( unsigned char * data, int count )
    {
        //throw?
    }

    void seek( long count, int whence )
    {
        out.seekp(
                count,
                whence == SEEK_SET
                ?std::ios::beg
                :(whence == SEEK_CUR
                    ?std::ios::cur
                    :std::ios::end)
                );
    }

    void write( unsigned char const* data, int count )
    {
        out.write(reinterpret_cast<char const*>(data), count);
    }

    void flush()
    {
        out << std::flush;
    }

private:
    std::ostream& out;
};


/**
 * Metafunction to detect input devices. 
 * Should be replaced by an external facility in the future.
 */
template< typename IODevice> struct is_input_device : mpl::false_{};
template<typename FormatTag> struct is_input_device<file_stream_device< FormatTag > > : mpl::true_{};
template<> struct is_input_device<istream_device> : mpl::true_{};

template< typename FormatTag, typename T, typename D = void >
struct is_adaptable_input_device : mpl::false_{};

template<typename FormatTag, typename T>
struct is_adaptable_input_device<FormatTag, T,
    typename enable_if<is_base_and_derived<std::istream,T> >::type
    >
  : mpl::true_
{
    typedef istream_device device_type;
};

template<typename FormatTag>
struct is_adaptable_input_device<FormatTag,FILE*,void> : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};



/**
 * Metafunction to detect output devices. 
 * Should be replaced by an external facility in the future.
 */
template<typename IODevice> struct is_output_device : mpl::false_{};
template<typename FormatTag> struct is_output_device<file_stream_device< FormatTag > > : mpl::true_{};
template<> struct is_output_device<ostream_device> : mpl::true_{};

template<typename FormatTag, typename IODevice,typename D=void> struct is_adaptable_output_device : mpl::false_{};
template<typename FormatTag, typename T> struct is_adaptable_output_device<FormatTag, T,
    typename enable_if<is_base_and_derived<std::ostream,T> >::type
    >
  : mpl::true_
{
    typedef ostream_device device_type;
};

template<typename FormatTag> struct is_adaptable_output_device<FormatTag,FILE*,void>
  : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};


} // ns detail
} // ns gil
} // ns boost

#endif

