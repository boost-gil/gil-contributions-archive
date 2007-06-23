#ifndef _gil_utils_hpp_
#define _gil_utils_hpp_

// (C) Copyright Tom Brinkman 2007.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/cast.hpp>
#include <boost/gil/image.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <hsv.hpp>
#include <hsl.hpp>

enum 
 { 
 	Left = (0x1 << 0),  
 	Center = (0x1 << 1),  
 	Right = (0x1 << 2),  
 	Top = (0x1 << 3),  
 	Middle = (0x1 << 4),  
 	Bottom = (0x1 << 5), 
 	FMiddle = (0x1 << 6), 
 	FBottom = (0x1 << 7), 
}; 

template <int h, int s, int v> 
struct hsv 
{
	static const int hue=h;
	static const int saturation=s;
	static const int value=v;

	static const boost::gil::hsv32f_pixel_t hsv32f_pixel()
	{
		return boost::gil::hsv32f_pixel_t(h/360.0,s/100.0,v/100.0);
	}

	static const boost::gil::rgb8_pixel_t rgb8_pixel()
	{
		boost::gil::hsv32f_pixel_t frm(h,s,v);
		boost::gil::rgb8_pixel_t to;
		boost::gil::color_convert(frm,to);
		return to;
	}
};

template <int r, int g, int b> 
struct rgb
{
	static const int red=r;
	static const int green=g;
	static const int blue=b;

	static const boost::gil::rgb8_pixel_t rgb8_pixel()
	{
		return boost::gil::rgb8_pixel_t(r,g,b);
	}
};

typedef rgb<0,0,1> invalid_rgb_color;
typedef rgb<0,0,0> black_t;

template <typename T>
struct is_valid_color
{
	static const int good = true;
};

template<>
struct is_valid_color<invalid_rgb_color>
{
	static const int good = false;
};

struct make_balanced_interval
{
	int size,r,adj,pos;
	make_balanced_interval(int width, int size) : 
		size(size), r((width-1)%size),
			adj((width-1)/size), pos(0){}
		
	int operator()(int in)
	{
		int out = pos;
		pos+=adj;

		if (r)
		{
			pos++;
			r--;
		}

		return out;
	}
};

struct make_alpha_blend
{
	short alpha;
	make_alpha_blend(short alpha) : alpha(alpha){}

	template <typename type_t>
	void operator()(type_t& dst, const type_t& src)
	{
		double dbl = (dst * alpha - src * alpha + src * 255.0) / 255.0;
		dst = boost::numeric_cast<type_t>(dbl);
	}
};

struct make_gradient
{
	double adj,adj2;
	make_gradient(double adj) : adj(adj),adj2(1-adj){}

	template <typename type_t>
	void operator()(type_t& dst, const type_t& src)
	{
		double dbl = (double)src * adj  + (double)dst * adj2;
		dst = boost::numeric_cast<type_t>(dbl);
	}
};

template <typename view_t, typename pixel_t> inline
void horizontal_gradient(const view_t& view, const pixel_t& start, const pixel_t& finish)
{
	using namespace boost::gil;

	for (int x = 0; x < view.width(); ++x)
	{
		pixel_t dst = start;
		static_for_each(dst, finish, 
			make_gradient(x/(double)view.width()));
		fill_pixels(subimage_view(view,x,0,1,view.height()),dst);
	}
}

template <typename view_t, typename pixel_t> inline
void vertical_gradient(const view_t& view, const pixel_t& start, const pixel_t& finish)
{
	using namespace boost::gil;

	for (int y = 0; y < view.height(); ++y)
	{
		pixel_t dst = start;
		static_for_each(dst, finish, 
			make_gradient(y/(double)view.height()));
		fill_pixels(subimage_view(view,0,y,view.width(),1),dst);
	}
}

template <typename gview_t, typename view_t, typename pixel_t> inline
void alpha_blend(const gview_t& grayview, const view_t& view, const pixel_t& color)
{
	using namespace boost::gil;

	for (int y = 0; y < view.height(); ++y)
		for (int x = 0; x < view.width(); ++x)
		{
			pixel_t dst = color;
			static_for_each(dst, view(x,y), 
				make_alpha_blend(grayview(x,y)));
			view(x,y) = dst;
		}
}

