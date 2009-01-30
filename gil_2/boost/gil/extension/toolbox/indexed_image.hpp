// Copyright 2009 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_TOOLBOX_INDEXED_IMAGE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_TOOLBOX_INDEXED_IMAGE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Indexed Image extension
/// \author Christian Henning \n
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {

typedef boost::gil::point2< std::ptrdiff_t > point_t;

// used for virtual locator
template< typename IndicesLoc
        , typename PaletteLoc
        >
struct indexed_image_deref_fn
{
    typedef IndicesLoc indices_locator_t;
    typedef PaletteLoc palette_locator_t;

    typedef indexed_image_deref_fn          const_t;
    typedef typename PaletteLoc::value_type value_type;
    typedef value_type                      reference;
    typedef value_type                      const_reference;
    typedef point_t                         argument_type;
    typedef reference                       result_type;

    result_type operator()( const point_t& p ) const
    {
        return value_type();
    }
};

template< typename IndicesLoc
        , typename PaletteLoc
        >
struct indexed_image_locator_type
{
    typedef virtual_2d_locator< indexed_image_deref_fn< IndicesLoc
                                                      , PaletteLoc
                                                      >
                              , false
                              > type;
};

template< typename Locator > // indexed_image_locator_type< ... >::type
class indexed_image_view
{
public:

    typedef typename Locator::deref_fn_t::indices_locator_t indices_locator_t;
    typedef typename Locator::deref_fn_t::palette_locator_t palette_locator_t;

    typedef indexed_image_view< Locator > const_t;

    indexed_image_view()
    : _dimensions( 0, 0 )
    , _num_colors( 0 )
    {}

    indexed_image_view( const point_t&              dimensions
                      , std::size_t                 num_colors
                      , const indices_locator_t& indices
                      , const palette_locator_t& palette
                      )
    : _dimensions( dimensions )
    , _num_colors( num_colors )
    , _indices( indices )
    , _palette( palette )
    {}

    template< typename IndexedView >
    indexed_image_view( const IndexedView& iv )
    : _dimensions( iv._dimensions )
    , _num_colors( iv._num_colors )
    , _indices( iv._indices )
    , _palette( iv._palette )
    {}

    const point_t&    dimensions() { return _dimensions; }
    const std::size_t num_colors() { return _num_colors; }

    const indices_locator_t& indices() { return _indices; }
    const palette_locator_t& palette() { return _palette; }

private:

    template< typename Locator2 > friend class indexed_image_view;

    point_t     _dimensions;
    std::size_t _num_colors;

    indices_locator_t _indices;
    palette_locator_t _palette;
};


template< typename Index
        , typename Pixel
        >
class indexed_image
{
public:

    typedef image< Index, false, std::allocator< unsigned char > > indices_t;
    typedef image< Pixel, false, std::allocator< unsigned char > > palette_t;

    typedef typename indices_t::view_t indices_view_t;
    typedef typename palette_t::view_t palette_view_t;

    typedef typename indices_t::const_view_t indices_const_view_t;
    typedef typename palette_t::const_view_t palette_const_view_t;

    typedef typename indices_view_t::locator indices_locator_t;
    typedef typename palette_view_t::locator palette_locator_t;

    typedef typename indexed_image_locator_type< indices_locator_t
                                               , palette_locator_t
                                               >::type locator_t;

    typedef indexed_image_view< locator_t > view_t;
    typedef typename view_t::const_t        const_view_t;

    indexed_image() {}

    indexed_image( const point_t& dimensions
                 , std::size_t    num_colors
                 )
    : _indices( dimensions    )
    , _palette( num_colors, 1 )
    {
        typedef typename indices_view_t::locator indices_loc_t;
        typedef typename palette_view_t::locator palette_loc_t;

        std::size_t indices_row_size = get_row_size_in_memunits< indices_view_t >( dimensions.x );
        std::size_t palette_row_size = get_row_size_in_memunits< palette_view_t >( num_colors   );

        _view = view_t( dimensions
                      , num_colors
                      , indices_loc_t( view( _indices ).xy_at( 0, 0 ), indices_row_size )
                      , palette_loc_t( view( _palette ).xy_at( 0, 0 ), palette_row_size )
                      );
    }

    indexed_image( const indexed_image& img )
    : _indices( img._indices )
    , _palette( img._palette )
    {}

    template <typename Pixel2, typename Index2>
    indexed_image( const indexed_image< Pixel2, Index2 >& img )
    {
        copy_pixels( img.get_indices_const_view(), get_indices_view() );
        copy_pixels( img.get_palette_view(), get_palette_view() );
    }

    indexed_image& operator= ( const indexed_image& img )
    {
        _indices = img._indices;
        _palette = img._palette;

        return *this;
    }

    indices_const_view_t get_indices_const_view() const { return const_view( _indices ); }
    palette_const_view_t get_palette_const_view() const { return const_view( _palette ); }

    indices_view_t get_indices_view() { return view( _indices ); }
    palette_view_t get_palette_view() { return view( _palette ); }

private:

    // Doesn't work if there is an alignment at the end of a row.
    template< typename View >
    std::size_t get_row_size_in_memunits( std::size_t width ) const
    {   
		return width * memunit_step( typename View::x_iterator() );
    }

public:

    view_t _view;

private:

    indices_t _indices;
    palette_t _palette;
};

template< typename Index
        , typename Pixel
        >
inline 
const typename indexed_image< Index, Pixel >::view_t& view( indexed_image< Index, Pixel >& img )
{
    return img._view;
}

template< typename Index
        , typename Pixel
        >
inline 
const typename indexed_image< Index, Pixel >::const_view_t const_view( indexed_image< Index, Pixel >& img )
{
    return static_cast< const typename indexed_image< Index, Pixel >::const_view_t>( img._view );
}

} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_TOOLBOX_INDEXED_IMAGE_HPP_INCLUDED
