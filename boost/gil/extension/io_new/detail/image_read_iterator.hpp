#pragma once

namespace boost { namespace gil {

/// Input iterator to read images.
template< typename Reader
        , typename View
        >
class image_read_iterator
{
public:

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
    /// The supplied view ( y dimension ) defines how many scanlines will be read at each time.
    /// The supplied view ( x dimension ) how much of a scanline needs to be read.
    image_read_iterator( Reader&     reader
                       , const View& view
                       )
    : _pos( 0 )
    , _reader( &reader )
    , _view( &view )
    {
        reader.read_header();
        reader.check_destination_view( *_view );
    }

    /// Constructor with preallocated image. Reading starts at pos scanline of source image.
    /// The supplied view ( y dimension ) defines how many scanlines will be read at each time.
    /// The supplied view ( x dimension ) how much of a scanline needs to be read.
    image_read_iterator( std::size_t pos
                       , Reader&     reader
                       , const View& view
                       )
    : _pos( pos )
    , _reader( &reader )
    , _view( &view )
    {
        reader.read_header();
        reader.check_destination_view();
        reader.initialize();

        for( std::size_t i = 0; i < pos; ++i )
        {
            skip();
        }
    }

    ///@todo Add constructors that will allocate dst image memory.

    /// Dereference Operator
    reference operator*() const
    {
        if( _reader && _view )
        {
            return _reader->read( *_view );
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

        return (*this)
    }

    /// Compare passed iterator to this.
    bool equal( const image_read_iterator< Reader, View >& rhs ) const
    {
        return (_reader == rhs._reader) && ( _view == rhs._view );
    }

private:

    void skip()
    {
        if( _reader && _view )
        {
            _reader->skip( *_view );
        }
    }

private:

    std::size_t _pos;

    Reader* _reader;
    const View*   _view;
};

} // namespace gil
} // namespace boost
