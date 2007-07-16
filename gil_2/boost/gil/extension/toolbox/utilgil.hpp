#ifndef _gil_utils_hpp_
#define _gil_utils_hpp_

// (C) Copyright Tom Brinkman 2007.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/cast.hpp>
#include <boost/gil/image.hpp>
#include <boost/lambda/lambda.hpp>
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

template <int r, int g, int b> 
struct rgb
{
	template <typename type_t>
	void operator()(type_t& out)
	{
		boost::gil::rgb8_pixel_t pix(r,g,b);
		boost::gil::color_convert(pix,out);
	}
};

template <int r, int g, int b, int a> 
struct rgba
{
	template <typename type_t>
	void operator()(type_t& out)
	{
		boost::gil::rgba8_pixel_t pix(r,g,b,a);
		boost::gil::color_convert(pix,out);
	}
};

typedef rgb<0,0,-1> rgb_invalid_t;
typedef rgb<0,0,0> rgb_black_t;
typedef rgb<255,255,255> rgb_white_t;

template <typename type_t>
struct is_valid_color
{
	static const int good = true;
};

template<>
struct is_valid_color<rgb_invalid_t>
{
	static const int good = false;
};

struct make_balanced_interval
{
	int r,adj,pos;
	make_balanced_interval(int width, int size)  : pos(0)
	{
		BOOST_ASSERT(size >= 1);
		r = (width-1)%size,
		adj = (width-1)/size;
	}
		
