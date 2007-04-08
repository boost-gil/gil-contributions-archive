#ifndef _BOOST_FREETYPE_HPP_
#define _BOOST_FREETYPE_HPP_

// (C) Copyright Tom Brinkman 2007.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/core.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/spirit/actor/push_back_actor.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <gil/core/gil_all.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace boost {
namespace freetype {

enum{
	Left = (0x1 << 0), 
	Center = (0x1 << 1), 
	Right = (0x1 << 2), 
	Top = (0x1 << 3), 
	Middle = (0x1 << 4), 
	Bottom = (0x1 << 5),
	NBottom = (0x1 << 5)
};

inline std::ostream& operator << (std::ostream& out, FT_Glyph_Metrics metrics)
{
		out << 
			(metrics.width >> 6) << "," << 
			(metrics.height >> 6) << "," << 
			(metrics.horiBearingX >> 6) << "," << 
			(metrics.horiBearingY >> 6) << "," << 
			(metrics.horiAdvance >> 6) << "," << 
			(metrics.vertBearingX >> 6) << "," << 
			(metrics.vertBearingY >> 6) << "," << 
			(metrics.vertAdvance >> 6) << "\n";
		return out;
};

template <typename T>
inline void make_poslst(int width, int size, T poslst)
{
	int r = (width % (size-1));
	int adj = width / (size-1);
	int pos = 0;
	
	for (int n = 0; n < size; n++, pos+=adj)
	{
		*poslst = pos;
		
		if (!r)
			continue;
		
		pos++;
		r--;
	}

	assert(pos-adj == width);
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

template <typename Color>
struct unit
{
	FT_Face face;
	char ch;
	Color color;
	unit(FT_Face face, char ch, Color color) :
		face(face), ch(ch), color(color) {}
};

template <typename Color>
struct make_unit
{
	Color color;
	FT_Face face;
	make_unit(FT_Face face, Color color) : 
		face(face), color(color) {}

	//TODO: copy constructor
	make_unit(boost::shared_ptr<freetype::unit<Color> > unit) : 
		face(unit->face), color(unit->color) {}

	boost::shared_ptr<freetype::unit<Color> > operator()(char ch)
	{
		return boost::shared_ptr<freetype::unit<Color> >(
			new freetype::unit<Color>(face,ch,color));
	}
};

struct make_metric
{
	template <typename Unit>
	FT_Glyph_Metrics operator()(Unit unit)
	{
		int glyph_index = FT_Get_Char_Index(unit->face, unit->ch); 
		FT_Load_Glyph(unit->face, glyph_index, FT_LOAD_DEFAULT); 
		return unit->face->glyph->metrics;
	}
};

struct make_kerning
{
	int previndex;
	make_kerning() : previndex(0) {}

	template <typename Unit>
	int operator()(Unit unit)
	{
		int glyph_index = FT_Get_Char_Index(unit->face, unit->ch); 
		if (!FT_HAS_KERNING(unit->face) || !previndex || !glyph_index) 
			return 0;

		FT_Vector delta; 
		FT_Get_Kerning(unit->face, previndex, glyph_index, FT_KERNING_DEFAULT, &delta); 
		previndex = glyph_index; 
		return delta.x >> 6; 
	}
};

template<typename _InIt, typename _InIt2, typename Pred> inline
_InIt find_if(_InIt _First, _InIt _Last, _InIt2 _First2, Pred pred)
{	
	for (; _First != _Last; ++_First,++_First2)
		if (pred(*_First,*_First2))
			break;
	return (_First);
}

template<typename _InIt, typename _InIt2, typename _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _InIt2 _First2, _Fn1 _Func)
{	
	for (; _First != _Last; ++_First,++_First2)
		_Func(*_First,*_First2);
	return (_Func);
}

template<typename _InIt, typename _InIt2, typename _InIt3, typename _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _InIt2 _First2, _InIt3 _First3, _Fn1 _Func)
{	
	for (; _First != _Last; ++_First,++_First2,++_First3)
		_Func(*_First, *_First2, *_First3);
	return (_Func);
}

struct get_width
{
	int advance;
	int lastwidth;
	int lastadvance;
	get_width() : advance(0), lastwidth(0), lastadvance(0) {}

	operator int()
	{
		int adj = lastadvance-lastwidth;
		return advance-adj;
	}

	void operator()(FT_Glyph_Metrics metrics, int kerning)
	{
		lastadvance = kerning + (metrics.horiAdvance >> 6); 
		lastwidth = (metrics.width >> 6);
		advance += lastadvance;
	}
};

struct get_full_width
{
	int advance;
	get_full_width() : advance(0){}

	operator int()
	{
		return advance;
	}

	void operator()(FT_Glyph_Metrics metrics, int kerning)
	{
		advance += kerning + (metrics.horiAdvance >> 6);
	}
};

struct get_height
{
	int height;
	get_height() : height(0){}
	operator int(){return height;}

