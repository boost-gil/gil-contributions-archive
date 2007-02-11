/*
#Light GIL wrapper for "freetype" http://www.freetype.com 
#Beta version 1.0.  
#Send changes to [reportbase (at) gmail.com] 
#Final version will be submitted to GIL as an extension. 
*/

#ifndef _GIL_FREETYPE_HPP_ 
#define _GIL_FREETYPE_HPP_ 
 
#include <gil/extension/io/png_dynamic_io.hpp> 
#include <iostream> 
#include <boost/shared_ptr.hpp> 
#include <ft2build.h> 
#include FT_FREETYPE_H 
#include FT_GLYPH_H 
 
enum 
{ 
Left = (0x1 << 0),  
Center = (0x1 << 1),  
Right = (0x1 << 2),  
Top = (0x1 << 3),  
Middle = (0x1 << 4),  
Bottom = (0x1 << 5), 
}; 
 
struct glyph_position 
{ 
glyph_position()  
{ 
glyph = boost::shared_ptr<FT_Glyph>(new FT_Glyph); 
} 
 
boost::shared_ptr<FT_Glyph> glyph; 
FT_Vector pos;  
}; 
 
struct get_glyph_positions 
{ 
FT_UInt glyph_index;  
FT_Bool use_kerning;  
FT_UInt previous;  
int x, y;  
FT_Face face; 
 
get_glyph_positions(FT_Face face) :  
face(face), x(0),y(0),previous(0) 
{ 
use_kerning = FT_HAS_KERNING(face);  
} 
 
boost::shared_ptr<glyph_position> operator()(char ch) 
{ 
int glyph_index = FT_Get_Char_Index(face, ch);  
if (use_kerning && previous && glyph_index)  
{  
FT_Vector delta;  
FT_Get_Kerning(face, previous, glyph_index, FT_KERNING_DEFAULT, &delta);  
x += delta.x >> 6;  
previous = glyph_index;  
}  
 
boost::shared_ptr<glyph_position> glyph_position_ptr =  
boost::shared_ptr<glyph_position>(new glyph_position()); 
glyph_position_ptr->pos.x = x;  
glyph_position_ptr->pos.y = y;  
 
FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);  
FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL); 
FT_Get_Glyph(face->glyph, boost::get_pointer(glyph_position_ptr->glyph));  
 
x += face->glyph->advance.x >> 6;  
y += face->glyph->advance.y >> 6;  
 
return glyph_position_ptr; 
} 
}; 
 
struct get_glyph_bbox 
{ 
FT_BBox& bbox;  
get_glyph_bbox(FT_BBox& bbox) : bbox(bbox)  
{ 
bbox.xMin = bbox.yMin = 32000;  
bbox.xMax = bbox.yMax = -32000; 
} 
 
void operator()(boost::shared_ptr<glyph_position> glyph_position_ptr) 
{ 
glyph_position* glyph_position_rptr = boost::get_pointer(glyph_position_ptr); 
FT_Glyph* glyph = boost::get_pointer(glyph_position_rptr->glyph); 
 
FT_BBox glyph_bbox; 
FT_Glyph_Get_CBox(*glyph,  
ft_glyph_bbox_pixels, &glyph_bbox);  
 
glyph_bbox.xMin += glyph_position_rptr->pos.x;  
glyph_bbox.xMax += glyph_position_rptr->pos.x;  
glyph_bbox.yMin += glyph_position_rptr->pos.y;  
glyph_bbox.yMax += glyph_position_rptr->pos.y;  
 
if ( glyph_bbox.xMin < bbox.xMin )  
bbox.xMin = glyph_bbox.xMin;  
if ( glyph_bbox.yMin < bbox.yMin )  
bbox.yMin = glyph_bbox.yMin;  
if ( glyph_bbox.xMax > bbox.xMax )  
bbox.xMax = glyph_bbox.xMax;  
if ( glyph_bbox.yMax > bbox.yMax )  
bbox.yMax = glyph_bbox.yMax;  
} 
}; 
 
template <typename View, typename Pixel=gil::rgba8_pixel_t> 
struct render_glyph_bitmap 
{ 
const View& view; 
Pixel pixel; 
render_glyph_bitmap(const View& view, Pixel pixel=gil::rgba8_pixel_t(0,0,0,255)) :  
view(view), pixel(pixel){}  
 
void operator()(boost::shared_ptr<glyph_position> glyph_position_ptr) 
{ 
glyph_position* glyph_position_rptr = boost::get_pointer(glyph_position_ptr); 
FT_Glyph* glyph = boost::get_pointer(glyph_position_rptr->glyph); 
FT_Vector pos = glyph_position_rptr->pos; 
 
FT_Glyph_To_Bitmap(glyph, FT_RENDER_MODE_NORMAL, &pos, 0);  
 
FT_BitmapGlyph bit = (FT_BitmapGlyph)*glyph; 
 
int xx = pos.x; 
int yy = view.height() - bit->top; 
 
FT_Int i, j, p, q; 
FT_Int x_max = xx + bit->bitmap.width; 
FT_Int y_max = yy + bit->bitmap.rows; 
 
for (i = xx, p = 0; i < x_max; i++, p++) 
for (j = yy, q = 0; j < y_max; j++, q++) 
{ 
double alpha = bit->bitmap.buffer[q * bit->bitmap.width + p]; 
if (!alpha) 
continue; 
 
double red = (pixel.red * alpha/255.0) + (view(i,j).red*(255.0-alpha)/255.0); 
double green = (pixel.green * alpha/255.0) + (view(i,j).green*(255.0-alpha)/255.0); 
double blue = (pixel.blue * alpha/255.0) + (view(i,j).blue*(255.0-alpha)/255.0); 
view(i,j) = Pixel((int)red,(int)green,(int)blue,255); 
} 
} 
}; 
 
#ifdef _TEST_GIL_FREETYPE_ 
int test_gil_freetype() 
{ 
FT_Library ft_library; 
FT_Face face; 
FT_Init_FreeType(&ft_library); 
FT_New_Face(ft_library,"/home/fonts/verdana.ttf",0,&face); 
FT_Set_Char_Size(face, size * 64, 0, 100, 0); 
 
char str[1000]; 
strcpy(str,"Hello"); 
int align = Center; 
 
std::vector<boost::shared_ptr<glyph_position> > glyphs; 
std::transform(str,str+strlen(str), 
std::back_inserter(glyphs), 
get_glyph_positions(face)); 
 
FT_BBox bbox;  
std::for_each(glyphs.begin(), glyphs.end(), 
get_glyph_bbox(bbox)); 
 
int width = bbox.xMax-bbox.xMin; 
int height = bbox.yMax-bbox.yMin; 
 
int x = xBorder; 
if (align & Center) 
x = (view.width()-width)/2; 
else if (align & Right) 
x = view.width()-width-xBorder; 
 
int y = yBorder; 
if (align & Middle) 
y = (view.height()-height)/2; 
else if (align & Bottom) 
y = view.height()-height-yBorder; 
 
std::for_each(glyphs.begin(), glyphs.end(),  
render_glyph_bitmap<View>( 
gil::subimage_view(view, 
x,y,width,height))); 
 
return 0; 
} 
#endif 
 
#endif 