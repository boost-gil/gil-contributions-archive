#include <iostream>
#include <string>
#include <gil/extension/io/png_dynamic_io.hpp>
#include <gil/core/gil_all.hpp>
#include "freetype/freetype.hpp"

//g++ freetype.cpp -lpng -lfreetype -lz

int main()
{
	FT_Library ft_library;
	FT_Face face;
	FT_Init_FreeType(&ft_library);
	FT_New_Face(ft_library,"/mnt/hdb/fonts/l048016t.pfa",0,&face);
	FT_Set_Char_Size(face, 12 * 64, 0, 100, 0);

	//freetype1.png
	{
		gil::rgba8_image_t img(300,300);

		draw_string("Hello World",face,gil::view(img),
			gil::rgba8_pixel_t(0,0,0,255),Center|Middle);
	
		png_write_view("freetype1.png", gil::view(img));
	}

	//freetype2.png
	{
		gil::rgb8_image_t img(200,200);

		draw_string("Hello World",face,gil::view(img),
			gil::rgb8_pixel_t(0,0,0));
	
		png_write_view("freetype2.png", gil::view(img));
	}

	return 0;
}
