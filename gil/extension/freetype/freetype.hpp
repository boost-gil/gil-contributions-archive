#ifndef _GIL_FREETYPE_HPP_
#define _GIL_FREETYPE_HPP_

#include <gil/extension/io/png_dynamic_io.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/lambda/lambda.hpp>
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
		using namespace boost::lambda;

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

#endif