	int operator()(int in)
	{
		int out = pos;
		pos += adj;

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

template <typename view_t, typename pixel_t>
inline void wuline(const view_t& view, const pixel_t& pixel, 
	int X0, int Y0, int X1, int Y1,
	int NumLevels, int IntensityBits)
{
	using namespace boost::gil;

	unsigned short IntensityShift, ErrorAdj, ErrorAcc;
	unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
	short DeltaX, DeltaY, Temp, XDir;

	if (Y0 > Y1) 
	{
  		Temp = Y0; 
		Y0 = Y1; 
		Y1 = Temp;
  		Temp = X0; 
		X0 = X1; 
		X1 = Temp;
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
	
			pixel_t dst = pixel;
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

		pixel_t dst = pixel;
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

template <typename view_t, typename pixel_t>
struct draw_wuline
{
	typedef typename view_t::value_type value_type;
	const view_t& view;
	short NumLevels;
	unsigned short IntensityBits;

	draw_wuline(const view_t& view, 
		int NumLevels = 256, int IntensityBits=8) :
		view(view), NumLevels(NumLevels),
		IntensityBits(IntensityBits){}

	void operator()(int x, int y, int x1, int y1)
	{
		value_type pixel;
		pixel_t()(pixel);
		
		wuline(view,pixel,
			x,y,x1,y1,
			NumLevels,IntensityBits);
	}

	template <typename point_t>
	void operator()(point_t pt0, point_t pt1)
	{
		value_type pixel;
		pixel_t()(pixel);

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

struct alpha_corner_def
{
	int x;
	int y;
	int alpha;
};

template <typename pixel_t, typename view_t>
inline void draw_alpha_corners(const view_t& view, alpha_corner_def* def, int size)
{
	using namespace boost::gil;
	typedef typename view_t::value_type value_type;
	BOOST_ASSERT(def);
	
	value_type pixel;
	pixel_t()(pixel);
	
	for (int n = 0; n < size; ++n)
	{
		int x = def[n].x;
		int y = def[n].y;
		int alpha = def[n].alpha;
		
		value_type clr = view(x,y);
		static_for_each(clr, pixel, make_alpha_blend(alpha));
		view(x,y) = clr;

		clr = view(view.width()-x-1,view.height()-y-1);
		static_for_each(clr, pixel, make_alpha_blend(alpha));
		view(view.width()-x-1,view.height()-y-1) = clr;

		clr = view(view.width()-x-1,y);
		static_for_each(clr, pixel, make_alpha_blend(alpha));
		view(view.width()-x-1,y) = clr;

		clr = view(x,view.height()-y-1);
		static_for_each(clr, pixel, make_alpha_blend(alpha));
		view(x,view.height()-y-1) = clr;
	}
}

template <int percentage=50>
struct make_relative_gradient
{
	BOOST_STATIC_ASSERT(percentage >= 0 && percentage <= 100);
	
	template <typename out_t>
	void operator()(int size, out_t out)
	{
		int front = boost::numeric_cast<int>(size * percentage/100.0);
		for (int n = 0; n < front; n++)
			*out = 0.0;

		int back = size - front;
		for (int n = 0; n < back; n++)
			*out = n/boost::numeric_cast<double>(back);
	}
};

template <int interval_size> 
struct make_balanced_gradient
{
	template <typename out_t>
	void operator()(int size, out_t out)
	{
		bool up = true;
		int a = 0;
		for (int n = 0; n < size; n++)
		{
			*out = a/boost::numeric_cast<double>(interval_size);

			if (up)
			{
				a++;
				if (a == interval_size)
					up = false;
			}
			else
			{
				a--;
				if (a < 0)
				{
					a = 0;
					up = true;
				}
			}
		}
	}
};

struct make_simple_gradient
{
	template <typename out_t>
	void operator()(int size, out_t out)
	{
		for (int n = 0; n < size; n++)
			*out = n/boost::numeric_cast<double>(size);
	}
};

template <int gradient_size>
struct make_fixed_gradient
{
	template <typename out_t>
	void operator()(int size, out_t out)
	{
	    int start = size - gradient_size;
        if (start >= 0)
		{
			for (int n = 0; n < start; n++)
				*out = 0.0;

			for (int n = 0; n < gradient_size; n++)
				*out = n/boost::numeric_cast<double>(gradient_size);
		}
		else
		{
			for (int n = (gradient_size-size); n < gradient_size; n++)
				*out = n/boost::numeric_cast<double>(gradient_size);    
		}	
	}
};
 
template <typename T>
struct point_in_polygon
{
	typedef std::vector<boost::gil::point2<T> > polygon_t;
	const std::vector<boost::gil::point2<T> >& polygon;
	point_in_polygon(const polygon_t& polygon) : polygon(polygon) {}
		
	bool operator()(const T& px, const T& py)
	{
	      bool result = false;
	      if (polygon.size() < 3) 
		      return false;
	      
	      std::size_t j = polygon.size() - 1;
	      for (std::size_t i = 0; i < polygon.size(); ++i)
	      {
			if (((polygon[i].y <= py) && (py < polygon[j].y)) || 
				((polygon[j].y <= py) && (py < polygon[i].y)))   
			{
				if (px - polygon[i].x < ((polygon[j].x - polygon[i].x) * 
					(py - polygon[i].y) / (polygon[j].y - polygon[i].y)))
				{
					result = !result;
				}
			}
			
			j = i;
	      }
	      
	      return result;
	}
};

template <typename pixel_t>
struct set_alpha_blended_pixel
{
	int alpha;
	boost::gil::rgb8_pixel_t pixel;

	set_alpha_blended_pixel()
	{
		using namespace boost::gil;
		
		rgba8_pixel_t apixel;
		pixel_t()(apixel);
		
		pixel = rgb8_pixel_t(
			get_color(apixel,red_t()),
			get_color(apixel,green_t()),
			get_color(apixel,blue_t()));
		
		alpha = get_color(apixel,alpha_t());
	}

	template <typename point_t>
	void operator()(point_t& point)
	{
		using namespace boost::gil;
		rgb8_pixel_t dst = pixel;
		static_for_each(dst, point, 
			make_alpha_blend(alpha));
		point = dst;
	}
};

template <typename pixel_t>
struct set_pixel
{
	boost::gil::rgb8_pixel_t pixel;

	set_pixel()
	{
		pixel_t()(pixel);
	}

	template <typename view_t>
	void operator()(view_t& view)
	{
		view = pixel;
	}
};

inline bool not_equal(const double& val1, const double& val2, const double& epsilon = 1.0E-07)
{
      double diff = val1 - val2;
      assert(((-epsilon > diff) || (diff > epsilon)) == ((std::abs)(val1 - val2) > epsilon));
      return ((-epsilon > diff) || (diff > epsilon));
}

inline bool is_equal(const double& val1, const double& val2, const double& epsilon = 1.0E-07)
{
    double diff = val1 - val2;
    assert(((-epsilon <= diff) && (diff <= epsilon)) == ((std::abs)(diff) <= epsilon));
    return ((-epsilon <= diff) && (diff <= epsilon));
}

template<typename T>
inline bool intersect(
	const boost::gil::point2<T>& p1,
	const boost::gil::point2<T>& p2,
	const boost::gil::point2<T>& p3,
	const boost::gil::point2<T>& p4,
			boost::gil::point2<T>& out)
{
	double x1 = p1.x;
	double y1 = p1.y;
	double x2 = p2.x;
	double y2 = p2.y;
	double x3 = p3.x;
	double y3 = p3.y;
	double x4 = p4.x;
	double y4 = p4.y;
      
	double ax = x2 - x1;
    double bx = x3 - x4;

    double lowerx;
    double upperx;
    double uppery;
    double lowery;

    if (ax < 0.0)
    {
    	lowerx = x2;
        upperx = x1;
    }
    else
    {
    	upperx = x2;
        lowerx = x1;
    }

    if (bx > 0.0)
    {
    	if ((upperx < x4) || (x3 < lowerx))
        	return false;
    }
    else if ((upperx < x3) || (x4 < lowerx))
	{
         return false;
	}

    double ay = y2 - y1;
    double by = y3 - y4;

    if (ay < 0.0)
    {
	    lowery = y2;
        uppery = y1;
    }
    else
    {
    	uppery = y2;
        lowery = y1;
    }

    if (by > 0.0)
    {
    	if ((uppery < y4) || (y3 < lowery))
        	return false;
    }
	else if ((uppery < y3) || (y4 < lowery))
	{
    	return false;
	}

	double cx = x1 - x3;
    double cy = y1 - y3;
    double d  = (by * cx) - (bx * cy);
    double f  = (ay * bx) - (ax * by);

    if (f > 0.0)
    {
	    if ((d < 0.0) || (d > f))
            return false;
    }
    else if ((d > 0.0) || (d < f))
	{
         return false;
	}

    double e = (ax * cy) - (ay * cx);

    if (f > 0.0)
    {
    	if ((e < 0.0) || (e > f))
	        return false;
    }
    else if ((e > 0.0) || (e < f))
	{
         return false;
	}

    double ratio = (ax * -by) - (ay * -bx);
	double ix,iy;

    if (not_equal(ratio,0.0))
    {
         ratio = ((cy * -bx) - (cx * -by)) / ratio;
         ix = x1 + (ratio * ax);
         iy = y1 + (ratio * ay);
    }
    else
    {
         if (is_equal((ax * -cy),(-cx * ay)))
         {
            ix = x3;
            iy = y3;
         }
         else
         {
            ix = x4;
			iy = y4;
         }
    }

	//TODO: round up or down
	out.x = boost::numeric_cast<int>(ix);
	out.y = boost::numeric_cast<int>(iy);
   	return true;
}

struct make_interval 
{
	int n;
	double interval,minvalue,maxvalue;
	
	make_interval(int levels, double minvalue, double maxvalue) : 
		n(0), minvalue(minvalue), maxvalue(maxvalue)
	{
		interval = (maxvalue-minvalue)/levels;
	}

	double operator()(double in)
	{
		return n++*interval;
	}
};

template <typename minimum_t, typename maximum_t>
struct make_minmax
{
	double& minimum;
	double& maximum;
	make_minmax(double& minimum, double& maximum) : 
		minimum(minimum), maximum(maximum)
	{
		minimum = LONG_MAX;
		maximum = LONG_MIN;
	}

	template <typename type_t>
	void operator()(const type_t& type)
	{
		double minimum2 = minimum_t()(type);
		double maximum2 = maximum_t()(type);

		if (maximum2 > maximum)
			maximum = maximum2;
		if (minimum2 < minimum)
			minimum = minimum2;
	}
};

template <typename actor_t>
struct make_point
{
	int day,days,height;
	make_point(const int days, int height) : 
		day(1),days(days),height(height) {}

	boost::gil::point2<int> operator()(int x)
	{
		int index = days-day++;
		double val,minvalue, maxvalue;
		actor_t()(index, val, minvalue, maxvalue);
		
		double rval = 1-((val-minvalue)/(maxvalue-minvalue));
		int y = (int)(rval*(height-1));
		
		return boost::gil::point2<int>(x,y);
	}
};

#endif
