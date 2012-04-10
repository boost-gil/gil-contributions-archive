#pragma once

namespace boost { namespace gil {

/// Input iterator to read images.
template< typename Reader
        , typename View
        >
class image_read_iterator
{
public:

    typedef typename Reader::backend_t backend_t;

    typedef std::input_iterator_tag iterator_category;
    typedef View value_type;
    typedef View const* pointer;
    typedef View const& reference;
    
    /// todo
    //typedef ptrdiff_t difference_type;

public:

    /// Default Constructor, usually used to create an end iterator.
    image_read_iterator()
    : _pos( 0 )
    , _reader( NULL )
    , _view( NULL )
    {}

    /// Constructor with preallocated image. Reading starts at first scanline of source image.
    image_read_iterator( Reader&     reader
                       , const View& view
                       )
    : _pos( 0 )
    , _reader( &reader )
    , _view( &view )
    {
        init();
    }

    /// Constructor with preallocated image. Reading starts at first scanline of source image.
    image_read_iterator( Reader& reader )
    : _pos( 0 )
    , _reader( &reader )
    , _view( 0 )
    {
        init();
    }

    /// Constructor with preallocated image. Reading starts at pos scanline of source image.
    image_read_iterator( std::size_t pos
                       , Reader&     reader
                       , const View& view
                       )
    : _pos( pos )
    , _reader( &reader )
    , _view( &view )
    {
        init();

        for( std::size_t i = 0; i < pos; ++i )
        {
            skip();
        }
    }

    ///@todo Add constructors that will allocate dst image memory.

    /// Set reader. Do clean up before if necessary.
    void set_reader(Reader& reader)
    {
        if( _reader )
        {
            _reader.clean_up();

            _pos = 0;
        }

        _reader = &reader;

        init();
    }

    /// Set reader. Do clean up before if necessary.
    void set_view(const View& view)
    {
        _view = &view;

        if( _reader && _view )
        {
            _reader->check_destination_view( *_view );
        }
    }


    /// Dereference Operator
    reference operator*() const
    {
        if( _reader && _view )
        {
            _reader->read( *_view, _pos );

            ++_pos;

            return *_view;
        }

        throw std::runtime_error( "Reader cannot be null for this operation." );
    }

    /// Pointer-To-Memper Operator.
    pointer operator->() const
    {
        return &(operator*());
    }

    /// Pre-Increment Operator
    image_read_iterator< Reader, View >& operator++()
    {
        skip();

        return (*this);
    }

    /// Compare passed iterator to this.
    bool equal( const image_read_iterator< Reader, View >& rhs ) const
    {
        return (_reader == rhs._reader) && ( _view == rhs._view );
    }

    /// Return backend.
    typename const backend_t& backend()
    {
        if( _reader )
        {
            return *_reader;
        }

        throw std::runtime_error( "Reader cannot be null for this operation." );        
    }

    /// Return length of scanline in bytes.
    int scanline_length()
    {
        if(_reader)
        {
            return _reader->scanline_length();
        }
    }

private:

    void init()
    {
        if( _reader )
        {
            _reader->read_header();
            _reader->initialize();
        }

        if( _reader && _view )
        {
            _reader->check_destination_view( *_view );
        }
    }

    void skip()
    {
        if( _reader && _view )
        {
            _reader->skip( *_view );

            ++_pos;
        }
    }

private:

    mutable int _pos;

    Reader* _reader;
    const View*   _view;
};

} // namespace gil
} // namespace boost
