#include "stdafx.h"

#include <boost/filesystem.hpp>

#include <boost/gil/extension/io_new/bmp_all.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

typedef bmp_tag tag_t;

// Test will loop through the "in" folder to read and convert
// the png's to rgb8_image_t's. Which then will be written in
// the "out" folder.
// 


// For rgba to rgb conversion. The default multiplies alpha with the color channels.
// But in the test image alpha is 0.
struct my_color_converter
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        get_color( dst, red_t()   ) = get_color( src, red_t()   );
        get_color( dst, green_t() ) = get_color( src, green_t() );
        get_color( dst, blue_t()  ) = get_color( src, blue_t()  );
    }
};

int main()
{
    {
        string in ( ".\\test_images\\bmp\\" );
        string out( ".\\test\\bmp\\" );

        fs::path in_path = fs::system_complete( fs::path( in, fs::native ) );

        if ( fs::is_directory( in_path ) )
        {
            fs::directory_iterator end_iter;
            for( fs::directory_iterator dir_itr( in_path )
               ; dir_itr != end_iter
               ; ++dir_itr
               )
            {
                if ( fs::is_regular( dir_itr->status() ) 
                    && ( fs::extension( dir_itr->path() ) == ".bmp" ))
                {
                    rgb8_image_t img;
                    string filename = in + dir_itr->path().leaf();

                    try
                    {
                        image_read_info< bmp_tag > info = read_image_info( filename, tag_t() );

                        if( info._bits_per_pixel == 32 )
                        {
                            read_and_convert_image( filename, img, my_color_converter(), tag_t() );
                        }
                        else
                        {
                            read_and_convert_image( filename, img, tag_t() );
                        }

                        write_view( out + fs::basename( dir_itr->path() ) + ".bmp"
                                  , view( img )
                                  , tag_t()
                                  );
                    }
                    catch( const std::ios_base::failure& ) {}
                }
            }
        }
    }
}
