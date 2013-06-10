#ifndef GIL_YUV_PLANAR_PIXEL_ITERATOR_HPP
#define GIL_YUV_PLANAR_PIXEL_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/gil/gil_config.hpp>
#include <boost/gil/utilities.hpp>
#include <boost/gil/pixel_iterator.hpp>
#include <boost/gil/pixel_iterator_adaptor.hpp>

namespace boost { 
namespace gil {


/// \defgroup ColorBaseModelPlanarPtr yuv_planar_pixel_iterator
/// \ingroup ColorBaseModel
/// \brief A homogeneous color base whose element is a channel iterator. Models HomogeneousColorBaseValueConcept
/// This class is used as an iterator to a planar yuv pixel.

/// \defgroup PixelIteratorModelPlanarPtr yuv_planar_pixel_iterator
/// \ingroup PixelIteratorModel
/// \brief An iterator over planar pixels. Models PixelIteratorConcept, HomogeneousPixelBasedConcept, MemoryBasedIteratorConcept, HasDynamicXStepTypeConcept

////////////////////////////////////////////////////////////////////////////////////////
/// \brief An iterator over yuv planar pixels. Models HomogeneousColorBaseConcept, PixelIteratorConcept, HomogeneousPixelBasedConcept, MemoryBasedIteratorConcept, HasDynamicXStepTypeConcept
///
/// YUV Planar pixels have channel data that is not consecutive in memory.
/// To abstract this we use classes to represent references and pointers to yuv planar pixels.
///
/// \ingroup PixelIteratorModelPlanarPtr ColorBaseModelPlanarPtr PixelBasedModel

template < typename ChannelPtr
         , typename ColorSpace
         >
struct yuv_planar_pixel_iterator : public iterator_facade< yuv_planar_pixel_iterator< ChannelPtr, ColorSpace >
                                                         , pixel< typename std::iterator_traits< ChannelPtr >::value_type
                                                                , layout< ColorSpace >
                                                                >
                                                         , std::random_access_iterator_tag
                                                         , const planar_pixel_reference< typename std::iterator_traits<ChannelPtr>::reference,ColorSpace>
                                                         >
                                 , public detail::homogeneous_color_base< ChannelPtr,layout<ColorSpace> 
                                                                        , mpl::size<ColorSpace>::value 
                                                                        >
{
private:

    typedef iterator_facade< yuv_planar_pixel_iterator< ChannelPtr,ColorSpace>
                           , pixel<typename std::iterator_traits<ChannelPtr>::value_type,layout<ColorSpace> >
                           , std::random_access_iterator_tag
                           , const planar_pixel_reference<typename std::iterator_traits<ChannelPtr>::reference,ColorSpace> 
                           > parent_t;

    typedef detail::homogeneous_color_base< ChannelPtr
                                          , layout< ColorSpace >, mpl::size< ColorSpace >::value
                                          >  color_base_parent_t;

    typedef typename std::iterator_traits< ChannelPtr >::value_type channel_t;

public:

    typedef typename parent_t::value_type      value_type;
    typedef typename parent_t::reference       reference;
    typedef typename parent_t::difference_type difference_type;

    // default constructor
    yuv_planar_pixel_iterator()
    : color_base_parent_t( 0 )
    {}

    // constructor    
    yuv_planar_pixel_iterator(bool) {}        // constructor that does not fill with zero (for performance)

    // constructor
    yuv_planar_pixel_iterator( const ChannelPtr& v0
                             , const ChannelPtr& v1
                             , const ChannelPtr& v2
                             )
    : color_base_parent_t( v0, v1, v2 )
    , _u_v_step( false )
    {}

    // copy constructor
    template <typename IC1, typename C1>
    yuv_planar_pixel_iterator( const yuv_planar_pixel_iterator< IC1, C1 >& ptr )
    : color_base_parent_t(ptr)
    , _u_v_step( false )
    {}

    /// Copy constructor and operator= from pointers to compatible planar pixels or planar pixel references.
    /// That allow constructs like pointer = &value or pointer = &reference
    /// Since we should not override operator& that's the best we can do.
    template <typename P>
    yuv_planar_pixel_iterator(P* pix)
    : color_base_parent_t(pix, true)
    , _u_v_step( false )
    {
        function_requires<PixelsCompatibleConcept<P,value_type> >();
    }

    struct address_of { template <typename T> T* operator()(T& t) { return &t; } };
    template <typename P>
    yuv_planar_pixel_iterator& operator=(P* pix)
    {
        function_requires<PixelsCompatibleConcept<P,value_type> >();
        static_transform(*pix,*this, address_of());

        // PERFORMANCE_CHECK: Compare to this:
        //this->template semantic_at_c<0>()=&pix->template semantic_at_c<0>();
        //this->template semantic_at_c<1>()=&pix->template semantic_at_c<1>();
        //this->template semantic_at_c<2>()=&pix->template semantic_at_c<2>();
        return *this;
    }

    /// For some reason operator[] provided by iterator_facade returns a custom class that is convertible to reference
    /// We require our own reference because it is registered in iterator_traits
    reference operator[](difference_type d)       const { return memunit_advanced_ref(*this,d*sizeof(channel_t));}

    reference operator->()                        const { return **this; }

    // PERFORMANCE_CHECK: Remove?
    bool operator< (const yuv_planar_pixel_iterator& ptr)   const { return gil::at_c<0>(*this)< gil::at_c<0>(ptr); }
    bool operator!=(const yuv_planar_pixel_iterator& ptr)   const { return gil::at_c<0>(*this)!=gil::at_c<0>(ptr); }
private:
    friend class boost::iterator_core_access;

    void increment()
    {
        at_c<0>( *this )++;

        if( _u_v_step )
        {
            at_c<1>( *this )++;
            at_c<2>( *this )++;
        }

        _u_v_step = !_u_v_step;
    }

    void decrement()
    {
        at_c<0>( *this )--;

        _u_v_step = !_u_v_step;

        if( _u_v_step )
        {
            at_c<1>( *this )--;
            at_c<2>( *this )--;
        }
    }

    void advance( std::ptrdiff_t d )
    {
        at_c<0>( *this ) += d;

        bool odd = d & 1;
        std::ptrdiff_t e = ( d >> 1 ) + ( _u_v_step & odd );    

        at_c<1>( *this ) += e;
        at_c<2>( *this ) += e;

        _u_v_step ^= odd;
    }

    reference dereference() const
    {
        return this->template deref<reference>();
    }

    std::ptrdiff_t distance_to(const yuv_planar_pixel_iterator& it) const
    {
        return gil::at_c<0>(it)-gil::at_c<0>(*this);
    }
    
    
    bool equal(const yuv_planar_pixel_iterator& it) const
    {
        return gil::at_c<0>(*this)==gil::at_c<0>(it);
    }

private:

    bool _u_v_step;
};

template <typename IC, typename C>
struct const_iterator_type<yuv_planar_pixel_iterator<IC,C> > {
private:
    typedef typename std::iterator_traits<IC>::value_type channel_t;
public:
    typedef yuv_planar_pixel_iterator<typename channel_traits<channel_t>::const_pointer,C> type;
};

// The default implementation when the iterator is a C pointer is to use the standard constness semantics
template <typename IC, typename C>
struct iterator_is_mutable<yuv_planar_pixel_iterator<IC,C> > : public detail::channel_iterator_is_mutable<IC> {};


/////////////////////////////
//  ColorBasedConcept
/////////////////////////////

template <typename IC, typename C, int K>
struct kth_element_type<yuv_planar_pixel_iterator<IC,C>, K> {
    typedef IC type;
};

template <typename IC, typename C, int K>
struct kth_element_reference_type<yuv_planar_pixel_iterator<IC,C>, K> : public add_reference<IC> {};

template <typename IC, typename C, int K>
struct kth_element_const_reference_type<yuv_planar_pixel_iterator<IC,C>, K> : public add_reference<typename add_const<IC>::type> {};

/////////////////////////////
//  HomogeneousPixelBasedConcept
/////////////////////////////

template <typename IC, typename C>
struct color_space_type<yuv_planar_pixel_iterator<IC,C> > {
    typedef C type;
};

template <typename IC, typename C>
struct channel_mapping_type<yuv_planar_pixel_iterator<IC,C> > : public channel_mapping_type<typename yuv_planar_pixel_iterator<IC,C>::value_type> {};

template <typename IC, typename C>
struct is_planar<yuv_planar_pixel_iterator<IC,C> > : public mpl::true_ {};

template <typename IC, typename C>
struct channel_type<yuv_planar_pixel_iterator<IC,C> > {
    typedef typename std::iterator_traits<IC>::value_type type;
};

/////////////////////////////
//  MemoryBasedIteratorConcept
/////////////////////////////

template <typename IC, typename C>
inline std::ptrdiff_t memunit_step(const yuv_planar_pixel_iterator<IC,C>&) { return sizeof(typename std::iterator_traits<IC>::value_type); }

template <typename IC, typename C>
inline std::ptrdiff_t memunit_distance(const yuv_planar_pixel_iterator<IC,C>& p1, const yuv_planar_pixel_iterator<IC,C>& p2) {
    return memunit_distance(gil::at_c<0>(p1),gil::at_c<0>(p2));
}

template <typename IC, typename C>
inline void memunit_advance(yuv_planar_pixel_iterator<IC,C>& p, std::ptrdiff_t diff) {
    static_transform(p, p, memunit_advance_fn<IC>(diff));
}

template <typename IC, typename C>
inline yuv_planar_pixel_iterator<IC,C> memunit_advanced(const yuv_planar_pixel_iterator<IC,C>& p, std::ptrdiff_t diff) {
    yuv_planar_pixel_iterator<IC,C> ret=p;
    memunit_advance(ret, diff);
    return ret;
}

template <typename ChannelPtr, typename ColorSpace>
inline planar_pixel_reference<typename std::iterator_traits<ChannelPtr>::reference,ColorSpace>
    memunit_advanced_ref(const yuv_planar_pixel_iterator<ChannelPtr,ColorSpace>& ptr, std::ptrdiff_t diff) {
    return planar_pixel_reference<typename std::iterator_traits<ChannelPtr>::reference,ColorSpace>(ptr, diff);
}

/////////////////////////////
//  HasDynamicXStepTypeConcept
/////////////////////////////

template <typename IC, typename C>
struct dynamic_x_step_type<yuv_planar_pixel_iterator<IC,C> > {
    typedef memory_based_step_iterator<yuv_planar_pixel_iterator<IC,C> > type;
};


/////////////////////////////
//  iterator_is_basic
/////////////////////////////

template <typename T, typename Cs>  // mutable   planar
struct iterator_is_basic< yuv_planar_pixel_iterator<      T*,Cs> > : public mpl::true_ {};
template <typename T, typename Cs>    // immutable planar
struct iterator_is_basic< yuv_planar_pixel_iterator<const T*,Cs> > : public mpl::true_ {};

template <typename T, typename Cs>  // mutable   planar step
struct iterator_is_basic<memory_based_step_iterator<yuv_planar_pixel_iterator<      T*,Cs> > > : public mpl::true_ {};
template <typename T, typename Cs>    // immutable planar step
struct iterator_is_basic<memory_based_step_iterator<yuv_planar_pixel_iterator<const T*,Cs> > > : public mpl::true_ {};


} // namespace gil
} // namespace boost

#endif // GIL_YUV_PLANAR_PIXEL_ITERATOR_HPP
