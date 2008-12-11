#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/pnm_read.hpp>
#include <boost/gil/extension/io_new/tiff_write.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
   string in ( ".\\test_images\\pnm\\" );
   string out( ".\\test\\pnm\\" );

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
            && ( fs::extension( dir_itr->path() ) == ".pnm" ))
         {
            rgb8_image_t img;
            string filename = in + dir_itr->path().leaf();

            read_and_convert_image( filename, img, pnm_tag() );

            write_view( out + fs::basename( dir_itr->path() ) + ".tiff"
                      , view( img )
                      , tiff_tag()
                      );
         }
      }
   }

   return 0;
}
