#ifndef _freegil_hpp_
#define _freegil_hpp_

#include <boost/gil/gil_all.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_CACHE_H

#include <blend.hpp>

struct glyph
{
	glyph(char ch, 
		FT_Face face, 
		boost::gil::rgb8_pixel_t pixel = boost::gil::rgb8_pixel_t(0,0,0)) :
		ch(ch), 
		face(face), 
		pixel(pixel){}

	char ch;
	FT_Face face;
	boost::gil::rgb8_pixel_t pixel;
};

struct make_metric
{
	template <typename glyph_t>
	FT_Glyph_Metrics operator()(glyph_t glyph)
	{
		BOOST_ASSERT(glyph->face);
		int load_flags = FT_LOAD_DEFAULT;
		int index = FT_Get_Char_Index(glyph->face,glyph->ch);
		FT_Load_Glyph(glyph->face, index, load_flags);
		return glyph->face->glyph->metrics;
	}
};

struct make_width
{
	int advance;
	int lasbwidth;
	int lastadvance;
	make_width() : advance(0), lasbwidth(0), lastadvance(0) {}
	operator int(){return advance-(lastadvance-lasbwidth);}
	void operator()(FT_Glyph_Metrics metrics)
	{
		lastadvance = metrics.horiAdvance >> 6; 
		lasbwidth = (metrics.width >> 6);
		advance += lastadvance;
	}
};

struct make_advance_width
{
	int advance;
	make_advance_width() : advance(0){}
	operator int(){return advance;}
	void operator()(FT_Glyph_Metrics metrics)
	{
		advance += metrics.horiAdvance >> 6;
	}
};

struct make_advance_height
{
	int height;
	make_advance_height() : height(0){}
	operator int(){return height;}
	void operator()(FT_Glyph_Metrics metrics)
	{
		int advance = (metrics.vertAdvance >> 6);
		height = (std::max)(height,advance);
	}
};

struct make_height
{
	int height;
	make_height() : height(0){}
	operator int(){return height;}
	void operator()(FT_Glyph_Metrics metrics)
	{
		int h = (metrics.height >> 6);
		height = (std::max)(height,h);
	}
};

struct make_glyph_height
{
	int height;
	make_glyph_height() : height(0) {} 
	operator int(){return height;}
	void operator()(FT_Glyph_Metrics metrics)
	{
		int n = (metrics.height >> 6) -(metrics.horiBearingY >> 6);
		height = (std::max)(height,n);
	}
};

struct find_last_fitted_glyph
{
	int width,x;
	find_last_fitted_glyph(int width) : width(width),x(0){}
	bool operator()(FT_Glyph_Metrics metric)
	{
		int tmp = x + (metric.width >> 6);
		x += (metric.horiAdvance >> 6); 
		return tmp > width;
	}
};

template <typename view_t>
struct render_gray_glyph
{
	int x;
	const view_t& view;
	render_gray_glyph(const view_t& view) : view(view), x(0) {}	

	template <typename glyph_t>
	void operator()(glyph_t glyph)
	{
		using namespace boost::gil;
		
		FT_GlyphSlot glyphslot = glyph->face->glyph; 
		FT_Face face = glyph->face;			

		int load_flags = FT_LOAD_DEFAULT;
		int index = FT_Get_Char_Index(face,glyph->ch);
		FT_Load_Glyph(face, index, load_flags);
		FT_Render_Glyph(glyphslot, FT_RENDER_MODE_NORMAL);

		int y = view.height() - (face->glyph->metrics.horiBearingY >> 6);
		int width = face->glyph->metrics.width >> 6;
		int height = face->glyph->metrics.height >> 6;
		int xadvance = face->glyph->advance.x >> 6;

		BOOST_ASSERT(view.width());
		BOOST_ASSERT(width == glyphslot->bitmap.width);
		BOOST_ASSERT(height == glyphslot->bitmap.rows);

		gray8c_view_t gray = interleaved_view(width,height,
			(gray8_pixel_t*)glyphslot->bitmap.buffer,glyphslot->bitmap.width);

		copy<alpha24_blend>(glyph->pixel,gray,subimage_view(view,x,y,width,height));
		x += xadvance; 
	}
};

#endif
