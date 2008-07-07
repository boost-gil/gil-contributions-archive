#ifndef TEXT_HPP
#define TEXT_HPP

#include <boost\shared_ptr.hpp>

#include "ipl_image_wrapper.hpp"
#include "drawing.hpp"

namespace boost { namespace gil { namespace opencv {

struct Font_Hershey_Simplex        : boost::mpl::int_< CV_FONT_HERSHEY_SIMPLEX > {};
struct Font_Hershey_Plain          : boost::mpl::int_< CV_FONT_HERSHEY_PLAIN > {};
struct Font_Hershey_Duplex         : boost::mpl::int_< CV_FONT_HERSHEY_DUPLEX > {};
struct Font_Hershey_Complex        : boost::mpl::int_< CV_FONT_HERSHEY_COMPLEX > {};
struct Font_Hershey_Triplex        : boost::mpl::int_< CV_FONT_HERSHEY_TRIPLEX > {};
struct Font_Hershey_Complex_Small  : boost::mpl::int_< CV_FONT_HERSHEY_COMPLEX_SMALL > {};
struct Font_Hershey_Script_Simplex : boost::mpl::int_< CV_FONT_HERSHEY_SCRIPT_SIMPLEX > {};
struct Font_Hershey_Script_Complex : boost::mpl::int_< CV_FONT_HERSHEY_SCRIPT_COMPLEX > {};


typedef boost::shared_ptr< CvFont > ipl_font_wrapper;


template< typename Font_Face
        , typename Line_Type
        >
inline
ipl_font_wrapper create_ipl_font( const Font_Face& font_face
                                , double           hscale
                                , double           vscale
                                , double           shear
                                , std::size_t      thickness
                                , const Line_Type& 
                                )
{
    ipl_font_wrapper ipl_font;

    cvInitFont( ipl_font.get()
              , typename Font_Face::type::value
              , hscale
              , vscale
              , shear
              , thickness
              , typename Line_Type::type::value
              );

    return ipl_font;
}

template< typename Color >
inline
void putText( ipl_image_wrapper&       ipl_image
            , const std::string&       text
            , point_t                  org
            , const ipl_font_wrapper&  ipl_font
            , const Color&             color
            )
{
    cvPutText( ipl_image.get()
             , text.c_str()
             , make_cvPoint ( org )
             , ipl_font.get()
             , make_cvScalar( color )
             );
}

template< typename View
        , typename Color
        >
inline
void putText( View                     v
            , const std::string&       text
            , point_t                  org
            , const ipl_font_wrapper&  ipl_font
            , const Color&             color
            )
{
    ipl_image_wrapper ipl = create_ipl_image( v );

    putText( ipl
           , text
           , org
           , ipl_font
           , color
           );
}

} // namespace opencv
} // namespace gil
} // namespace boost


#endif // TEXT_HPP