	void operator()(FT_Glyph_Metrics metrics)
	{
		int advance = (metrics.vertAdvance >> 6);
		height = (std::max)(height,advance);
	}
};

struct get_glyph_height
{
	int height;
	get_glyph_height() : height(0) {} 
	operator int(){return height;}

	void operator()(FT_Glyph_Metrics metrics)
	{
		int n = (metrics.height >> 6) -
			(metrics.horiBearingY >> 6);
		height = (std::max)(height,n);
	}
};

template <typename View>
struct draw_glyph
{
	const View& view;
	int x;
	draw_glyph(const View& view) : 
		view(view), x(0){}	

	template <typename Unit>
	void operator()(Unit unit, FT_Glyph_Metrics metrics, int kerning)
	{
		int glyph_index = FT_Get_Char_Index(unit->face, unit->ch);
		FT_Load_Glyph(unit->face, glyph_index, FT_LOAD_DEFAULT); 

		FT_Glyph glyph;
		FT_Get_Glyph(unit->face->glyph, &glyph); 

		FT_Vector pos;
		pos.x = x + kerning;
		pos.y = 0;
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &pos, 0); 
		FT_BitmapGlyph bit = (FT_BitmapGlyph)glyph;

		int y = view.height() - (metrics.horiBearingY >> 6);
		int i, j, p, q;
		int x_max = x + bit->bitmap.width;
		int y_max = y + bit->bitmap.rows;

		typedef typename View::pixel_t pixel_t; 

		for ( i = x, p = 0; i < x_max; i++, p++ )
			for ( j = y, q = 0; j < y_max; j++, q++ )
			{
				int alpha = bit->bitmap.buffer[q * bit->bitmap.width + p];
				if (!alpha)
					continue;

				pixel_t dst = unit->color;
				pixel_t src = view(i,j);

				double a = (alpha)/255.0;
				gil::for_each_channel(dst,src,
					interpolate_channels<>(a));
				view(i,j) = dst;
			}

		x += (unit->face->glyph->advance.x >> 6); 
	}
};

struct exceeds_width
{
	int width,x;
	exceeds_width(int width) : width(width),x(0){}

	bool operator()(FT_Glyph_Metrics metric, int kerning)
	{
		x += kerning;
		int tmp = x + (metric.width >> 6);
		x += (metric.horiAdvance >> 6); 
		return tmp > width;
	}
};

struct clip
{
	template <typename Color>
	int operator()(int width, std::vector<boost::shared_ptr<unit<Color> > >& units,
		std::vector<FT_Glyph_Metrics>& metrics,
		std::vector<int>& kerning)
	{
		std::vector<FT_Glyph_Metrics>::iterator it = 
			freetype::find_if(metrics.begin(),metrics.end(),kerning.begin(),
				exceeds_width(width));

		int distance = (int)std::distance(it,metrics.end());
		units.erase(units.end()-distance,units.end());
		return freetype::for_each(metrics.begin(),metrics.begin()+units.size(), 
			kerning.begin(),get_width());
	}
};

template <typename T, typename View> inline 
int draw(T begin, T end, const View& view, int align)
{
	std::vector<FT_Glyph_Metrics> metrics;			
	std::transform(begin,end,
		std::back_inserter(metrics), make_metric());

	std::vector<int> kerning;			
	std::transform(begin,end,
		std::back_inserter(kerning), make_kerning());

	int width = freetype::for_each(
		metrics.begin(), metrics.end(), kerning.begin(), get_width());
	int height = std::for_each(
		metrics.begin(), metrics.end(), get_height());

	if (width > view.width())
		return 0;
	
	int x = 0;
	if (align & Center)
		x = (view.width()-width)/2;
	else if (align & Right)
		x = view.width()-width;

	int y = 0;
	if (align & Middle)
		y = (view.height()-height)/2;
	else if (align & Bottom || align & NBottom)
		y = view.height()-height;
	
	int gheight = 0;
	if (align & NBottom)
		gheight = std::for_each(metrics.begin(),
			metrics.end(),get_glyph_height());

	freetype::for_each(begin, end, 
		metrics.begin(), kerning.begin(),
		draw_glyph<View>(gil::subimage_view(view,x,y,width,height-gheight)));

	return width;
}

