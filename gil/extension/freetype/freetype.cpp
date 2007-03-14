#include <iostream>
#include <string>
#include <sstream>
#include <gil/extension/io/png_dynamic_io.hpp>
#include <gil/core/gil_all.hpp>
#include "freetype.hpp"

using namespace boost;
using namespace boost::freetype;

//Draw a string onto an image
void example_1()
{
	//Step 1.  Initialize the freetype library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library,"c:/fonts/arial.ttf",0,&face);
	FT_Set_Pixel_Sizes(face,0,16);

	//Step 2.  Create a gil image
	gil::rgba8_image_t img(200,30);
	gil::fill_pixels(gil::view(img),gil::rgba8_pixel_t(255,210,200,255));

	//Step 3. Transform the string into a freetype::units vector
	std::string str = "hello";
	std::vector<boost::shared_ptr<freetype::unit<gil::rgba8_pixel_t> > > units;
	std::transform(str.begin(), str.end(),
		std::back_inserter(units), freetype::make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	//Step 4. Draw the freetype::units vector onto the gil image
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img), freetype::Center|freetype::Middle);

	//Step 5. output the image
	png_write_view("c:/out/example_1.png", gil::view(img));
};

//Draws the string into nine image quadrants
void example_2()
{
	//Step 1.  Initialize the freetype library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library,"c:/fonts/arial.ttf",0,&face);
	FT_Set_Pixel_Sizes(face,0,16);

	//Step 2.  Create a gil image
	gil::rgba8_image_t img(200,60);
	gil::fill_pixels(gil::view(img),gil::rgba8_pixel_t(255,210,200,255));

	//Step 3. Transform the string into a freetype::units vector
	std::string str = "hello";
	std::vector<boost::shared_ptr<freetype::unit<gil::rgba8_pixel_t> > > units;
	std::transform(str.begin(), str.end(),
		std::back_inserter(units), freetype::make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	//Step 4. Draw the freetype::units vector onto the gil image
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Left|freetype::Top);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Left|freetype::Middle);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Left|freetype::Bottom);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Center|freetype::Top);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Center|freetype::Bottom);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Right|freetype::Top);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Right|freetype::Middle);
	boost::freetype::draw(units.begin(), units.end(), 
		gil::view(img),freetype::Right|freetype::Bottom);

	//Step 5. output the image
	png_write_view("c:/out/example_2.png", gil::view(img));
};

//Draw a string with elipses at the end
void example_3()
{
	//Step 1.  Initialize the freetype library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library,"c:/fonts/arial.ttf",0,&face);
	FT_Set_Pixel_Sizes(face,0,16);

	//Step 2.  Create a gil image
	gil::rgba8_image_t img(200,30);
	gil::fill_pixels(gil::view(img),gil::rgba8_pixel_t(255,210,200,255));

	//Step 3. Transform the string into a freetype::units vector
	std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<boost::shared_ptr<freetype::unit<gil::rgba8_pixel_t> > > units;
	std::transform(str.begin(), str.end(),
		std::back_inserter(units), freetype::make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	std::string elipses(3,'.');
	std::vector<boost::shared_ptr<unit<gil::rgba8_pixel_t> > > velipses;
	std::transform(elipses.begin(),elipses.end(),
		std::back_inserter(velipses), make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	//Step 4. Draw the freetype::units vector onto the gil image
	boost::freetype::draw_elipsed(units.begin(), units.end(), velipses.begin(), velipses.end(),
		gil::view(img), Left|Top);

	//Step 5. output the image
	png_write_view("c:/out/example_3.png", gil::view(img));
};

//Draw an index styled string
void example_4()
{
	//Step 1.  Initialize the freetype library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library,"c:/fonts/arial.ttf",0,&face);
	FT_Set_Pixel_Sizes(face,0,16);

	//Step 2.  Create a gil image
	gil::rgba8_image_t img(200,30);
	gil::fill_pixels(gil::view(img),gil::rgba8_pixel_t(255,210,200,255));

	//Step 3. Transform the strings into freetype::units vectors
	std::string str = "XXX";
	std::vector<boost::shared_ptr<unit<gil::rgba8_pixel_t> > > units;
	std::transform(str.begin(), str.end(),
		std::back_inserter(units), make_unit<gil::rgba8_pixel_t>(
			face,gil::rgba8_pixel_t(0,0,0,255)));
	
	std::string str2 = "YYY";
	std::vector<boost::shared_ptr<unit<gil::rgba8_pixel_t> > > units2;
	std::transform(str2.begin(), str2.end(),
		std::back_inserter(units2), make_unit<gil::rgba8_pixel_t>(
			face,gil::rgba8_pixel_t(0,0,0,255)));

	std::string dots(200,'.');
	std::vector<boost::shared_ptr<unit<gil::rgba8_pixel_t> > > dunits;
	std::transform(dots.begin(),dots.end(),
		std::back_inserter(dunits), make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	//Step 4. Draw the freetype::units vector onto the gil image
	freetype::draw_index(
			units.begin(), units.end(), 
			units2.begin(), units2.end(),
			dunits.begin(), dunits.end(),
			gil::view(img), Middle);

	//Step 5. output the image
	png_write_view("c:/out/example_4.png", gil::view(img));
}

//Draw a wrapped string
void example_5()
{
	//Step 1.  Initialize the freetype library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library,"c:/fonts/arial.ttf",0,&face);
	FT_Set_Pixel_Sizes(face,0,16);

	//Step 2.  Create a gil image
	gil::rgba8_image_t img(200,100);
	gil::fill_pixels(gil::view(img),gil::rgba8_pixel_t(255,210,200,255));

	//Step 3. Transform the string into a freetype::units vector
	std::string str = "Images are a fundamental construct in Digital Imaging/Digital Video projects "
		"and yet the variability in pixel data representations (color space, bit depth, channel ordering, "
		"planar/interleaved, alignment policy) makes it hard to write imaging-related code that is both "
		"generic and efficient. This library allows for writing generic imaging algorithms with performance "
		"comparable to hand-writing for a particular image type. The library is designed with the "
		"following five goals in mind.";
	std::vector<boost::shared_ptr<freetype::unit<gil::rgba8_pixel_t> > > units;
	std::transform(str.begin(), str.end(),
		std::back_inserter(units), freetype::make_unit<gil::rgba8_pixel_t>(
		face,gil::rgba8_pixel_t(0,0,0,255)));

	//Step 4. Draw the freetype::units vector onto the gil image
	boost::freetype::draw_wrapped(units.begin(), units.end(), 
		gil::view(img),freetype::Center|freetype::Middle);

	//Step 5. output the image
	png_write_view("c:/out/example_5.png", gil::view(img));
}

int main (int args, char *argv[])
{
	example_1();
	example_2();
	example_3();
	example_4();
	example_5();
	return 0;
}
