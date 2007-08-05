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

template <int r, int g, int b> 
struct rgb8
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t pixel;
	rgb8(pixel_t pixel = pixel_t(r,g,b)) : pixel(pixel) {}
	operator pixel_t(){return pixel;}
		
	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		view(x,y) = pixel;
	}
};

template <int r, int g, int b, int a> 
struct rgba8
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	int alpha;
	pixel_t pixel;
	rgba8(pixel_t pixel = pixel_t(r,g,b), int alpha = a) : 
			pixel(pixel), alpha(alpha) {}
	operator pixel_t(){return pixel;}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		pixel_t dst = pixel;
		boost::gil::static_for_each(dst, view(x,y), make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};

template <typename fpix_t, typename tpix_t>
struct horizontal_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix;
	pixel_t tpix;

	horizontal_gradient() :
		fpix(fpix_t()), tpix(tpix_t()){}

	operator pixel_t(){return fpix;}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		double perc = boost::numeric_cast<double>(x+1) / view.width();
		int alpha = boost::numeric_cast<int>(perc*255);
		pixel_t dst = fpix;
		boost::gil::static_for_each(dst, tpix, make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};

template <typename fpix_t, typename tpix_t>
struct vertical_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix;
	pixel_t tpix;

	vertical_gradient() :
		fpix(fpix_t()), tpix(tpix_t()){}

	operator pixel_t(){return fpix;}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		double perc = boost::numeric_cast<double>(y+1) / view.height();
		int alpha = boost::numeric_cast<int>(perc*255);
		pixel_t dst = fpix;
		boost::gil::static_for_each(dst, tpix, make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};


template <typename fpix_t, typename tpix_t, std::size_t size>
struct balanced_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix;
	pixel_t tpix;
	bool up;
	int a;

	balanced_gradient() :
		fpix(fpix_t()), tpix(tpix_t()), up(true), a(0){}

	operator pixel_t(){return fpix;}
		
	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		double perc = a/boost::numeric_cast<double>(size);
		int alpha = boost::numeric_cast<int>(perc*255);

		boost::gil::rgb8_pixel_t dst = fpix;
		boost::gil::static_for_each(dst, tpix, make_alpha_blend(alpha));
		view(x,y) = dst;
		
		if (up)
		{
			a++;
			if (a == size)
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
};

template <typename fpix_t, typename tpix_t>
struct diagonal_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix;
	pixel_t tpix;
	diagonal_gradient() : fpix(fpix_t()), tpix(tpix_t()) {}
	operator pixel_t(){return fpix;}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y)
	{
		double xperc = boost::numeric_cast<double>(x+1) / view.width();
		double yperc = boost::numeric_cast<double>(y+1) / view.height();
		double aperc = (xperc + yperc) / 2;

		int alpha = boost::numeric_cast<int>(aperc*255);
		pixel_t dst = fpix;
		boost::gil::static_for_each(dst, tpix, make_alpha_blend(alpha));
		view(x,y) = dst;
	}
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
	int NumLevels = 256, int IntensityBits=8)
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

template <typename view_t>
struct draw_wuline
{
	typedef typename view_t::value_type value_type;
	const view_t& view;
	value_type pixel;
	draw_wuline(const view_t& view, value_type pixel) : 
		view(view), pixel(pixel) {}

	void operator()(int x, int y, int x1, int y1)
	{
		wuline(view,pixel,x,y,x1,y1);
	}

	template <typename point_t>
	void operator()(point_t pt0, point_t pt1)
	{
		wuline(view,pixel,pt0.x,pt0.y,pt1.x,pt1.y);
	}
};

struct defin
{
	int x;
	int y;
};

template <typename view_t>
struct translate_coordinates
{
	const view_t& view;
	translate_coordinates(const view_t& view) : view(view){}	
		
	boost::gil::point2<int> operator()(const defin& def)
	{
		using namespace boost;
		
		double x = (numeric_cast<double>(def.x)/100.0) * view.width();
		double y = (numeric_cast<double>(def.y)/100.0) * view.height();
		
		return boost::gil::point2<int>(
			numeric_cast<int>(x),numeric_cast<int>(y));
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

template <typename out_t> inline
void make_circle(int lines, double radius, int cx, int cy, out_t out)
{
	double pie = 3.1415926535;		
	int fx,fy;
	for (int line = 0; line < lines; line++)
	{
		double angle = line*2*pie/lines;
		int x = boost::numeric_cast<int>(radius * std::cos(angle) + cx);
		int y = boost::numeric_cast<int>(radius * std::sin(angle) + cy);
		*out = boost::gil::point2<int>(x,y);
		
		if (line != 0)
			continue;
		
		fx = x;
		fy = y;
	}

	*out = boost::gil::point2<int>(fx,fy);
}

template <typename out_t> inline
void make_star(int lines, double radius, double radius2, int cx, int cy, out_t out)
{
	double pie = 3.1415926535;		
	int fx,fy;
	for (int line = 0; line < lines; line++)
	{
		double rad;
		line % 2 == 0 ? rad = radius2 : rad = radius;

		double angle = line*2*pie/lines;
		int x = boost::numeric_cast<int>(rad * std::cos(angle) + cx);
		int y = boost::numeric_cast<int>(rad * std::sin(angle) + cy);
		*out = boost::gil::point2<int>(x,y);
		
		if (line == 0)
		{
			fx = x;
			fy = y;
		}
	}

	*out = boost::gil::point2<int>(fx,fy);
}

template <typename out_t> inline
void make_rectangle(int width, int height, out_t out)
{
	*out = boost::gil::point2<int>(0,0);
	*out = boost::gil::point2<int>(0,height);
	*out = boost::gil::point2<int>(width,height);
	*out = boost::gil::point2<int>(width,0);
	*out = boost::gil::point2<int>(0,0);
}

struct make_extents
{
	int& top;
	int& left;
	int& right;
	int& bottom;
	make_extents(int& left, int& top, int& right, int& bottom) : 
		left(left), top(top), right(right), bottom(bottom)
	{
		left = (std::numeric_limits<int>::max)();
		top = (std::numeric_limits<int>::max)();
		right = (std::numeric_limits<int>::min)();
		bottom = (std::numeric_limits<int>::min)();
	}

	void operator()(const boost::gil::point2<int>& point)
	{
		if (point.x < left)
			left = point.x;
		if (point.y < top)
			top = point.y;
		if (point.x > right)
			right = point.x;
		if (point.y > bottom)
			bottom = point.y;
	}	
};

struct shift_polygon
{
	int left, top;
	shift_polygon(int left, int top) : left(left), top(top) {}

	boost::gil::point2<int> operator()(const boost::gil::point2<int>& point)
	{
		boost::gil::point2<int> opoint = point;
		opoint.x += left;
		opoint.y += top;
		return opoint;
	}	
};

template <typename fill_pixels_t, typename view_t, typename polygon_t>
void fill_polygon(const view_t& view, const polygon_t& polygon)
{
	int left,right,top,bottom;
	std::for_each(polygon.begin(),polygon.end(),
		make_extents(left,top,right,bottom));       
	
	polygon_t polygon2;
	std::transform(polygon.begin(),polygon.end(),
		std::back_inserter(polygon2) , shift_polygon(-left,-top));       

	view_t view2 = boost::gil::subimage_view(
		view,left,top,right-left,bottom-top);

	fill_pixels_t fill_pixels;	
	point_in_polygon<int> point_in(polygon2);
	for (int x = 0; x < view2.width(); ++x)
		for (int y = 0; y < view2.height(); ++y)
			if (point_in(x,y))
				fill_pixels(view2,x,y);
}

#endif
