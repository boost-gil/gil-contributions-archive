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

struct make_alpha_blend
{
	short alpha;
	make_alpha_blend(short alpha=255) : alpha(alpha)
	{
		//TODO: BOOST_ASSERT(alpha >= 0 && alpha <= 255);
	}

	template <typename type_t>
	void operator()(type_t& dst, const type_t& src)
	{
		double dbl = (dst * alpha - src * alpha + src * 255.0) / 255.0;
		dst = boost::numeric_cast<type_t>(dbl);
	}
};

template <unsigned long hex_>
struct rgbhex
{
	static const unsigned long hex = hex_;
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t pixel;

	rgbhex()
	{
		using namespace boost::gil;
		int red = static_cast<bits8>((hex & 0xFF0000) >> 16);
		int green = static_cast<bits8>((hex & 0x00FF00) >> 8);
		int blue = static_cast<bits8>(hex & 0x0000FF);
		pixel = pixel_t(red,green,blue);
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int a=255)
	{
		pixel_t dst = pixel;
		boost::gil::static_for_each(dst, view(x,y), make_alpha_blend(a));
		view(x,y) = dst;
	}
};

struct color
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t pixel;

	color(int red, int green, int blue)
	{
		pixel = pixel_t(red,green,blue);
	}

	color(pixel_t pixel) : pixel(pixel) {}

	color(unsigned long hex)
	{
		using namespace boost::gil;
		int red = static_cast<bits8>((hex & 0xFF0000) >> 16);
		int green = static_cast<bits8>((hex & 0x00FF00) >> 8);
		int blue = static_cast<bits8>(hex & 0x0000FF);
		pixel = pixel_t(red,green,blue);
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int a=255)
	{
		pixel_t dst = pixel;
		boost::gil::static_for_each(dst, view(x,y), make_alpha_blend(a));
		view(x,y) = dst;
	}
};

template <int r=0, int g=0, int b=0> 
struct rgb
{
	static const int red = r;
	static const int green = g;
	static const int blue = b;

	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t pixel;
	
	rgb()
	{
		pixel = pixel_t(r,g,b);
	}
		
	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int a=255)
	{
		pixel_t dst = pixel;
		boost::gil::static_for_each(dst, view(x,y), make_alpha_blend(a));
		view(x,y) = dst;
	}
};

template <int r=0, int g=0, int b=0, int a=255> 
struct rgba
{
	static const int red = r;
	static const int green = g;
	static const int blue = b;
	static const int alpha = a;

	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t pixel;

	rgba()
	{
		pixel = pixel_t(r,g,b);
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int unused=255)
	{
		pixel_t dst = pixel;
		boost::gil::static_for_each(dst, view(x,y), make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};

template <typename rgb_t, typename rgb2_t>
struct horizontal_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix,tpix;

	horizontal_gradient()
	{
		fpix = rgb_t().pixel;
		tpix = rgb2_t().pixel;
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int unused=255)
	{
		double perc = boost::numeric_cast<double>(x+1) / view.width();
		int alpha = boost::numeric_cast<int>(perc*255);
		pixel_t dst = tpix;
		boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};

template <typename rgb_t, typename rgb2_t>
struct vertical_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix,tpix;

	vertical_gradient()
	{
		fpix = rgb_t().pixel;
		tpix = rgb2_t().pixel;
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int unused=255)
	{
		double perc = boost::numeric_cast<double>(y+1) / view.height();
		int alpha = boost::numeric_cast<int>(perc*255);
		pixel_t dst = tpix;
		boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
		view(x,y) = dst;
	}
};

//TODO: improve this
template <typename rgb_t, typename rgb2_t, int stop>
struct vertical_gradient_stop
{
	BOOST_STATIC_ASSERT(stop > 0);

	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix,tpix;

	vertical_gradient_stop()
	{
		//TODO: ugly
		fpix = rgb_t().pixel;
		tpix = rgb2_t().pixel;
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int unused=255)
	{
		BOOST_ASSERT(stop*4 < view.height());

		if (y <= stop)
		{
			double perc = boost::numeric_cast<double>(y+1) / stop;
			int alpha = boost::numeric_cast<int>(perc*255);

			pixel_t dst = tpix;
			boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
			view(x,y) = dst;
		}
		else if (y > view.height()/2-stop && 
			y <= view.height()/2)
		{
			int yy = y-view.height()/2+stop;
			double perc = boost::numeric_cast<double>(yy+1) / stop;
			int alpha = boost::numeric_cast<int>(perc*255);
	
			pixel_t dst = fpix;
			boost::gil::static_for_each(dst, tpix, make_alpha_blend(alpha));
			view(x,y) = dst;
		}
		else if (y > view.height()/2 && 
			y <= view.height()/2+stop)
		{
			int yy = y-view.height()/2;
			double perc = boost::numeric_cast<double>(yy+1) / stop;
			int alpha = boost::numeric_cast<int>(perc*255);
		
			pixel_t dst = tpix;
			boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
			view(x,y) = dst;
		}
		else if (y > view.height()-stop)
		{
			int yy = view.height()-y;
			double perc = boost::numeric_cast<double>(yy+1) / stop;
			int alpha = boost::numeric_cast<int>(perc*255);
			pixel_t dst = tpix;
			boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
			view(x,y) = dst;
		}
		else
		{
			view(x,y) = tpix;
		}
	}
};

//TODO: broken
template <typename rgb_t, typename rgb2_t>
struct diagonal_gradient
{
	typedef boost::gil::rgb8_pixel_t pixel_t;
	pixel_t fpix,tpix;

	diagonal_gradient()
	{
		fpix = rgb_t().pixel;
		tpix = rgb2_t().pixel;
	}

	template <typename view_t>
	void operator()(const view_t& view, int x, int y, int unused=255)
	{
		double xperc = boost::numeric_cast<double>(x+1) / view.width();
		double yperc = boost::numeric_cast<double>(y+1) / view.height();
		double aperc = (xperc + yperc) / 2;

		int alpha = boost::numeric_cast<int>(aperc*255);
		pixel_t dst = tpix;
		boost::gil::static_for_each(dst, fpix, make_alpha_blend(alpha));
		view(x,y) = dst;
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

	//TODO: round
	out.x = boost::numeric_cast<int>(ix);
	out.y = boost::numeric_cast<int>(iy);
   	return true;
}

template <typename out_t> inline
void make_circle(int lines, double radius, int cx, int cy, out_t out)
{
	//TODO: use PI
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
	//TODO: PI
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

template <typename pixel_t, typename view_t>
void fill(const view_t& view)
{
	pixel_t pixel;	
	for (int x = 0; x < view.width(); ++x)
		for (int y = 0; y < view.height(); ++y)
			pixel(view,x,y);
}

template <typename pixel_t, typename view_t, typename polygon_t>
void fill(const view_t& view, const polygon_t& polygon)
{
	int left,right,top,bottom;
	std::for_each(polygon.begin(),polygon.end(),
		make_extents(left,top,right,bottom));       
	
	polygon_t polygon2;
	std::transform(polygon.begin(),polygon.end(),
		std::back_inserter(polygon2) , shift_polygon(-left,-top));       

	view_t view2 = boost::gil::subimage_view(
		view,left,top,right-left,bottom-top);

	pixel_t pixel;
	point_in_polygon<int> point_in(polygon2);
	for (int x = 0; x < view2.width(); ++x)
		for (int y = 0; y < view2.height(); ++y)
			if (point_in(x,y))
				pixel(view2,x,y);
}

#endif
