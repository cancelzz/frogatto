#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "SDL.h"

#include <string>

struct point {
	explicit point(const std::string& str);
	point(int x=0, int y=0) : x(x), y(y)
	{}

	std::string to_string() const;

	int x, y;
};

class rect {
public:
	static rect from_coordinates(int x1, int y1, int x2, int y2);
	explicit rect(const std::string& str);
	explicit rect(int x=0, int y=0, int w=0, int h=0);
	int x() const;
	int y() const;
	int x2() const;
	int y2() const;
	int w() const;
	int h() const;

	std::string to_string() const;

	SDL_Rect sdl_rect() const;
private:
	point top_left_, bottom_right_;
};

bool point_in_rect(const point& p, const rect& r);
bool rects_intersect(const rect& a, const rect& b);

#endif