template <typename view_t, typename value_type>
inline void wuline(
	const view_t& view, const value_type& pixel, 
	int X0, int Y0, int X1, int Y1,
	int NumLevels, int IntensityBits)
{
	using namespace boost::gil;

	unsigned short IntensityShift, ErrorAdj, ErrorAcc;
	unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
	short DeltaX, DeltaY, Temp, XDir;

	if (Y0 > Y1) 
	{
  		Temp = Y0; Y0 = Y1; Y1 = Temp;
  		Temp = X0; X0 = X1; X1 = Temp;
	}

	view(X0,Y0) = pixel;

	if ((DeltaX = X1 - X0) >= 0) 
	{
  		XDir = 1;
	} 
	else 
	{
		XDir = -1;
		DeltaX = -DeltaX; 
	}

	if ((DeltaY = Y1 - Y0) == 0) 
	{
  		while (DeltaX-- != 0) 
		{
			X0 += XDir;
			view(X0,Y0) = pixel;
  		}
      		
		return;
	}
	
	if (DeltaX == 0) 
	{
		do 
		{
			Y0++;
			view(X0,Y0) = pixel;
		} 
		while (--DeltaY != 0);

		return;
	}

	if (DeltaX == DeltaY) 
	{
		do 
		{
			X0 += XDir;
			Y0++;
			view(X0,Y0) = pixel;
		} 
		while (--DeltaY != 0);

		return;
	}

	ErrorAcc = 0;  
	IntensityShift = 16 - IntensityBits;
	WeightingComplementMask = NumLevels - 1;

	if (DeltaY > DeltaX) 
	{
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;

      		while (--DeltaY) 
		{
			ErrorAccTemp = ErrorAcc;   
			ErrorAcc += ErrorAdj;     
         	
			if (ErrorAcc <= ErrorAccTemp) 
				X0 += XDir;
         		
			Y0++;

			Weighting = ErrorAcc >> IntensityShift;
	
			value_type dst = pixel;
			static_for_each(dst,view(X0,Y0),
				make_alpha_blend((Weighting ^ WeightingComplementMask)));
			view(X0,Y0) = dst;

			dst = pixel;
			static_for_each(dst,view(X0 + XDir, Y0),
				make_alpha_blend(Weighting));
			view(X0 + XDir, Y0) = dst;
  		}

		view(X1,Y1) = pixel;
		return;
	}

	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	while (--DeltaX) 
	{
		ErrorAccTemp = ErrorAcc;   
		ErrorAcc += ErrorAdj;     
      		
		if (ErrorAcc <= ErrorAccTemp) 
			Y0++;

		X0 += XDir; 
      		
		Weighting = ErrorAcc >> IntensityShift;

		value_type dst = pixel;
		static_for_each(dst,view(X0,Y0),
			make_alpha_blend(Weighting ^ WeightingComplementMask));
		view(X0,Y0) = dst;
	
		dst = pixel;
		static_for_each(dst,view(X0, Y0 + 1),
			make_alpha_blend(Weighting));
		view(X0, Y0 + 1) = dst;
	}

	view(X1,Y1) = pixel;
}

template <typename view_t>
struct draw_line
{
	typedef typename view_t::value_type value_type;
	const view_t& view;
	const value_type& pixel;
	short NumLevels;
	unsigned short IntensityBits;

	draw_line(const view_t& view, const value_type& pixel, 
		int NumLevels = 256, int IntensityBits=8) :
		view(view), pixel(pixel), NumLevels(NumLevels),
		IntensityBits(IntensityBits){}

	template <typename point_t>
	void operator()(point_t pt0, point_t pt1)
	{
		wuline(view,pixel,
			pt0.x,pt0.y,pt1.x,pt1.y,
			NumLevels,IntensityBits);
	}
};

template <typename view_t, typename pixel_t> inline
void diagonal_gradient(const view_t& view, const pixel_t& start, const pixel_t& finish)
{
	int x1=0,y1=0;
    int x2=0,y2=0;

    while(x1 < view.width() && y1 < view.height())
    {
        if(y1 < view.height()-1)
            y1++;
        else
            x1++;

        if(x2 < view.width()-1)
            x2++;
        else
            y2++;

        double adj = (x1+y1)/((double)view.width()+view.height());
		pixel_t dst = start;
		static_for_each(dst, finish, 
			make_gradient(adj));

		wuline(view,dst,x1,y1,x2,y2,256,8);
    } 
}

#endif