template <typename T, typename T2, typename View> inline 
int draw_elipsed(T begin, T end, T2 begin2, T2 end2, const View& view, int align)
{
	std::vector<FT_Glyph_Metrics> dmetrics;			
	std::transform(begin2,end2,
		std::back_inserter(dmetrics), make_metric());

	std::vector<int> dkerning;			
	std::transform(begin2,end2,
		std::back_inserter(dkerning), make_kerning());

	int dwidth = freetype::for_each(dmetrics.begin(),dmetrics.end(), 
		dkerning.begin(),get_width());
	
	std::vector<FT_Glyph_Metrics> metrics;			
	std::transform(begin,end,
		std::back_inserter(metrics), make_metric());

	std::vector<int> kerning;			
	std::transform(begin,end,
		std::back_inserter(kerning), make_kerning());

	std::vector<FT_Glyph_Metrics>::iterator it = 
		freetype::find_if(metrics.begin(),metrics.end(),kerning.begin(),
			exceeds_width(view.width()-dwidth));

	int distance = (int)std::distance(it,metrics.end());
	int width = freetype::for_each(
		metrics.begin(), metrics.end()-distance, kerning.begin(), get_width());
	int height = std::for_each(
		metrics.begin(), metrics.end()-distance, get_height());

	int x = 0;
	int y = 0;
	if (align & Middle)
		y = (view.height()-height)/2;
	else if (align & Bottom || align & NBottom)
		y = view.height()-height;
	
	int gheight = 0;
	if (align & NBottom)
		gheight = std::for_each(metrics.begin(),
			metrics.end(),get_glyph_height());

	freetype::for_each(begin, end-distance, 
		metrics.begin(), kerning.begin(),
		draw_glyph<View>(gil::subimage_view(view,x,y,view.width(),height-gheight)));

	freetype::for_each(begin2, end2, 
		dmetrics.begin(), dkerning.begin(),
		draw_glyph<View>(gil::subimage_view(view,x+width,y,dwidth,height-gheight)));

	return width+dwidth;
}

template <typename T, typename View> inline 
int draw_clipped(T begin, T end, const View& view, int align)
{
	std::vector<FT_Glyph_Metrics> metrics;			
	std::transform(begin,end,
		std::back_inserter(metrics), make_metric());

	std::vector<int> kerning;			
	std::transform(begin,end,
		std::back_inserter(kerning), make_kerning());

	std::vector<FT_Glyph_Metrics>::iterator it = 
		freetype::find_if(metrics.begin(),metrics.end(),kerning.begin(),
			exceeds_width(view.width()));

	int distance = (int)std::distance(it,metrics.end());
	int width = freetype::for_each(
		metrics.begin(), metrics.end()-distance, kerning.begin(), get_width());
	int height = std::for_each(
		metrics.begin(), metrics.end()-distance, get_height());

	int x = 0;
	int y = 0;
	if (align & Middle)
		y = (view.height()-height)/2;
	else if (align & Bottom || align & NBottom)
		y = view.height()-height;
	
	int gheight = 0;
	if (align & NBottom)
		gheight = std::for_each(metrics.begin(),
			metrics.end(),get_glyph_height());

	freetype::for_each(begin, end-distance, 
		metrics.begin(), kerning.begin(),
		draw_glyph<View>(gil::subimage_view(view,x,y,view.width(),height-gheight)));

	return width;
}

template <typename F, typename B, typename D,typename View> inline 
int draw_index(
	F beginf, F endf, 
	B beginb, B endb, 
	D begind, D endd, 
	const View& view, int align)
{
	std::vector<FT_Glyph_Metrics> fmetrics,bmetrics,dmetrics;			
	std::vector<int> fkerning,bkerning,dkerning;			

	std::transform(begind,begind+3,
		std::back_inserter(dmetrics), make_metric());
	std::transform(begind,begind+3,
		std::back_inserter(dkerning), make_kerning());
	int dwidth = freetype::for_each(dmetrics.begin(),dmetrics.end(), 
		dkerning.begin(),get_width());

	std::transform(beginb,endb,
		std::back_inserter(bmetrics), make_metric());
	std::transform(beginb,endb,
		std::back_inserter(bkerning), make_kerning());
	int bwidth = freetype::for_each(bmetrics.begin(),bmetrics.end(), 
		bkerning.begin(),get_width());

	std::transform(beginf,endf,
		std::back_inserter(fmetrics), make_metric());
	std::transform(beginf,endf,
		std::back_inserter(fkerning), make_kerning());
	int fwidth = freetype::for_each(fmetrics.begin(),fmetrics.end(), 
		fkerning.begin(),get_width());

	int rwidth = view.width() - dwidth - bwidth;
	if (rwidth > fwidth)
		rwidth = fwidth;

	fwidth = draw_clipped(beginf, endf, 
		gil::subimage_view(view,0,0,rwidth,view.height()), Left|Middle);
	bwidth = draw(beginb, endb, 
		gil::subimage_view(view,0,0,view.width(),view.height()), Right|Middle);

	dwidth = view.width()- fwidth - bwidth;
	bwidth = draw_clipped(begind, endd, 
		gil::subimage_view(view,fwidth,0,dwidth,view.height()), Center|Middle);

	return view.width();
}

template <typename F, typename View> inline 
int draw_tabbed(F beginf, F endf, const View& view, int align)
{
	return 0;
}

template <typename F, typename View> inline 
int draw_wrapped(F beginf, F endf, const View& view, int align)
{
	return 0;
}

} //free
} //boost

#endif


