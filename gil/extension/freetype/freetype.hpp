#ifndef _GIL_FREETYPE_HPP_
#define _GIL_FREETYPE_HPP_

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <gil/core/gil_all.hpp>
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

template <typename type=double>
struct interpolate_channels
{
	type alpha,beta;
	interpolate_channels(type alpha) : 
		alpha(alpha),beta(1-alpha)
	{
		assert(alpha >= 0 && alpha <= 1);
	}

	template <typename T>
	void operator()(T& dst, const T src)
	{
		dst = (T)(dst*alpha) + (T)(src*beta);
	}
};

template <typename View, typename Pixel>
struct render_glyph_bitmap
{
	const View& view;
	Pixel pixel;
	render_glyph_bitmap(const View& view, Pixel pixel=gil::rgba8_pixel_t(0,0,0,255)) : 
		view(view), pixel(pixel){}	

	void operator()(boost::shared_ptr<glyph_position> glyph_position_ptr)
	{
		boost::function_requires<gil::PixelsCompatibleConcept<typename View::pixel_t,Pixel> >();
		boost::function_requires<gil::ImageViewConcept<View> >();

		glyph_position* glyph_position_rptr = boost::get_pointer(glyph_position_ptr);
		FT_Glyph* glyph = boost::get_pointer(glyph_position_rptr->glyph);
		FT_Vector pos = glyph_position_rptr->pos;

		FT_Glyph_To_Bitmap(glyph, FT_RENDER_MODE_NORMAL, &pos, 0); 
		FT_BitmapGlyph bit = (FT_BitmapGlyph)*glyph;

		View glyph_view = gil::subimage_view(view,pos.x,view.height()-bit->top,
			bit->bitmap.width,bit->bitmap.rows);

		for (int x = 0; x < glyph_view.width(); x++)
			for (int y = 0; y < glyph_view.height(); y++)
			{
				int alpha = bit->bitmap.buffer[y * bit->bitmap.width + x];
				if (!alpha)
					continue;
				
				Pixel dst = pixel;
				Pixel src = glyph_view(x,y);

				double a = (alpha)/255.0;
				gil::for_each_channel(dst,src,
					interpolate_channels<>(a));
				glyph_view(x,y) = dst;
			}
	}
};

template <typename View, typename Pixel>
inline void draw_string(
	char* str,
	const View& view,
	Pixel pixel = gil::rgba8_pixel_t(0,0,0,255),
	FT_Face face,
	int align = Center|Middle, 
	int xBorder=0, 
	int yBorder=0)
{
	std::vector<boost::shared_ptr<glyph_position> > glyphs;
	std::transform(str,str+strlen(str),
		std::back_inserter(glyphs),
		get_glyph_positions(face));

	FT_BBox bbox; 
	std::for_each(glyphs.begin(), glyphs.end(),
		get_glyph_bbox(bbox));

	int width = bbox.xMax-bbox.xMin;
	int height = bbox.yMax-bbox.yMin;

	//TODO: not working
	if (width > view.width())
	{
		std::vector<boost::shared_ptr<glyph_position> > dotglyphs;
		char dots[4];
		strcpy(dots,"...");
		std::transform(dots,dots+strlen(dots),
			std::back_inserter(dotglyphs),
			get_glyph_positions(face));

		FT_BBox dbbox; 
		std::for_each(dotglyphs.begin(), dotglyphs.end(),
			get_glyph_bbox(dbbox));
	
		int dwidth = bbox.xMax-bbox.xMin;
		int dheight = bbox.yMax-bbox.yMin;

		char tmp[1000];
		int n = strlen(str);
		for (; n >= 0; n--)
		{
			strncpy(tmp,str,n);
			tmp[n+1]='\0';

			std::for_each(glyphs.begin(), glyphs.begin()+n,
				get_glyph_bbox(bbox));

			int awidth = bbox.xMax-bbox.xMin;
			if (awidth + dwidth > view.width())
				continue;

			strcpy(str,tmp);
			strcat(str,"...");

			glyphs.clear();
			std::transform(str,str+strlen(str),
				std::back_inserter(glyphs),
				get_glyph_positions(face));

			std::for_each(glyphs.begin(), glyphs.end(),
				get_glyph_bbox(bbox));
			width = bbox.xMax-bbox.xMin;
			break;
		}
	}	

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

	typedef typename View::pixel_t pixel_t;

	std::for_each(glyphs.begin(), glyphs.end(), 
		render_glyph_bitmap<View,pixel_t>(
			gil::subimage_view(view,x,y,width,height),pixel));
}

#endif
